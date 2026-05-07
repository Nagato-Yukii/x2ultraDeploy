# =============================================================================
# Purpose  : 将 C++ 帧向量（flat float32，kFrameDim=103 维）的内存布局
#            具体化为 Python 可直接索引的 slice 对象集合。
#            是训练代码与实时推理代码访问观测分量的唯一权威接口（Single Source
#            of Truth），避免各处硬编码魔法数字偏移量。
#
# Dependencies: _bindings 模块暴露的布局常量（ARM_POS0、IMU_QUAT0 等）；
#              这些常量来自 C++ layout.hpp 的 constexpr 枚举，通过 from_bindings()
#              同步到此处。被 examples/rl_deploy_basic.py、teleop_control.py 等
#              所有策略代码使用，以 OBS.leg_pos 等属性直接切片观测向量。
#
# Architecture: 冻结 dataclass（frozen=True），所有 @property 均为纯计算，无副作用。
#              模块末尾的 OBS 常量是进程级单例，由 _make_default_obs_slices()
#              在导入时从 _bindings 同步一次后固定，运行期不再变更。
# =============================================================================

from __future__ import annotations

from dataclasses import dataclass
from typing import Any


def _get_int_attr(mod: Any, name: str, default: int) -> int:
    v = getattr(mod, name, default)
    return int(v)

@dataclass(frozen=True)
class ObsSlices:
    """X2 Ultra 观测帧布局的不可变描述符。

    以起始偏移量（*0 字段）和自由度数（*_dof 字段）共同定义每个关节组
    在 103 维观测向量中的位置。@property 将这两个参数组合为 Python slice,
    供策略代码直接用于 numpy 数组切片(obs[OBS.leg_pos])。

    X2 Ultra 帧布局（顺序严格固定，与 C++ FrameLayout constexpr 一致）：
        [0  .. 13 ] arm_pos   (14 dof,左臂在前,右臂在后)
        [14 .. 27 ] arm_vel
        [28 .. 41 ] arm_eff   14 * 3 = 42
        [42 .. 53 ] leg_pos   (12 dof,左腿在前,右腿在后)
        [54 .. 65 ] leg_vel
        [66 .. 77 ] leg_eff   42 + 12 * 3 = 78
        [78 .. 80 ] waist_pos (3 dof: yaw/pitch/roll)
        [81 .. 83 ] waist_vel
        [84 .. 86 ] waist_eff
        [87 .. 88 ] head_pos  (2 dof: yaw/pitch)
        [89 .. 90 ] head_vel
        [91 .. 92 ] head_eff
        [93 .. 96 ] imu_quat  (xyzw 四元数)
        [97 .. 99 ] imu_gyro  (xyz 角速度)
        [100.. 102] imu_acc   (xyz 线加速度)
        总计 103 维 (kFrameDim)

    """
    dim: int
    arm_dof: int
    leg_dof: int
    waist_dof: int
    head_dof: int

    arm_pos0: int
    arm_vel0: int
    arm_eff0: int
    leg_pos0: int
    leg_vel0: int
    leg_eff0: int
    waist_pos0: int
    waist_vel0: int
    waist_eff0: int
    head_pos0: int
    head_vel0: int
    head_eff0: int
    imu_quat0: int
    imu_gyro0: int
    imu_acc0: int

    @classmethod
    def from_bindings(cls, bindings: Any) -> "ObsSlices":
        """从 _bindings 模块的 constexpr 导出常量构造布局描述符。

        C++ layout.hpp 是唯一事实来源；此方法在导入时将其同步到 Python 端，
        确保两侧布局始终一致，无需手动维护两份平行数字。若 _bindings 不含
        某个属性(例如旧版扩展),则回退到括号内的硬编码默认值(X2 Ultra 配置)。

        Args:
            bindings: _bindings 模块对象，需暴露 FRAME_DIM、ARM_POS0 等属性。

        Returns:
            填充完整的 ObsSlices 实例（尚未调用 _validate()，调用方负责验证）。
        """
        dim = _get_int_attr(bindings, "FRAME_DIM", 103)
        arm_dof = _get_int_attr(bindings, "ARM_DOF", 14)
        leg_dof = _get_int_attr(bindings, "LEG_DOF", 12)
        waist_dof = _get_int_attr(bindings, "WAIST_DOF", 3)
        head_dof = _get_int_attr(bindings, "HEAD_DOF", 2)
        return cls(
            dim=dim,
            arm_dof=arm_dof,
            leg_dof=leg_dof,
            waist_dof=waist_dof,
            head_dof=head_dof,
            arm_pos0=_get_int_attr(bindings, "ARM_POS0", 0),
            arm_vel0=_get_int_attr(bindings, "ARM_VEL0", 14),
            arm_eff0=_get_int_attr(bindings, "ARM_EFF0", 28),
            leg_pos0=_get_int_attr(bindings, "LEG_POS0", 42),
            leg_vel0=_get_int_attr(bindings, "LEG_VEL0", 54),
            leg_eff0=_get_int_attr(bindings, "LEG_EFF0", 66),
            waist_pos0=_get_int_attr(bindings, "WAIST_POS0", 78),
            waist_vel0=_get_int_attr(bindings, "WAIST_VEL0", 81),
            waist_eff0=_get_int_attr(bindings, "WAIST_EFF0", 84),
            head_pos0=_get_int_attr(bindings, "HEAD_POS0", 87),
            head_vel0=_get_int_attr(bindings, "HEAD_VEL0", 89),
            head_eff0=_get_int_attr(bindings, "HEAD_EFF0", 91),
            imu_quat0=_get_int_attr(bindings, "IMU_QUAT0", 93), # xyzw
            imu_gyro0=_get_int_attr(bindings, "IMU_GYRO0", 97),
            imu_acc0=_get_int_attr(bindings, "IMU_ACC0", 100),
        )

    def _validate(self) -> None:
        """校验所有分段的偏移量合法且不越界、不重叠。

        执行三项检查：
          1. 所有 DOF 和 dim 必须为正数；
          2. 每个分段 [start, end) 必须完全落在 [0, dim) 内；
          3. 任意两个分段不得有重叠(O(n²) 扫描，仅在初始化时调用一次，代价可接受)。
          4. arm_dof 和 leg_dof 必须为偶数(left/right 等分要求)。

        Raises:
            ValueError: 检查失败时，携带详细的分段名称和区间信息。
        """
        if self.dim <= 0:
            raise ValueError("obs dim must be > 0")
        if self.arm_dof <= 0 or self.leg_dof <= 0 or self.waist_dof <= 0 or self.head_dof <= 0:
            raise ValueError("arm/leg/waist/head dof must be > 0")

        segments: list[tuple[str, int, int]] = [
            ("arm_pos", self.arm_pos0, self.arm_pos0 + self.arm_dof),
            ("arm_vel", self.arm_vel0, self.arm_vel0 + self.arm_dof),
            ("arm_eff", self.arm_eff0, self.arm_eff0 + self.arm_dof),
            ("leg_pos", self.leg_pos0, self.leg_pos0 + self.leg_dof),
            ("leg_vel", self.leg_vel0, self.leg_vel0 + self.leg_dof),
            ("leg_eff", self.leg_eff0, self.leg_eff0 + self.leg_dof),
            ("waist_pos", self.waist_pos0, self.waist_pos0 + self.waist_dof),# *
            ("waist_vel", self.waist_vel0, self.waist_vel0 + self.waist_dof),# *
            ("waist_eff", self.waist_eff0, self.waist_eff0 + self.waist_dof),# *
            ("head_pos", self.head_pos0, self.head_pos0 + self.head_dof),# *
            ("head_vel", self.head_vel0, self.head_vel0 + self.head_dof),# *
            ("head_eff", self.head_eff0, self.head_eff0 + self.head_dof),# *
            ("imu_quat", self.imu_quat0, self.imu_quat0 + 4),
            ("imu_gyro", self.imu_gyro0, self.imu_gyro0 + 3),
            ("imu_acc", self.imu_acc0, self.imu_acc0 + 3),
        ]

        for name, start, end in segments:
            if start < 0 or end < 0 or end < start:
                raise ValueError(f"invalid {name} segment: [{start}, {end})")
            if end > self.dim:
                raise ValueError(f"{name} segment exceeds dim: end={end} dim={self.dim}")

        for i in range(len(segments)):
            ni, si, ei = segments[i]
            for j in range(i + 1, len(segments)):
                nj, sj, ej = segments[j]
                if max(si, sj) < min(ei, ej):
                    raise ValueError(f"overlapping segments: {ni} [{si},{ei}) and {nj} [{sj},{ej})")

        if self.arm_dof % 2 != 0:
            raise ValueError("arm_dof must be even for left/right splits")
        if self.leg_dof % 2 != 0:
            raise ValueError("leg_dof must be even for left/right splits")

    @property
    def arm_pos(self) -> slice:
        return slice(self.arm_pos0, self.arm_pos0 + self.arm_dof)
    
    @property
    def arm_vel(self) -> slice:
        return slice(self.arm_vel0, self.arm_vel0 + self.arm_dof)

    @property
    def arm_eff(self) -> slice:
        return slice(self.arm_eff0, self.arm_eff0 + self.arm_dof)

    @property
    def leg_pos(self) -> slice:
        return slice(self.leg_pos0, self.leg_pos0 + self.leg_dof)

    @property
    def leg_vel(self) -> slice:
        return slice(self.leg_vel0, self.leg_vel0 + self.leg_dof)

    @property
    def leg_eff(self) -> slice:
        return slice(self.leg_eff0, self.leg_eff0 + self.leg_dof)

    @property
    def waist_pos(self) -> slice:
        return slice(self.waist_pos0, self.waist_pos0 + self.waist_dof)# *

    @property
    def waist_vel(self) -> slice:
        return slice(self.waist_vel0, self.waist_vel0 + self.waist_dof)# *

    @property
    def waist_eff(self) -> slice:
        return slice(self.waist_eff0, self.waist_eff0 + self.waist_dof)# *

    @property
    def head_pos(self) -> slice:
        return slice(self.head_pos0, self.head_pos0 + self.head_dof)# *

    @property
    def head_vel(self) -> slice:
        return slice(self.head_vel0, self.head_vel0 + self.head_dof)# *

    @property
    def head_eff(self) -> slice:
        return slice(self.head_eff0, self.head_eff0 + self.head_dof)# * for X2.5 settings

    @property
    def imu_quat_xyzw(self) -> slice:
        return slice(self.imu_quat0, self.imu_quat0 + 4)
    
    # 新增,返回w,x,y,z,拆成返回w和返回x,y,z
    @property
    def imu_quat_w_only(self) -> slice:
        return slice(self.imu_quat0 + 3, self.imu_quat0 + 4)
    
    @property
    def imu_quat_xyz_only(self) -> slice:
        return slice(self.imu_quat0, self.imu_quat0 + 3)
    # 新增结束

    @property
    def imu_gyro_xyz(self) -> slice:
        return slice(self.imu_gyro0, self.imu_gyro0 + 3)

    @property
    def imu_acc_xyz(self) -> slice:
        return slice(self.imu_acc0, self.imu_acc0 + 3)

    @property
    def arm_left_pos(self) -> slice:
        half = self.arm_dof // 2
        return slice(self.arm_pos0, self.arm_pos0 + half)

    @property
    def arm_left_vel(self) -> slice:
        half = self.arm_dof // 2
        return slice(self.arm_vel0, self.arm_vel0 + half)

    @property
    def arm_left_eff(self) -> slice:
        half = self.arm_dof // 2
        return slice(self.arm_eff0, self.arm_eff0 + half)

    @property
    def arm_right_pos(self) -> slice:
        half = self.arm_dof // 2
        return slice(self.arm_pos0 + half, self.arm_pos0 + self.arm_dof)

    @property
    def arm_right_vel(self) -> slice:
        half = self.arm_dof // 2
        return slice(self.arm_vel0 + half, self.arm_vel0 + self.arm_dof)

    @property
    def arm_right_eff(self) -> slice:
        half = self.arm_dof // 2
        return slice(self.arm_eff0 + half, self.arm_eff0 + self.arm_dof)

    @property
    def leg_left_pos(self) -> slice:
        half = self.leg_dof // 2
        return slice(self.leg_pos0, self.leg_pos0 + half)

    @property
    def leg_left_vel(self) -> slice:
        half = self.leg_dof // 2
        return slice(self.leg_vel0, self.leg_vel0 + half)

    @property
    def leg_left_eff(self) -> slice:
        half = self.leg_dof // 2
        return slice(self.leg_eff0, self.leg_eff0 + half)

    @property
    def leg_right_pos(self) -> slice:
        half = self.leg_dof // 2
        return slice(self.leg_pos0 + half, self.leg_pos0 + self.leg_dof)

    @property
    def leg_right_vel(self) -> slice:
        half = self.leg_dof // 2
        return slice(self.leg_vel0 + half, self.leg_vel0 + self.leg_dof)

    @property
    def leg_right_eff(self) -> slice:
        half = self.leg_dof // 2
        return slice(self.leg_eff0 + half, self.leg_eff0 + self.leg_dof)
    
    # 提供交叉obs的获取方法
    def get_body_interleaved(self, obs: Any, body: str , attr: str) -> Any:
        """
        将原始观测
        [L1,L2,...Ln,R1,R2,...Rn],
        按照
        [L1,R1,L2,R2,...Ln,Rn]
        重排
        attr可选'pos','vel','eff'
        """
        raw = obs[getattr(self, f"{body}_{attr}")]

        if not hasattr(raw , "reshape"):
            raise TypeError(f"obs:{body}_{attr} cannot be reshaped。its true type: {type(raw)}")
        return raw.reshape(2, -1).T.reshape(-1)


    def split_for_walking(self, obs: Any) -> dict[str, Any]:
        """将一个完整观测向量拆分为按关节组命名的字典。

        调试和可视化时的便捷接口；热路径中应直接使用各 @property slice 以避免
        每次推理都构建临时字典。

        Args:
            obs: 支持 [] 切片操作的对象（numpy array、torch Tensor 等），
                 长度须 >= self.dim。

        Returns:
            包含 15 个键的字典（arm_pos/vel/eff, leg_pos/vel/eff,
            waist_pos/vel/eff, head_pos/vel/eff, imu_quat_xyzw/gyro_xyz/acc_xyz），
            每个值为对应分段的视图或副本（取决于 obs 的类型）。
        """
        return {
            "arm_pos": obs[self.arm_pos],
            "arm_vel": obs[self.arm_vel],
            #"arm_eff": obs[self.arm_eff],
            "leg_pos": obs[self.leg_pos],
            "leg_vel": obs[self.leg_vel],
            #"leg_eff": obs[self.leg_eff],
            "waist_pos": obs[self.waist_pos],
            "waist_vel": obs[self.waist_vel],
            #"waist_eff": obs[self.waist_eff],
            "head_pos": obs[self.head_pos],
            "head_vel": obs[self.head_vel],
            #"head_eff": obs[self.head_eff],
            "imu_quat_xyzw": obs[self.imu_quat_xyzw],
            "imu_gyro_xyz": obs[self.imu_gyro_xyz],
            "imu_acc_xyz": obs[self.imu_acc_xyz],
            "imu_quat_w_only": obs[self.imu_quat_w_only],
            "imu_quat_xyz_only": obs[self.imu_quat_xyz_only],
            "arm_pos_interleaved": self.get_body_interleaved(obs, "arm", "pos"),
            "arm_vel_interleaved": self.get_body_interleaved(obs, "arm", "vel"),
            "leg_pos_interleaved": self.get_body_interleaved(obs, "leg", "pos"),
            "leg_vel_interleaved": self.get_body_interleaved(obs, "leg", "vel"),
        }

def _make_default_obs_slices() -> ObsSlices:
    from . import _bindings

    obs = ObsSlices.from_bindings(_bindings)
    obs._validate()
    return obs

OBS = _make_default_obs_slices()