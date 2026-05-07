"""
=============================================================================
Purpose:
    YAML 策略配置文件的加载与验证管道。将人类可读的层级化 YAML（包含 model/
    robot/policy/observation 等 section）解析并扁平化为运行时所需的 AppCfg 数
    据类。AppCfg 是整个推理流水线（OnnxPolicyRunner、MotionFSM 等）的唯一配
    置源，保证参数在进入 C++/ONNX 边界之前均经过类型转换和范围校验。

Dependencies:
    - omegaconf:  YAML 解析与结构化 merge（需 pip install omegaconf）；
    - numpy:      将 list[float] 强制转换为 np.ndarray(N, float32)；
    被调用方：rl_deploy_basic.py::main() 通过 load_app_cfg() 获取 AppCfg 实例。

Architecture:
    两阶段 merge 设计：
    1. OmegaConf.merge(structured(AppCfgFile), raw_yaml)  ← 层级模式匹配
    2. 将 AppCfgFile 展平成字典后再 merge 到 structured(AppCfg) ← 扁平化
    3. AppCfg.validate() 完成所有边界校验并返回类型安全的冻结副本。
    ObsComponent 列表在 validate() 内被展开并验证总维度 == observation_size，
    以在启动阶段而非推理阶段暴露维度不匹配问题。
=============================================================================
"""
from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import Path

import numpy as np

try:
    from omegaconf import MISSING, OmegaConf
except ModuleNotFoundError as e:
    raise RuntimeError("This example requires `omegaconf` (pip package: `omegaconf`).") from e


@dataclass(frozen=True)
class ObsComponent:
    """单个观测分量的规格描述，与 YAML observation.components[i] 一一对应。

    Attributes:
        type:    分量类型字符串，决定其维度（由 component_dim() 解析）。
        scale:   标量缩放因子，在观测向量送入策略网络前逐元素相乘。
        indices: 若不为 None，则对该分量按此索引重排序——用于将模拟器关节
                 顺序（如 L6R6）映射到策略网络训练时的物理顺序。
    """

    type: str
    scale: float = 1.0
    indices: list[int] | None = None


def component_dim(typ: str) -> int:
    """根据观测分量类型返回其固定维度。

    Args:
        typ: 观测分量类型字符串，须为已知枚举之一。

    Returns:
        int: 该分量的向量维度。

    Raises:
        ValueError: typ 不在已知集合中时抛出，防止 YAML 中拼写错误在推理时
                    才暴露为形状不匹配。
    """
    # vx,vy,wz,base frame角速度IMU,body frame重力向量 模长为1
    if typ in ("command", "base_ang_vel", "projected_gravity"):
        return 3
    if typ in ("leg_pos", "leg_vel", "last_actions"):
        return 12
    if typ in ("imu_gyro", "imu_euler"):
        return 3
    if typ == "imu_quat":
        return 4
    raise ValueError(f"unsupported observation component type: {typ}")


@dataclass
class AppCfg:
    """运行时扁平化配置对象，是推理流水线的唯一数据源。

    所有字段在 validate() 完成类型转换后均为 Python 原生类型或 np.ndarray，
    不再包含 OmegaConf 代理对象，因此可安全跨线程传递。
    腿部/臂部/腰部/头部参数以物理关节顺序存储（L/R 各半）。
    """

    model_path: str = MISSING
    control_hz: float = MISSING
    sync_hz: float = MISSING
    action_scale: float = MISSING
    clip_actions: float = MISSING
    clip_obs: float = MISSING
    observation_size: int = MISSING
    num_hist: int = MISSING
    cycle_time: float = MISSING   #x2 omnidirectional walking无cycle_time
    cmd_threshold: float = MISSING
    sw_mode: bool = MISSING
    stacking_mode: str = "spread"  # "spread" or "term"
    leg_default_joint_angles: list[float] = MISSING  # -> np.ndarray(12,) in validate()
    leg_stiffness: list[float] = MISSING  # -> np.ndarray(12,) in validate()
    leg_damping: list[float] = MISSING  # -> np.ndarray(12,) in validate()
    arm_default_joint_angles: list[float] = field(default_factory=lambda: [0.0] * 14)  # -> np.ndarray(14,)
    arm_stiffness: list[float] = field(
        default_factory=lambda: [100.0, 200.0, 200.0, 100.0, 200.0, 200.0, 200.0] * 2
    )  # -> np.ndarray(14,)
    arm_damping: list[float] = field(default_factory=lambda: [4.0, 0.2, 0.2, 4.0, 0.2, 0.2, 0.2] * 2)  # -> np.ndarray(14,)
    arm_delta_pos_threshold: float = 0.05
    arm_emergency_damping: float = 3.0
    waist_default_joint_angles: list[float] = field(default_factory=lambda: [0.0] * 3)  # -> np.ndarray(3,)
    waist_stiffness: list[float] = field(default_factory=lambda: [200.0, 100.0, 100.0])  # -> np.ndarray(3,)
    waist_damping: list[float] = field(default_factory=lambda: [8.0, 5.0, 5.0])  # -> np.ndarray(3,)
    waist_delta_pos_threshold: float = 0.05
    waist_emergency_damping: float = 3.0
    head_default_joint_angles: list[float] = field(default_factory=lambda: [0.0] * 2)  # -> np.ndarray(2,)
    head_stiffness: list[float] = field(default_factory=lambda: [10.0, 10.0])  # -> np.ndarray(2,)
    head_damping: list[float] = field(default_factory=lambda: [1.0, 1.0])  # -> np.ndarray(2,)
    head_delta_pos_threshold: float = 0.05
    head_emergency_damping: float = 3.0

    obs_components: list[ObsComponent] = MISSING

    def validate(self, cfg_dir: Path, model_override: Path | None = None) -> AppCfg:
        """对所有字段执行范围校验与类型强制转换，返回经验证的 AppCfg 副本。

        这是从 OmegaConf 代理类型到纯 Python/NumPy 类型的最终边界。
        在此处提前校验可以在进程启动时暴露配置错误，而非在首次推理时崩溃。

        Args:
            cfg_dir:        YAML 文件所在目录，用于将相对 model_path 解析为绝对路径。
            model_override: 若不为 None，则覆盖 YAML 中的 model.path 字段。

        Returns:
            AppCfg: 字段已类型转换并通过所有断言的新实例。

        Raises:
            ValueError: 任何数值字段超出允许范围，或 observation_size 与
                        components 维度之和不符。
            KeyError:   obs_components 条目缺少必填 `type` 键。
            TypeError:  obs_components 条目类型既不是 dict 也不是 ObsComponent。
        """
        model_path_s = str(model_override) if model_override is not None else str(self.model_path)
        model_path = resolve_path(cfg_dir, model_path_s)

        control_hz = float(self.control_hz)
        sync_hz = float(self.sync_hz)
        if control_hz <= 0.0:
            raise ValueError("control.hz must be > 0")
        if sync_hz <= 0.0:
            raise ValueError("control.sync_hz must be > 0")

        action_scale = float(self.action_scale)
        clip_actions = float(self.clip_actions)
        clip_obs = float(self.clip_obs)
        if clip_actions <= 0.0:
            raise ValueError("policy.clip_actions must be > 0")
        if clip_obs <= 0.0:
            raise ValueError("observation.clip must be > 0")

        observation_size = int(self.observation_size)
        num_hist = int(self.num_hist)
        stacking_mode = str(self.stacking_mode).lower()
        if stacking_mode not in ("spread", "term"):
            raise ValueError(f"observation.stacking_mode must be 'spread' or 'term', got {stacking_mode}")

        if observation_size <= 0:
            raise ValueError("observation.size must be > 0")
        if num_hist <= 0:
            raise ValueError("observation.num_hist must be > 0")

        cycle_time = float(self.cycle_time)
        cmd_threshold = float(self.cmd_threshold)
        sw_mode = bool(self.sw_mode)
        if cycle_time <= 0.0:
            raise ValueError("phase.cycle_time must be > 0")
        if cmd_threshold < 0.0:
            raise ValueError("phase.cmd_threshold must be >= 0")

        leg_default_joint_angles = as_floats(self.leg_default_joint_angles, 12, "robot.leg_default_joint_angles")
        leg_stiffness = as_floats(self.leg_stiffness, 12, "robot.leg_stiffness")
        leg_damping = as_floats(self.leg_damping, 12, "robot.leg_damping")

        arm_default_joint_angles = as_floats(self.arm_default_joint_angles, 14, "robot.arm_default_joint_angles")
        arm_stiffness = as_floats(self.arm_stiffness, 14, "robot.arm_stiffness")
        arm_damping = as_floats(self.arm_damping, 14, "robot.arm_damping")

        arm_delta_pos_threshold = float(self.arm_delta_pos_threshold)
        arm_emergency_damping = float(self.arm_emergency_damping)
        if arm_delta_pos_threshold < 0.0:
            raise ValueError("robot.arm_delta_pos_threshold must be >= 0")
        if arm_emergency_damping < 0.0:
            raise ValueError("robot.arm_emergency_damping must be >= 0")

        waist_default_joint_angles = as_floats(self.waist_default_joint_angles, 3, "robot.waist_default_joint_angles")
        waist_stiffness = as_floats(self.waist_stiffness, 3, "robot.waist_stiffness")
        waist_damping = as_floats(self.waist_damping, 3, "robot.waist_damping")
        waist_delta_pos_threshold = float(self.waist_delta_pos_threshold)
        waist_emergency_damping = float(self.waist_emergency_damping)
        if waist_delta_pos_threshold < 0.0:
            raise ValueError("robot.waist_delta_pos_threshold must be >= 0")
        if waist_emergency_damping < 0.0:
            raise ValueError("robot.waist_emergency_damping must be >= 0")

        head_default_joint_angles = as_floats(self.head_default_joint_angles, 2, "robot.head_default_joint_angles")
        head_stiffness = as_floats(self.head_stiffness, 2, "robot.head_stiffness")
        head_damping = as_floats(self.head_damping, 2, "robot.head_damping")
        head_delta_pos_threshold = float(self.head_delta_pos_threshold)
        head_emergency_damping = float(self.head_emergency_damping)
        if head_delta_pos_threshold < 0.0:
            raise ValueError("robot.head_delta_pos_threshold must be >= 0")
        if head_emergency_damping < 0.0:
            raise ValueError("robot.head_emergency_damping must be >= 0")

        obs_components_raw = self.obs_components
        if not isinstance(obs_components_raw, list) or not obs_components_raw:
            raise ValueError("observation.components must be a non-empty list")
        obs_components: list[ObsComponent] = []
        for i, item in enumerate(obs_components_raw):
            if isinstance(item, ObsComponent):
                obs_components.append(item)
            elif isinstance(item, dict):
                if "type" not in item:
                    raise KeyError(f"observation.components[{i}] missing required key `type`")

                # 为换序构建indices
                typ = str(item["type"])
                dim = component_dim(typ)
                indices = item.get("indices", list(range(dim)))
                if len(indices) != dim:
                    raise ValueError(f"observation.components[{i}] indices length mismatch: expected {dim}, got {len(indices)}")

                # inv_indices logic is now implicitly handled by np.array mapping
                obs_components.append(ObsComponent(
                    type=str(item["type"]),
                    scale=float(item.get("scale", 1.0)),
                    indices=indices
                ))
            else:
                raise TypeError(f"observation.components[{i}] must be a dict, got {type(item).__name__}")

        # 预计算各分量维度之和，与 observation_size 对齐；在启动阶段早于推理阶段发现问题
        offset = 0
        for comp in obs_components:
            offset += component_dim(comp.type)
        if offset != observation_size:
            raise ValueError(f"observation.size mismatch: components sum={offset} cfg={observation_size}")

        return AppCfg(
            model_path=str(model_path),
            control_hz=control_hz,
            sync_hz=sync_hz,
            action_scale=action_scale,
            clip_actions=clip_actions,
            clip_obs=clip_obs,
            observation_size=observation_size,
            num_hist=num_hist,
            stacking_mode=stacking_mode,
            cycle_time=cycle_time,
            cmd_threshold=cmd_threshold,
            sw_mode=sw_mode,
            leg_default_joint_angles=leg_default_joint_angles,  # type: ignore[arg-type]
            leg_stiffness=leg_stiffness,  # type: ignore[arg-type]
            leg_damping=leg_damping,  # type: ignore[arg-type]
            arm_default_joint_angles=arm_default_joint_angles,  # type: ignore[arg-type]
            arm_stiffness=arm_stiffness,  # type: ignore[arg-type]
            arm_damping=arm_damping,  # type: ignore[arg-type]
            arm_delta_pos_threshold=arm_delta_pos_threshold,
            arm_emergency_damping=arm_emergency_damping,
            waist_default_joint_angles=waist_default_joint_angles,  # type: ignore[arg-type]
            waist_stiffness=waist_stiffness,  # type: ignore[arg-type]
            waist_damping=waist_damping,  # type: ignore[arg-type]
            waist_delta_pos_threshold=waist_delta_pos_threshold,
            waist_emergency_damping=waist_emergency_damping,
            head_default_joint_angles=head_default_joint_angles,  # type: ignore[arg-type]
            head_stiffness=head_stiffness,  # type: ignore[arg-type]
            head_damping=head_damping,  # type: ignore[arg-type]
            head_delta_pos_threshold=head_delta_pos_threshold,
            head_emergency_damping=head_emergency_damping,

            obs_components=obs_components,
        )


@dataclass
class ControlCfg:
    """YAML control section：控制回路频率配置。"""

    hz: float = MISSING
    sync_hz: float | None = None


@dataclass
class ModelCfg:
    """YAML model section：ONNX 权重文件路径（相对/绝对均可）。"""

    path: str = MISSING


@dataclass
class RobotCfg:
    """YAML robot section：各肢体的默认关节角、PD 增益及安全限幅参数。

    delta_pos_threshold 控制每控制步允许的最大关节位置变化量（rad），
    防止启动阶段关节目标跳变导致大力矩冲击。
    emergency_damping 在紧急停止时替换正常阻尼，提供软着陆。
    """

    leg_default_joint_angles: list[float] = MISSING
    leg_stiffness: list[float] = MISSING
    leg_damping: list[float] = MISSING
    arm_default_joint_angles: list[float] = field(default_factory=lambda: [0.0] * 14)
    arm_stiffness: list[float] = field(default_factory=lambda: [100.0, 200.0, 200.0, 100.0, 200.0, 200.0, 200.0] * 2)
    arm_damping: list[float] = field(default_factory=lambda: [4.0, 0.2, 0.2, 4.0, 0.2, 0.2, 0.2] * 2)
    arm_delta_pos_threshold: float = 0.05
    arm_emergency_damping: float = 3.0
    waist_default_joint_angles: list[float] = field(default_factory=lambda: [0.0] * 3)
    waist_stiffness: list[float] = field(default_factory=lambda: [100.0, 200.0, 200.0])
    waist_damping: list[float] = field(default_factory=lambda: [4.0, 0.2, 0.2])
    waist_delta_pos_threshold: float = 0.05
    waist_emergency_damping: float = 3.0
    head_default_joint_angles: list[float] = field(default_factory=lambda: [0.0] * 2)
    head_stiffness: list[float] = field(default_factory=lambda: [10.0, 10.0])
    head_damping: list[float] = field(default_factory=lambda: [1.0, 1.0])
    head_delta_pos_threshold: float = 0.05
    head_emergency_damping: float = 3.0


@dataclass
class PolicyCfg:
    """YAML policy section：动作空间缩放与裁剪系数。"""

    action_scale: float = MISSING
    clip_actions: float = MISSING


@dataclass
class PhaseCfg:
    """YAML phase section：步态相位钟控制参数。

    sw_mode（standing-wave mode）为 True 时，当速度指令低于 cmd_threshold
    时相位冻结在 0，机器人保持站立而非继续迈步。
    """

    cycle_time: float = MISSING
    sw_mode: bool = MISSING
    cmd_threshold: float = MISSING


@dataclass
class ObservationCfg:
    """YAML observation section：历史帧数、裁剪范围与分量列表。

    stacking_mode:
        "spread" — 将整个历史窗口按时间轴展开成一个大向量送入网络；
        "term"   — 将每个分量的历史单独拼接后再水平拼接（term-major stacking），
                   某些架构在此布局上训练效率更高。
    """

    size: int = MISSING
    num_hist: int = MISSING
    stacking_mode: str = "spread"
    clip: float = MISSING
    components: list[ObsComponent] = MISSING


@dataclass
class AppCfgFile:
    """YAML 文件的顶层结构，与磁盘上的层级键名一一对应。

    仅用于 OmegaConf 的两阶段 merge，不直接被推理代码使用。
    """

    control: ControlCfg = field(default_factory=ControlCfg)
    model: ModelCfg = field(default_factory=ModelCfg)
    robot: RobotCfg = field(default_factory=RobotCfg)
    policy: PolicyCfg = field(default_factory=PolicyCfg)
    phase: PhaseCfg = field(default_factory=PhaseCfg)
    observation: ObservationCfg = field(default_factory=ObservationCfg)


def resolve_path(base: Path, p: str) -> Path:
    """将配置文件中的路径解析为绝对路径。

    Args:
        base: YAML 文件所在目录，作为相对路径的锚点。
        p:    YAML 中读到的路径字符串，可为相对或绝对。

    Returns:
        Path: 绝对路径；若 p 本身已是绝对路径则直接返回，不附加 base。
    """
    path = Path(p)
    return path if path.is_absolute() else (base / path)


def as_floats(x: object, n: int, what: str) -> np.ndarray:
    """将 list 或 ndarray 强制转换为 shape=(n,) 的 float32 数组。

    Args:
        x:    原始值，接受 list[float] 或已有的 np.ndarray。
        n:    期望的元素数量。
        what: 字段名称字符串，仅用于错误信息定位。

    Returns:
        np.ndarray: shape=(n,), dtype=float32。

    Raises:
        TypeError:  x 既不是 list 也不是 ndarray。
        ValueError: x 展平后长度不等于 n。
    """
    arr: np.ndarray
    if isinstance(x, np.ndarray):
        arr = x.astype(np.float32, copy=False).reshape(-1)
    elif isinstance(x, list):
        arr = np.array([float(v) for v in x], dtype=np.float32)
    else:
        raise TypeError(f"{what} must be a list[{n}]")
    if arr.shape != (n,):
        raise ValueError(f"{what} must be a list[{n}]")
    return arr


def load_app_cfg(cfg_path: Path, model_override: Path | None = None) -> AppCfg:
    """从磁盘 YAML 加载、合并并验证完整的应用配置。

    两阶段 OmegaConf merge 设计：首先将 raw YAML 合并到结构化 AppCfgFile（层
    级格式），再将展平字典合并到结构化 AppCfg（扁平格式），最后调用 validate()
    完成所有边界校验。该设计使 YAML 可以维持可读的层级结构，同时运行时代码只
    需访问一个扁平对象。

    Args:
        cfg_path:       YAML 配置文件的绝对或相对路径。
        model_override: 若不为 None，则忽略 YAML 中的 model.path，使用该值。

    Returns:
        AppCfg: 已验证、类型安全的运行时配置实例。

    Raises:
        KeyError:   检测到旧版 deploy-style 配置 schema（不兼容）。
        ValueError: 任何字段校验失败（见 AppCfg.validate）。
        FileNotFoundError: cfg_path 不存在（由 OmegaConf.load 抛出）。
    """
    raw = OmegaConf.load(str(cfg_path))
    raw_container = OmegaConf.to_container(raw, resolve=False)
    # 旧版 deploy 配置包含顶层 LeggedRobotCfg 或 rl_controllers key，与当前 schema 不兼容
    if isinstance(raw_container, dict) and ("LeggedRobotCfg" in raw_container or "rl_controllers" in raw_container):
        raise KeyError(
            "deploy-style config is not supported by this example anymore; "
            "please use the example schema in `aimrl_sdk/examples/configs/agibot_a2_dof12.yaml`"
        )

    cfg_file = OmegaConf.merge(OmegaConf.structured(AppCfgFile), raw)
    cfg_obj: AppCfgFile = OmegaConf.to_object(cfg_file)

    control_hz = float(cfg_obj.control.hz)
    # sync_hz 未设置时默认与 control_hz 相同，表示同步采样与控制在同一频率
    sync_hz = float(cfg_obj.control.sync_hz) if cfg_obj.control.sync_hz is not None else control_hz

    app_dict = {
        "model_path": str(cfg_obj.model.path),
        "control_hz": control_hz,
        "sync_hz": sync_hz,
        "action_scale": float(cfg_obj.policy.action_scale),
        "clip_actions": float(cfg_obj.policy.clip_actions),
        "clip_obs": float(cfg_obj.observation.clip),
        "observation_size": int(cfg_obj.observation.size),
        "num_hist": int(cfg_obj.observation.num_hist),
        "stacking_mode": str(cfg_obj.observation.stacking_mode),
        "cycle_time": float(cfg_obj.phase.cycle_time),
        "cmd_threshold": float(cfg_obj.phase.cmd_threshold),
        "sw_mode": bool(cfg_obj.phase.sw_mode),
        "leg_default_joint_angles": list(cfg_obj.robot.leg_default_joint_angles),
        "leg_stiffness": list(cfg_obj.robot.leg_stiffness),
        "leg_damping": list(cfg_obj.robot.leg_damping),
        "arm_default_joint_angles": list(cfg_obj.robot.arm_default_joint_angles),
        "arm_stiffness": list(cfg_obj.robot.arm_stiffness),
        "arm_damping": list(cfg_obj.robot.arm_damping),
        "arm_delta_pos_threshold": float(cfg_obj.robot.arm_delta_pos_threshold),
        "arm_emergency_damping": float(cfg_obj.robot.arm_emergency_damping),
        "waist_default_joint_angles": list(cfg_obj.robot.waist_default_joint_angles),
        "waist_stiffness": list(cfg_obj.robot.waist_stiffness),
        "waist_damping": list(cfg_obj.robot.waist_damping),
        "waist_delta_pos_threshold": float(cfg_obj.robot.waist_delta_pos_threshold),
        "waist_emergency_damping": float(cfg_obj.robot.waist_emergency_damping),
        "head_default_joint_angles": list(cfg_obj.robot.head_default_joint_angles),
        "head_stiffness": list(cfg_obj.robot.head_stiffness),
        "head_damping": list(cfg_obj.robot.head_damping),
        "head_delta_pos_threshold": float(cfg_obj.robot.head_delta_pos_threshold),
        "head_emergency_damping": float(cfg_obj.robot.head_emergency_damping),
        "obs_components": list(cfg_obj.observation.components),
    }

    cfg_app = OmegaConf.merge(OmegaConf.structured(AppCfg), OmegaConf.create(app_dict))
    cfg: AppCfg = OmegaConf.to_object(cfg_app)
    return cfg.validate(cfg_dir=cfg_path.parent, model_override=model_override)
