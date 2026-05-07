"""
=============================================================================
Purpose:
    aimrl_sdk 的主控制循环入口。将 ONNX 策略推理、PD 关节控制、遥操作输入和
    遥测录制集成为一个完整的部署进程。支持三种运行模式：
    - default_stand：仿真重置后以 PD 保持站立姿态，不运行推理（调试用）；
    - walking：仿真重置后运行 RL 推理，驱动全身行走；
    - 无 --mode：等同 walking 但跳过 FSM 初始化可选（兼容模式）。

Dependencies:
    - aimrl_sdk:         C++ 绑定层，提供 open/close、State/Cmd API；
    - onnxruntime:       ONNX 模型推理后端（仅 walking 模式需要）；
    - teleop_control:    TeleopInput（手柄/键盘）和 MotionFSM（状态机）；
    - rl_deploy_config:  AppCfg（运行时配置）和 load_app_cfg()；
    - telemetry:         MCAP 录制和 Foxglove 实时推流（可选）；
    被调用方：命令行直接运行（`python rl_deploy_basic.py --cfg ...`）。

Architecture:
    启动流水线：
    1. 加载并验证 AppCfg（YAML → 类型安全 Python 对象）；
    2. 打开 aimrl_sdk（建立 AIMRT 传输连接，启动同步时钟）；
    3. 发送 /aima/sim/reset（仿真侧解除指令门控，机器人传送至站立姿态）；
    4. Init loop：等待第一个对齐完整帧，同时持续发送 PD Hold 防止机器人倒下；
    5. Main loop：以 control_hz 节拍运行：读帧 → FSM/推理 → 发布指令；
    控制节拍由 sleep_until() 的混合睡眠（粗睡+忙等）保证精度。
=============================================================================
"""
from __future__ import annotations

import argparse
import math
import sys
import time
from pathlib import Path

import numpy as np
from loguru import logger

import aimrl_sdk
from rl_deploy_config import AppCfg, component_dim, load_app_cfg
from teleop_control import MotionFSM, TeleopInput
from telemetry import FrameCaptureThread, Telemetry, TelemetryConfig


def sleep_until(deadline_s: float, *, spin_threshold_s: float = 0.002) -> None:
    """睡眠直到绝对时间点（perf_counter 基准），使用混合策略降低过睡误差。

    粗睡阶段（remaining > spin_threshold_s）调用 time.sleep() 节约 CPU；
    进入 spin_threshold_s 窗口后切换到忙等以获得亚毫秒级精度，对实时控制
    回路至关重要。

    Args:
        deadline_s:       目标唤醒时刻（time.perf_counter() 的秒数）。
        spin_threshold_s: 切换到忙等的剩余时间阈值，默认 2 ms。
    """
    while True:
        now = time.perf_counter()
        remaining = deadline_s - now
        if remaining <= 0.0:
            return

        if remaining > spin_threshold_s:
            time.sleep(max(0.0, remaining - spin_threshold_s))
            continue

        while time.perf_counter() < deadline_s:
            pass
        return


def quat_xyzw_to_euler_xyz(q_xyzw: np.ndarray) -> np.ndarray:
    """将 xyzw 四元数转换为 ZYX 约定欧拉角（roll, pitch, yaw）。

    用于遥测日志中的姿态可读化，不参与策略推理观测向量。

    Args:
        q_xyzw: shape=(4,) 四元数 [x, y, z, w]。

    Returns:
        np.ndarray: shape=(3,) dtype=float32，[roll, pitch, yaw]，单位 rad。
    """
    x, y, z, w = [float(v) for v in q_xyzw]

    sinr_cosp = 2.0 * (w * x + y * z)
    cosr_cosp = 1.0 - 2.0 * (x * x + y * y)
    roll = math.atan2(sinr_cosp, cosr_cosp)

    sinp = 2.0 * (w * y - z * x)
    # sinp 超出 [-1, 1] 对应万向锁奇异点，用 copysign 钳制到 ±90°
    if abs(sinp) >= 1.0:
        pitch = math.copysign(math.pi / 2.0, sinp)
    else:
        pitch = math.asin(sinp)

    siny_cosp = 2.0 * (w * z + x * y)
    cosy_cosp = 1.0 - 2.0 * (y * y + z * z)
    yaw = math.atan2(siny_cosp, cosy_cosp)

    return np.array([roll, pitch, yaw], dtype=np.float32)


def _compute_projected_gravity(q_xyzw: np.ndarray) -> np.ndarray:
    """将世界坐标系 [0, 0, -1] 投影到机身坐标系

    将世界坐标系重力方向 g_w=[0,0,-1] 通过四元数的旋转矩阵变换到机身坐标系，
    得到机身系下的重力单位向量。该向量直接反映机器人的倾斜状态，是比欧拉角
    更适合网络输入的姿态特征（避免万向锁奇异性，模长恒为 1）。

    Args:
        q_xyzw: shape=(4,) 机身姿态四元数 [x, y, z, w]，需已归一化。

    Returns:
        np.ndarray: shape=(3,) dtype=float32，机身系下的重力单位向量。
    """
    x, y, z, w = q_xyzw

    # 直接展开 R^T * [0, 0, -1]，避免显式构造 3×3 旋转矩阵
    res = np.zeros(3, dtype=np.float32)
    res[0] = -2.0 * (x * z - w * y)
    res[1] = -2.0 * (y * z + w * x)
    res[2] = -(1.0 - 2.0 * (x * x + y * y))
    return res


def hold_joints_towards_target(cur_pos: np.ndarray, target_pos: np.ndarray, max_delta_pos: float) -> np.ndarray:
    """将当前关节位置向目标位置移动，每步限幅 max_delta_pos（rad）。

    用于臂/腰/头在主控制循环中的渐进归零，防止启动或模式切换时的关节目标
    跳变产生大力矩冲击。

    Args:
        cur_pos:       当前关节位置 shape=(N,)。
        target_pos:    目标关节位置 shape=(N,)，须与 cur_pos 形状一致。
        max_delta_pos: 单步最大位置增量 [rad]，> 0。若 <= 0 则保持当前位置不动。

    Returns:
        np.ndarray: shape=(N,) dtype=float64，本步应发送的关节位置目标。

    Raises:
        ValueError: cur_pos 与 target_pos 形状不匹配。
    """
    cur = cur_pos.astype(np.float64, copy=False).reshape(-1)
    target = target_pos.astype(np.float64, copy=False).reshape(-1)
    if cur.shape != target.shape:
        raise ValueError(f"shape mismatch: cur={cur.shape} target={target.shape}")
    if max_delta_pos <= 0.0:
        return cur.copy()
    delta = target - cur
    step = np.clip(delta, -max_delta_pos, max_delta_pos)
    return cur + step


class OnnxPolicyRunner:
    """有状态的 ONNX 策略推理包装器，管理历史帧堆叠、相位钟和动作重排序。

    内部维护一个 shape=(num_hist, observation_size) 的历史缓冲区，每步滚动
    更新后送入网络。支持两种 stacking_mode：
    - "spread"：将整个历史矩阵展平为一维向量（num_hist × obs_size）；
    - "term"：按观测分量分别堆叠历史，再水平拼接（某些架构训练效率更高）。

    Args:
        cfg: 已验证的 AppCfg 实例，提供模型路径、观测配置和动作缩放参数。
    """

    def __init__(self, cfg: AppCfg):
        try:
            import onnxruntime as ort
        except Exception as e:
            raise RuntimeError("onnxruntime is required to run model inference") from e

        self.cfg = cfg
        self.session = ort.InferenceSession(str(cfg.model_path), providers=["CPUExecutionProvider"])
        self.input_name = self.session.get_inputs()[0].name
        self.output_name = self.session.get_outputs()[0].name

        in_shape = self.session.get_inputs()[0].shape
        if len(in_shape) == 2 and isinstance(in_shape[1], int):
            expect = int(cfg.observation_size) * int(cfg.num_hist)
            if in_shape[1] != expect:
                raise ValueError(f"model input dim mismatch: model={in_shape[1]} cfg={expect}")

        self.last_actions = np.zeros((12,), dtype=np.float32)
        self.hist = np.zeros((cfg.num_hist, cfg.observation_size), dtype=np.float32)
        self.is_first = True
        self.phase_start_time = time.time()
        self.phase = 0.0
        self._last_actions_slices: list[slice] = []

        self.leg_indices = None
        self._term_slices: list[tuple[int, int]] = []
        offset = 0
        for comp in cfg.obs_components:
            dim = component_dim(comp.type)
            self._term_slices.append((offset, offset + dim))
            if comp.type == "last_actions":
                self._last_actions_slices.append(slice(offset, offset + dim))
            if comp.type == "leg_pos":
                # 记录 leg_pos 分量的物理→模型顺序映射，用于推理后将动作逆映射回物理关节顺序
                self.leg_indices = np.array(comp.indices)
            offset += dim
        if offset != cfg.observation_size:
            raise ValueError(f"observation.size mismatch: components sum={offset} cfg={cfg.observation_size}")

    def _update_phase(self, cmd_x: float, cmd_y: float, cmd_yaw: float) -> None:
        """推进步态相位钟，sw_mode 下速度归零时相位冻结。

        sw_mode（standing-wave mode）：当速度指令低于 cmd_threshold 时，
        相位钳制为 0 而非继续累积，机器人停止迈步保持站立。
        这避免了"空转步态"——即速度=0 但关节仍做步态振荡的问题。
        """
        if not self.cfg.sw_mode:
            t = time.time() - self.phase_start_time
            self.phase = (t / self.cfg.cycle_time) % 1.0
            return

        cmd_norm = math.sqrt(cmd_x * cmd_x + cmd_y * cmd_y + cmd_yaw * cmd_yaw)
        if cmd_norm <= self.cfg.cmd_threshold:
            self.phase = 0.0
            self.phase_start_time = time.time()
            return

        t = time.time() - self.phase_start_time
        self.phase = (t / self.cfg.cycle_time) % 1.0

    def _build_step_observation(self, obs: np.ndarray, cmd_x: float, cmd_y: float, cmd_yaw: float) -> np.ndarray:
        """将原始传感器观测拼装为网络输入的单步观测向量。

        按 cfg.obs_components 的顺序逐分量提取、缩放、重排，最终拼接为
        shape=(observation_size,) 的 float32 向量。

        Args:
            obs:   完整的 SDK 观测向量（aimrl_sdk.OBS 索引定义各字段位置）。
            cmd_x/y/yaw: 当前速度指令，注入到 "command" 分量。

        Returns:
            np.ndarray: shape=(observation_size,) dtype=float32，单步观测。
        """
        out_parts: list[np.ndarray] = []
        for comp in self.cfg.obs_components:
            typ = comp.type
            scale = comp.scale
            indices = comp.indices
            # inv_indices = comp.inv_indices

            if typ == "command":
                out_parts.append(
                    np.array(
                        [
                            # math.sin(2.0 * math.pi * self.phase),
                            # math.cos(2.0 * math.pi * self.phase),
                            cmd_x,
                            cmd_y,
                            cmd_yaw,
                        ],
                        dtype=np.float32,
                    ) * scale
                )
            elif typ == "leg_pos":
                leg_pos = obs[aimrl_sdk.OBS.leg_pos].astype(np.float32, copy=False)
                # 减去默认站立角度得到相对偏差，再按 indices 重排为网络训练时的关节顺序
                leg_relative_pos = leg_pos - self.cfg.leg_default_joint_angles
                leg_reordered_pos = leg_relative_pos[indices]
                out_parts.append(leg_reordered_pos * scale)
            elif typ == "leg_vel":
                leg_vel = obs[aimrl_sdk.OBS.leg_vel].astype(np.float32, copy=False)
                leg_reordered_vel = leg_vel[indices]
                out_parts.append(leg_reordered_vel * scale)
            elif typ == "last_actions":
                out_parts.append(self.last_actions)
            elif typ in ("base_ang_vel", "imu_gyro"):
                imu_gyro = obs[aimrl_sdk.OBS.imu_gyro_xyz].astype(np.float32, copy=False)
                out_parts.append((imu_gyro if indices is None else imu_gyro[indices]) * scale)
            elif typ == "imu_euler":
                imu_quat = obs[aimrl_sdk.OBS.imu_quat_xyzw].astype(np.float32, copy=False)
                out_parts.append(quat_xyzw_to_euler_xyz(imu_quat) * scale)
            elif typ == "imu_quat":
                imu_quat = obs[aimrl_sdk.OBS.imu_quat_xyzw].astype(np.float32, copy=False)
                out_parts.append(imu_quat * scale)
            elif typ == "projected_gravity":
                # 读取四元数并投影 (3维)
                q_xyzw = obs[aimrl_sdk.OBS.imu_quat_xyzw].astype(np.float32, copy=False)
                grav_b = _compute_projected_gravity(q_xyzw)
                out_parts.append((grav_b if indices is None else grav_b[indices]) * scale)
            else:
                raise ValueError(f"unsupported observation component type: {typ}")

        step_obs = np.concatenate(out_parts, dtype=np.float32)
        if step_obs.shape != (self.cfg.observation_size,):
            raise RuntimeError(
                f"unexpected step observation shape {step_obs.shape}, expected {(self.cfg.observation_size,)}"
            )
        return step_obs

    def step(self, obs: np.ndarray, cmd_x: float, cmd_y: float, cmd_yaw: float) -> np.ndarray:
        """执行单步策略推理，返回物理关节顺序的目标位置（含默认角度偏置）。

        Args:
            obs:         当前 SDK 观测向量。
            cmd_x/y/yaw: 当前速度指令（m/s, m/s, rad/s）。

        Returns:
            np.ndarray: shape=(12,) dtype=float32，腿部目标关节位置 [rad]，
                        已加上 leg_default_joint_angles 偏置，可直接发送给硬件。

        Side Effects:
            - 更新 self.hist（历史缓冲区滚动）；
            - 更新 self.last_actions（供下一步的 last_actions 分量使用）；
            - 推进 self.phase。
        """
        self._update_phase(cmd_x, cmd_y, cmd_yaw)
        step_obs = self._build_step_observation(obs, cmd_x, cmd_y, cmd_yaw)

        # 按块组合历史
        if self.is_first:
            # 首帧：用当前观测填充全部历史，但将 last_actions 分量置零，
            # 避免用未初始化的动作污染历史缓冲区
            step0 = step_obs.copy()
            for s in self._last_actions_slices:
                step0[s] = 0.0
            self.hist[:] = step0
            self.is_first = False
        else:
            # 滚动历史缓冲区：最旧帧被覆盖，最新帧写入末尾
            self.hist[:-1] = self.hist[1:]
            self.hist[-1] = step_obs

        if self.cfg.stacking_mode == "term":
            # term stacking：对每个观测分量，单独堆叠其历史后拼接
            # 布局：[comp0_t0..tN, comp1_t0..tN, ...] 而非 spread 的 [t0_all, t1_all, ...]
            term_parts = []
            for start, end in self._term_slices:
                term_parts.append(self.hist[:, start:end].reshape(-1))
            inp = np.concatenate(term_parts).reshape(1, -1)
        else:
            inp = self.hist.reshape(1, -1)

        np.clip(inp, -self.cfg.clip_obs, self.cfg.clip_obs, out=inp)

        out = self.session.run([self.output_name], {self.input_name: inp})[0]
        actions = out.reshape(-1).astype(np.float32, copy=False) # 模型顺序的actions

        if actions.shape != (12,):
            raise RuntimeError(f"unexpected actions shape {actions.shape}")

        np.clip(actions, -self.cfg.clip_actions, self.cfg.clip_actions, out=actions)
        self.last_actions = actions.copy() # 模型顺序的last_actions

        # 换序回到物理顺序 (L6, R6)：模型输出按训练时关节顺序，需逆映射回硬件物理顺序
        phys_actions = np.zeros(12, dtype=np.float32)
        if self.leg_indices is not None:
            phys_actions[self.leg_indices] = actions
        else:
            phys_actions = actions

        # 动作为相对于默认站立角度的增量，乘以 action_scale 后加上默认角度得到绝对目标
        return phys_actions * self.cfg.action_scale + self.cfg.leg_default_joint_angles


def _setup_logger() -> None:
    """移除默认 handler，配置带毫秒时间戳的 stderr 输出。"""
    logger.remove()
    logger.add(sys.stderr, format="[{time:YYYY-MM-DD HH:mm:ss.SSS}] [{level}] {message}", level="INFO")


def parse_args() -> argparse.Namespace:
    """解析命令行参数，返回 argparse.Namespace。

    Returns:
        argparse.Namespace: 所有命令行参数，未指定的使用默认值。
    """
    examples_dir = Path(__file__).resolve().parent
    default_cfg = examples_dir / "configs" / "agibot_a2_dof12.yaml"

    p = argparse.ArgumentParser()
    p.add_argument(
        "--aimrt-backend",
        type=str,
        default="iceoryx",
        choices=["iceoryx", "ros2"],
        help="Select built-in AimRT backend (default: iceoryx)",
    )
    p.add_argument(
        "--aimrt-config-path",
        type=Path,
        default=None,
        help="Custom AimRT backend YAML path (overrides --aimrt-backend)",
    )
    p.add_argument("--control-hz", type=float, default=None)
    p.add_argument("--sync-hz", type=float, default=None)
    p.add_argument(
        "--align-delay-ms",
        type=float,
        default=0.0,
        help="Core knob (ms): wait this long after each tick before producing a frame. Larger improves interpolation alignment but adds fixed latency (default: 0.0).",
    )
    p.add_argument("--model", type=Path, default=None)
    p.add_argument("--cfg", type=Path, default=default_cfg)
    p.add_argument("--cmd-x", type=float, default=0.0, help="initial command x (forward)")
    p.add_argument("--cmd-y", type=float, default=0.0, help="initial command y (left/right)")
    p.add_argument("--cmd-yaw", type=float, default=0.0, help="initial command yaw (turn)")
    p.add_argument("--joystick", type=Path, default=Path("/dev/input/js0"), help="Linux joystick device path")
    p.add_argument(
        "--no-fsm",
        action="store_true",
        help="Disable motion FSM; start directly in WALK (policy) mode and only gate motion by deadman",
    )
    p.add_argument(
        "--mode",
        type=str,
        default=None,
        choices=["default_stand", "walking"],
        help=(
            "Simulation mode. 'default_stand': reset sim, hold at standing pose (no inference). "
            "'walking': reset sim, then run RL inference loop. "
            "If omitted, no reset is sent and the walking inference loop runs directly."
        ),
    )
    p.add_argument("--enable-statistics", action="store_true", help="Enable aimrl_sdk runtime statistics (default off)")
    p.add_argument(
        "--statistics-sample-every",
        type=int,
        default=1,
        help="Statistics sampling: aggregate 1/N events (default: 1)",
    )
    p.add_argument(
        "--statistics-ema-shift",
        type=int,
        default=4,
        help="Statistics EMA shift (alpha=1/2^shift, default: 4)",
    )
    p.add_argument(
        "--statistics-log-every-s",
        type=float,
        default=0.0,
        help="Log statistics snapshot every N seconds (0 disables, default: 0)",
    )
    p.add_argument(
        "--telemetry",
        type=str,
        default="off",
        choices=["off", "live", "record"],
        help="Telemetry mode: off|live|record (default: off)",
    )
    p.add_argument(
        "--telemetry-path",
        type=Path,
        default=Path("log"),
        help="Recording output path (directory or .mcap file) for telemetry=record (default: ./log)",
    )
    p.add_argument(
        "--telemetry-host",
        type=str,
        default="0.0.0.0",
        help="Foxglove live server bind host for telemetry=live (default: 0.0.0.0)",
    )
    p.add_argument(
        "--telemetry-port",
        type=int,
        default=8765,
        help="Foxglove live WebSocket port for telemetry=live (default: 8765)",
    )
    p.add_argument(
        "--telemetry-live-hz",
        type=float,
        default=20.0,
        help="Downsample rate for telemetry=live (Hz, default: 20)",
    )
    p.add_argument(
        "--telemetry-queue",
        type=int,
        default=2000,
        help="Telemetry queue size (default: 2000). When full, drops oldest messages.",
    )
    return p.parse_args()


def main() -> None:
    """主控制循环：初始化 SDK → 发送仿真重置 → 等待首帧 → 进入实时控制循环。

    控制循环以 control_hz 为节拍，每帧执行：
    1. wait_frame()：阻塞直到下一个对齐传感器帧就绪；
    2. 检查输入（TeleopInput.poll()）和状态机（MotionFSM.on_input()）；
    3. 根据模式计算腿部目标（default_stand / fsm.step / policy.step）；
    4. 计算臂/腰/头渐进跟踪目标（hold_joints_towards_target）；
    5. cmd.commit() 将本帧指令通过 AIMRT 发布到机器人。

    Side Effects:
        - 打开并最终关闭 aimrl_sdk（建立/断开 AIMRT 传输连接）；
        - 若 --telemetry 启用，后台录制或推流数据；
        - 将终端切换至原始模式（teleop_control.RawKeyboard），退出时恢复。

    Raises:
        FileNotFoundError: cfg 或 model 文件不存在。
        KeyboardInterrupt: Ctrl-C 或手柄退出信号，优雅退出。
    """
    args = parse_args()
    _setup_logger()

    if not args.cfg.exists():
        raise FileNotFoundError(f"cfg not found: {args.cfg}")

    app_cfg = load_app_cfg(args.cfg, model_override=args.model)
    needs_model = args.mode != "default_stand"
    if needs_model and not Path(app_cfg.model_path).exists():
        raise FileNotFoundError(f"model not found: {app_cfg.model_path}")

    cmd_x = float(args.cmd_x)
    cmd_y = float(args.cmd_y)
    cmd_yaw = float(args.cmd_yaw)

    control_hz = float(args.control_hz) if args.control_hz is not None else app_cfg.control_hz
    sync_hz = float(args.sync_hz) if args.sync_hz is not None else app_cfg.sync_hz
    policy = OnnxPolicyRunner(app_cfg) if needs_model else None

    open_kwargs = dict(
        sync_hz=sync_hz,
        sync_phase_ms=0.0,
        sync_clock="fixed",
        align_delay_ms=float(args.align_delay_ms),
        enable_statistics=bool(args.enable_statistics),
        statistics_sample_every=int(args.statistics_sample_every),
        statistics_ema_shift=int(args.statistics_ema_shift),
    )
    if args.aimrt_config_path is not None:
        state, cmd = aimrl_sdk.open(config_path=args.aimrt_config_path, **open_kwargs)
        logger.info(f"AimRT config: {args.aimrt_config_path}")
    else:
        state, cmd = aimrl_sdk.open(aimrt_backend=str(args.aimrt_backend), **open_kwargs)
        logger.info(f"AimRT backend: {args.aimrt_backend}")

    logger.info(f"Opened AimRL SDK successfully (sync_hz={sync_hz})")
    # 无论仿真还是实机，均发送一次重置信号：
    # - 仿真：解除 MuJoCo 的指令门控（cmd_enabled_），机器人传送至站立姿态；
    # - 实机：/aima/sim/reset 无订阅者，消息静默丢弃，无副作用。
    logger.info(f"Sending sim reset to /aima/sim/reset (no-op on real hardware)")
    cmd.request_sim_reset()
    time.sleep(0.5)  # allow MuJoCo to process the reset before first frame arrives
    if args.mode is not None:
        logger.info(f"Mode: {args.mode}")
    if needs_model:
        logger.info(f"ONNX policy: {app_cfg.model_path}")
    if args.enable_statistics:
        logger.info(
            "Statistics enabled "
            f"(sample_every={int(args.statistics_sample_every)}, ema_shift={int(args.statistics_ema_shift)})"
        )

    # Pre-compute hold commands (sent during init to prevent robot from falling before main loop)
    _init_leg_pos = np.array(app_cfg.leg_default_joint_angles, dtype=np.float64)
    _init_leg_stiff = np.array(app_cfg.leg_stiffness, dtype=np.float64)
    _init_leg_damp = np.array(app_cfg.leg_damping, dtype=np.float64)
    _init_arm_pos = np.array(app_cfg.arm_default_joint_angles, dtype=np.float64)
    _init_arm_stiff = np.array(app_cfg.arm_stiffness, dtype=np.float64)
    _init_arm_damp = np.array(app_cfg.arm_damping, dtype=np.float64)
    _init_waist_pos = np.array(app_cfg.waist_default_joint_angles, dtype=np.float64)
    _init_waist_stiff = np.array(app_cfg.waist_stiffness, dtype=np.float64)
    _init_waist_damp = np.array(app_cfg.waist_damping, dtype=np.float64)
    _init_head_pos = np.array(app_cfg.head_default_joint_angles, dtype=np.float64)
    _init_head_stiff = np.array(app_cfg.head_stiffness, dtype=np.float64)
    _init_head_damp = np.array(app_cfg.head_damping, dtype=np.float64)

    stats_started = not args.enable_statistics
    # Init loop：持续发送 PD Hold 指令直到收到第一个对齐完整帧，防止仿真中机器人在
    # 控制回路启动前因无指令而倒下
    while True:
        logger.info("Waiting for first aligned frame")
        stamp_ns, aligned, complete, _ = state.wait_frame(timeout_s=1.0)
        if stamp_ns > 0:
            cmd.set_leg(position=_init_leg_pos, stiffness=_init_leg_stiff, damping=_init_leg_damp)
            cmd.set_arm(position=_init_arm_pos, stiffness=_init_arm_stiff, damping=_init_arm_damp)
            cmd.set_waist(position=_init_waist_pos, stiffness=_init_waist_stiff, damping=_init_waist_damp)
            cmd.set_head(position=_init_head_pos, stiffness=_init_head_stiff, damping=_init_head_damp)
            cmd.commit(stamp_ns=stamp_ns)
        if args.enable_statistics and (not stats_started) and complete and stamp_ns > 0:
            state.reset_statistics()
            stats_started = True
            logger.info("Statistics reset; starting collection after first complete frame")
        if aligned and complete and stamp_ns > 0:
            logger.info(f"Received first aligned frame at timestamp: {stamp_ns / 1e9:.3f} s")
            break

    telemetry_mode = str(args.telemetry).strip().lower()
    telemetry_cfg = TelemetryConfig(
        mode=telemetry_mode,
        record_mcap=args.telemetry_path if telemetry_mode == "record" else None,
        foxglove_host=str(args.telemetry_host),
        foxglove_port=int(args.telemetry_port) if telemetry_mode == "live" else None,
        foxglove_live_hz=float(args.telemetry_live_hz),
        queue_size=int(args.telemetry_queue),
    )
    telemetry_enabled = telemetry_mode != "off"
    telemetry = None
    frame_capture = None
    if telemetry_enabled:
        telemetry = Telemetry(
            telemetry_cfg,
            metadata={
                "aimrt_backend": str(args.aimrt_backend),
                "aimrt_config_path": str(args.aimrt_config_path) if args.aimrt_config_path is not None else "",
                "cfg_path": str(args.cfg),
                "model_path": str(app_cfg.model_path),
                "control_hz": f"{control_hz:.6f}",
                "sync_hz": f"{sync_hz:.6f}",
            },
        )
        telemetry.start()
        frame_capture = FrameCaptureThread(state, telemetry, timeout_s=1.0)
        frame_capture.start()

    dt = 1.0 / control_hz
    log_every = max(1, int(control_hz))
    arm_target = np.array(app_cfg.arm_default_joint_angles, dtype=np.float64)
    arm_stiffness_nom = np.array(app_cfg.arm_stiffness, dtype=np.float64)
    arm_damping_nom = np.array(app_cfg.arm_damping, dtype=np.float64)
    arm_zero_stiffness = np.zeros(aimrl_sdk.OBS.arm_dof, dtype=np.float64)
    arm_emergency_damping = float(app_cfg.arm_emergency_damping)
    arm_emergency_damping_arr = np.full(aimrl_sdk.OBS.arm_dof, arm_emergency_damping, dtype=np.float64)
    arm_max_delta = float(app_cfg.arm_delta_pos_threshold)
    waist_target = np.array(app_cfg.waist_default_joint_angles, dtype=np.float64)
    waist_stiffness_nom = np.array(app_cfg.waist_stiffness, dtype=np.float64)
    waist_damping_nom = np.array(app_cfg.waist_damping, dtype=np.float64)
    waist_zero_stiffness = np.zeros(aimrl_sdk.OBS.waist_dof, dtype=np.float64)
    waist_emergency_damping = float(app_cfg.waist_emergency_damping)
    waist_emergency_damping_arr = np.full(aimrl_sdk.OBS.waist_dof, waist_emergency_damping, dtype=np.float64)
    waist_max_delta = float(app_cfg.waist_delta_pos_threshold)
    head_target = np.array(app_cfg.head_default_joint_angles, dtype=np.float64)
    head_stiffness_nom = np.array(app_cfg.head_stiffness, dtype=np.float64)
    head_damping_nom = np.array(app_cfg.head_damping, dtype=np.float64)
    head_zero_stiffness = np.zeros(aimrl_sdk.OBS.head_dof, dtype=np.float64)
    head_emergency_damping = float(app_cfg.head_emergency_damping)
    head_emergency_damping_arr = np.full(aimrl_sdk.OBS.head_dof, head_emergency_damping, dtype=np.float64)
    head_max_delta = float(app_cfg.head_delta_pos_threshold)

    teleop = TeleopInput(args.joystick, init_cmd=(cmd_x, cmd_y, cmd_yaw))
    fsm = MotionFSM(app_cfg) if not args.no_fsm else None
    emergency_stop = False
    stats_next_log_t = time.monotonic() + max(0.0, float(args.statistics_log_every_s))

    try:
        loop_idx = 0
        last_stamp_ns = 0
        last_aligned = True
        last_align_warn_t = 0.0
        while True:
            loop_idx += 1
            try:
                stamp_ns, aligned, complete, obs = state.wait_frame(timeout_s=1.0)
            except Exception:
                logger.warning("wait_frame timeout/stopped; retrying")
                continue

            dt_step = dt
            if int(stamp_ns) > 0 and last_stamp_ns > 0:
                dt_s = (int(stamp_ns) - int(last_stamp_ns)) / 1e9
                # 使用实际时间间隔驱动过渡插值，滤除明显异常跳变（> 200 ms）
                if 0.0 < dt_s < 0.2:
                    dt_step = dt_s
            if int(stamp_ns) > 0:
                last_stamp_ns = int(stamp_ns)

            if not complete:
                now = time.monotonic()
                if last_aligned or (now - last_align_warn_t) >= 1.0:
                    logger.warning("Latest frame is incomplete (missing arm/leg/imu/waist/head); using held last observation")
                    last_align_warn_t = now
                last_aligned = False
            elif not aligned:
                now = time.monotonic()
                if last_aligned or (now - last_align_warn_t) >= 1.0:
                    logger.warning("Latest frame is not aligned (skew too large)")
                    last_align_warn_t = now
                last_aligned = False
            elif not last_aligned:
                logger.info("Frames are aligned again")
                last_aligned = True

            snap = teleop.poll()
            if snap.quit_edge:
                raise KeyboardInterrupt
            if fsm is not None:
                fsm.on_input(snap, obs)
            else:
                if snap.emergency_stop_edge:
                    emergency_stop = True
                    logger.warning("Emergency stop set (no-fsm mode); press 'p' to clear")
                if snap.start_control_edge and emergency_stop:
                    emergency_stop = False
                    logger.warning("Emergency stop cleared (no-fsm mode)")

            start_time = time.perf_counter()
            if args.mode == "default_stand":
                # PD-only hold at standing pose; no inference.
                leg_pos_des = _init_leg_pos.copy()
                leg_stiffness = _init_leg_stiff.copy()
                leg_damping = _init_leg_damp.copy()
                arm_emergency = False
                waist_emergency = False
                head_emergency = False
            elif fsm is not None:
                leg_pos_des, leg_stiffness, leg_damping = fsm.step(obs, policy, snap, dt_step)
                arm_emergency = bool(fsm.emergency_stop)
                waist_emergency = bool(fsm.emergency_stop)
                head_emergency = bool(fsm.emergency_stop)
            else:
                if emergency_stop:
                    leg_pos_des = obs[aimrl_sdk.OBS.leg_pos].astype(np.float64, copy=False)
                    leg_stiffness = np.zeros(12, dtype=np.float64)
                    leg_damping = np.array(app_cfg.leg_damping, dtype=np.float64)
                    arm_emergency = True
                    waist_emergency = True
                    head_emergency = True
                else:
                    leg_pos_des = policy.step(obs, snap.cmd_x, snap.cmd_y, snap.cmd_yaw).astype(np.float64, copy=False)
                    leg_stiffness = np.array(app_cfg.leg_stiffness, dtype=np.float64)
                    leg_damping = np.array(app_cfg.leg_damping, dtype=np.float64)
                    arm_emergency = False
                    waist_emergency = False
                    head_emergency = False

            arm_pos_cur = obs[aimrl_sdk.OBS.arm_pos].astype(np.float64, copy=False)
            if arm_emergency:
                arm_pos_des = arm_pos_cur
                arm_stiffness = arm_zero_stiffness
                arm_damping = arm_emergency_damping_arr
            else:
                arm_pos_des = hold_joints_towards_target(
                    arm_pos_cur,
                    arm_target,
                    arm_max_delta,
                )
                arm_stiffness = arm_stiffness_nom
                arm_damping = arm_damping_nom
            waist_pos_cur = obs[aimrl_sdk.OBS.waist_pos].astype(np.float64, copy=False)
            if waist_emergency:
                waist_pos_des = waist_pos_cur
                waist_stiffness = waist_zero_stiffness
                waist_damping = waist_emergency_damping_arr
            else:
                waist_pos_des = hold_joints_towards_target(
                    waist_pos_cur,
                    waist_target,
                    waist_max_delta,
                )
                waist_stiffness = waist_stiffness_nom
                waist_damping = waist_damping_nom
            head_pos_cur = obs[aimrl_sdk.OBS.head_pos].astype(np.float64, copy=False)
            if head_emergency:
                head_pos_des = head_pos_cur
                head_stiffness = head_zero_stiffness
                head_damping = head_emergency_damping_arr
            else:
                head_pos_des = hold_joints_towards_target(
                    head_pos_cur,
                    head_target,
                    head_max_delta,
                )
                head_stiffness = head_stiffness_nom
                head_damping = head_damping_nom

            end_time = time.perf_counter()
            if loop_idx % log_every == 0:
                if args.mode == "default_stand":
                    logger.info(
                        f"mode=default_stand step_time={(end_time - start_time) * 1000.0:.3f} ms"
                    )
                elif fsm is not None:
                    logger.info(
                        f"mode={fsm.mode.value} start={int(fsm.start_control)} emg={int(fsm.emergency_stop)} "
                        f"deadman={int(snap.deadman)} cmd=({snap.cmd_x:.2f},{snap.cmd_y:.2f},{snap.cmd_yaw:.2f}) "
                        f"step_time={(end_time - start_time) * 1000.0:.3f} ms"
                    )
                else:
                    logger.info(
                        f"mode=walk(no-fsm) emg={int(emergency_stop)} deadman={int(snap.deadman)} "
                        f"cmd=({snap.cmd_x:.2f},{snap.cmd_y:.2f},{snap.cmd_yaw:.2f}) "
                        f"step_time={(end_time - start_time) * 1000.0:.3f} ms"
                    )

            # start_time = time.perf_counter()
            if telemetry is not None:
                telemetry.push_cmd(
                    stamp_ns=int(stamp_ns),
                    arm_position=arm_pos_des,
                    arm_stiffness=arm_stiffness,
                    arm_damping=arm_damping,
                    leg_position=leg_pos_des,
                    leg_stiffness=leg_stiffness,
                    leg_damping=leg_damping,
                    waist_position=waist_pos_des,
                    waist_stiffness=waist_stiffness,
                    waist_damping=waist_damping,
                    head_position=head_pos_des,
                    head_stiffness=head_stiffness,
                    head_damping=head_damping,
                )
            cmd.set_leg(position=leg_pos_des, stiffness=leg_stiffness, damping=leg_damping)
            cmd.set_arm(position=arm_pos_des, stiffness=arm_stiffness, damping=arm_damping)
            cmd.set_waist(position=waist_pos_des, stiffness=waist_stiffness, damping=waist_damping)
            cmd.set_head(position=head_pos_des, stiffness=head_stiffness, damping=head_damping)
            cmd.commit(stamp_ns=stamp_ns)
            # end_time = time.perf_counter()
            # if loop_idx % log_every == 0:
            #     logger.info(f"commit time: {(end_time - start_time) * 1000.0:.3f} ms")

            if args.enable_statistics and args.statistics_log_every_s > 0.0:
                now = time.monotonic()
                if now >= stats_next_log_t:
                    stats_next_log_t = now + float(args.statistics_log_every_s)
                    s = state.statistics()

                    def _metric_ms(metric: dict) -> str:
                        count = int(metric.get("count", 0))
                        if count <= 0:
                            return "n/a"
                        last_ms = float(metric.get("last_ns", 0)) / 1e6
                        ema_ms = float(metric.get("ema_ns", 0)) / 1e6
                        min_ms = float(metric.get("min_ns", 0)) / 1e6
                        max_ms = float(metric.get("max_ns", 0)) / 1e6
                        return f"ema {ema_ms:7.3f} (min {min_ms:7.3f} max {max_ms:7.3f} last {last_ms:7.3f}, n={count})"

                    def _hz_from_interval(interval_metric: dict) -> str:
                        ema_ns = float(interval_metric.get("ema_ns", 0))
                        if ema_ns <= 0:
                            return "n/a"
                        hz = 1e9 / ema_ns
                        return f"{hz:6.1f}"

                    arm_state = s.get("arm_state", {})
                    leg_state = s.get("leg_state", {})
                    waist_state = s.get("waist_state", {})
                    head_state = s.get("head_state", {})

                    imu_state = s.get("imu", {})

                    arm_delay = _metric_ms(arm_state.get("delay_ns", {}))
                    leg_delay = _metric_ms(leg_state.get("delay_ns", {}))
                    imu_delay = _metric_ms(imu_state.get("delay_ns", {}))
                    waist_delay = _metric_ms(waist_state.get("delay_ns", {}))
                    head_delay = _metric_ms(head_state.get("delay_ns", {}))

                    arm_jitter = _metric_ms(arm_state.get("interval_jitter_ns", {}))
                    leg_jitter = _metric_ms(leg_state.get("interval_jitter_ns", {}))
                    imu_jitter = _metric_ms(imu_state.get("interval_jitter_ns", {}))
                    waist_jitter = _metric_ms(waist_state.get("interval_jitter_ns", {}))
                    head_jitter = _metric_ms(head_state.get("interval_jitter_ns", {}))

                    arm_hz = _hz_from_interval(arm_state.get("interval_ns", {}))
                    leg_hz = _hz_from_interval(leg_state.get("interval_ns", {}))
                    imu_hz = _hz_from_interval(imu_state.get("interval_ns", {}))
                    waist_hz = _hz_from_interval(waist_state.get("interval_ns", {}))
                    head_hz = _hz_from_interval(head_state.get("interval_ns", {}))

                    pub_arm = _metric_ms(s.get("publish_arm", {}).get("duration_ns", {}))
                    pub_leg = _metric_ms(s.get("publish_leg", {}).get("duration_ns", {}))
                    pub_waist = _metric_ms(s.get("publish_waist", {}).get("duration_ns", {}))
                    pub_head = _metric_ms(s.get("publish_head", {}).get("duration_ns", {}))
                    commit_total = _metric_ms(s.get("commit_total", {}).get("duration_ns", {}))

                    sync = s.get("sync", {})
                    wait_frame = s.get("wait_frame", {})
                    uptime_s = float(s.get("uptime_ns", 0)) / 1e9
                    if stamp_ns and int(stamp_ns) > 0:
                        frame_age_ms = (time.time_ns() - int(stamp_ns)) / 1e6
                        frame_age_str = f"{frame_age_ms:7.3f}"
                    else:
                        frame_age_str = "n/a"
                    age_arm = _metric_ms(sync.get("age_arm_ns", {}))
                    age_leg = _metric_ms(sync.get("age_leg_ns", {}))
                    age_waist = _metric_ms(sync.get("age_waist_ns", {}))
                    age_head = _metric_ms(sync.get("age_head_ns", {}))
                    age_imu = _metric_ms(sync.get("age_imu_ns", {}))

                    logger.info(
                        f"statistics (uptime={uptime_s:.1f}s, sample_every={int(s.get('sample_every', 1))}, ema_shift={int(s.get('ema_shift', 4))}):\n"
                        f"  RX rate  (Hz):  arm={arm_hz}  leg={leg_hz}  imu={imu_hz}\n"
                        f"  RX delay (ms):  arm={arm_delay} \n"
                        f"                  leg={leg_delay} \n"
                        f"                  imu={imu_delay} \n"
                        f"                  waist={waist_delay} \n"
                        f"                  head={head_delay} \n"
                        f"  RX jitter(ms):  arm={arm_jitter} \n"
                        f"                  leg={leg_jitter} \n"
                        f"                  imu={imu_jitter} \n"
                        f"                  waist={waist_jitter} \n"
                        f"                  head={head_jitter} \n"
                        f"  TX cost  (ms):  commit={commit_total} \n"
                        f"                  pub_arm={pub_arm} \n"
                        f"                  pub_leg={pub_leg} \n"
                        f"                  pub_waist={pub_waist} \n"
                        f"                  pub_head={pub_head} \n"
                        f"  SYNC:\n"
                        f"    ticks         : {int(sync.get('tick_total', 0)):,}   (overrun: {int(sync.get('tick_overrun', 0)):,})\n"
                        f"    frame age (ms):         {frame_age_str}   (now - latest_frame.stamp_ns)\n"
                        f"    tick age  (ms): arm={age_arm} \n"
                        f"                    leg={age_leg} \n"
                        f"                    imu={age_imu} \n"
                        f"                    waist={age_waist} \n"
                        f"                    head={age_head} \n"
                        f"    frames:\n"
                        f"      written     : {int(sync.get('frame_written', 0)):,}\n"
                        f"      complete    : {int(sync.get('frame_complete', 0)):,}\n"
                        f"      incomplete  : {int(sync.get('frame_incomplete', 0)):,} (missing: arm {int(sync.get('frame_incomplete_missing_arm', 0)):,} | leg {int(sync.get('frame_incomplete_missing_leg', 0)):,} | imu {int(sync.get('frame_incomplete_missing_imu', 0)):,})\n"
                        f"      aligned     : {int(sync.get('frame_aligned', 0)):,}\n"
                        f"      unaligned   : {int(sync.get('frame_unaligned_skew', 0)):,} (skew)\n"
                        f"    wait_frame    : ok {int(wait_frame.get('ok', 0)):,} | timeout {int(wait_frame.get('timeout', 0)):,} | stopped {int(wait_frame.get('stopped', 0)):,}",
                    )

    except KeyboardInterrupt:
        logger.info("Keyboard interrupt")
    finally:
        if frame_capture is not None:
            frame_capture.close()
        if telemetry is not None:
            telemetry.close()
        teleop.close()
        aimrl_sdk.close(state)


if __name__ == "__main__":
    main()
