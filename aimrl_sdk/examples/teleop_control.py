"""
=============================================================================
Purpose:
    人机交互输入复用层与运动状态机。将来自物理手柄（/dev/input/jsX Linux
    joystick 协议）或终端键盘（原始模式 TTY）的异构输入归一化为统一的
    InputSnapshot，供上层控制循环无感知地消费。MotionFSM 在此基础上实现
    四状态机（DEFAULT → LIE ↔ STAND → WALK），并在状态切换时执行 2 秒线
    性插值过渡，避免关节位置目标跳变产生大力矩冲击。

Dependencies:
    - aimrl_sdk:         读取当前关节位置（OBS.leg_pos）用于过渡插值起点；
    - rl_deploy_config:  AppCfg 提供默认关节角和 PD 增益；
    - termios / select:  非阻塞原始键盘输入；
    被调用方：rl_deploy_basic.py::main() 在每个控制 tick 调用 poll() 和 step()。

Architecture:
    输入优先级：手柄（连续轴）> 键盘（离散增量步进）。
    LinuxJoystick 解析内核 joystick 事件包（8 字节 struct IhBB），strip 掉
    初始化标志位（0x80）后分发给 axes/buttons 数组。
    RawKeyboard 解析 ANSI CSI/SS3 转义序列以识别方向键，同时处理 Ctrl-C/D。
    TeleopInput 在手柄断连时自动后台重试，断连瞬间强制释放 deadman。
    MotionFSM 所有状态切换均经过 500 ms 软件去抖，防止误触发。
=============================================================================
"""
from __future__ import annotations

import enum
import errno
import os
import select
import sys
import termios
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Protocol

import numpy as np
from loguru import logger

import aimrl_sdk
from rl_deploy_config import AppCfg


class Mode(enum.Enum):
    """运动状态机的状态枚举。

    状态转移图（单向约束）：
        DEFAULT ←→ LIE ←→ STAND → WALK → STAND（position_control_edge）
    """

    DEFAULT = "default"
    LIE = "lie"
    STAND = "stand"
    WALK = "walk"


@dataclass
class InputSnapshot:
    """单次 poll() 调用返回的输入快照，表示当前时刻的所有控制信号。

    连续量（cmd_x/y/yaw、deadman）反映当前持续状态；
    *_edge 字段为上升沿（按下瞬间）标志，仅在触发帧为 True，下一帧自动归 False。
    """

    cmd_x: float          # 前进/后退速度指令 [m/s]，deadman 未激活时为 0
    cmd_y: float          # 左右平移速度指令 [m/s]
    cmd_yaw: float        # 偏航角速度指令 [rad/s]
    deadman: bool         # 安全开关（手柄 Button 4 / 键盘空格）；False 时速度指令归零
    start_control_edge: bool      # 开启/关闭控制（Button 7 / 'p'）
    switch_mode_edge: bool        # LIE ↔ STAND 切换（deadman + Button 0 / 'm'）
    walk_mode_edge: bool          # STAND → WALK（Button 2 / 'k'）
    position_control_edge: bool   # WALK → STAND（Button 3 / 'l'）
    emergency_stop_edge: bool     # 紧急停止（Button 5+1 同时按住再按 6 / 'x'/ESC）
    quit_edge: bool               # 退出进程（Ctrl-C / Ctrl-D）


class LinuxJoystick:
    """直接读取 Linux /dev/input/jsX 设备文件的轻量手柄驱动。

    绕过 pygame/ROS joy node 等依赖，使用原始内核 joystick 事件协议。
    支持断连检测与上层主动重试。
    """

    # Linux joystick event format: u32 time, s16 value, u8 type, u8 number
    _EVENT_SIZE = 8
    _FMT = "IhBB"      # struct 格式：4B 时间戳, 2B 有符号值, 1B 事件类型, 1B 编号
    _TYPE_BUTTON = 0x01
    _TYPE_AXIS = 0x02
    _TYPE_INIT = 0x80  # 设备打开时内核发送的合成初始化事件，需屏蔽

    def __init__(self, path: Path):
        self.path = Path(path)
        self.fd: int | None = None
        self.axes = [0.0] * 8
        self.buttons = [0] * 11
        self._last_buttons = self.buttons.copy()

    def open(self) -> bool:
        """以非阻塞只读方式打开手柄设备文件。

        Returns:
            bool: True 表示成功打开；False 表示设备不存在或无权限。
        """
        try:
            self.fd = os.open(str(self.path), os.O_RDONLY | os.O_NONBLOCK)
            return True
        except OSError:
            self.fd = None
            return False

    def close(self) -> None:
        """关闭设备文件描述符并将所有状态归零。

        Side Effects: 修改 self.fd、self.axes、self.buttons。
        """
        if self.fd is not None:
            try:
                os.close(self.fd)
            finally:
                self.fd = None
        self.axes = [0.0] * len(self.axes)
        self.buttons = [0] * len(self.buttons)
        self._last_buttons = self.buttons.copy()

    def _mark_disconnected(self) -> None:
        self.close()

    def poll(self) -> bool:
        """排空内核事件队列，更新 axes/buttons 状态数组。

        非阻塞：若队列为空则立即返回。应在每个控制 tick 调用一次。

        Returns:
            bool: True 表示设备仍然连接；False 表示检测到断连（ENODEV/EIO）。

        Side Effects: 更新 self.axes 和 self.buttons。
        """
        if self.fd is None:
            return False

        import struct

        while True:
            r, _, _ = select.select([self.fd], [], [], 0.0)
            if not r:
                return True

            try:
                data = os.read(self.fd, self._EVENT_SIZE)
            except OSError as e:
                # Common when the receiver is unplugged while the program is running.
                if e.errno in (errno.ENODEV, errno.EIO, errno.EBADF):
                    self._mark_disconnected()
                    return False
                raise
            except BlockingIOError:
                return True
            if len(data) != self._EVENT_SIZE:
                # Unexpected short read; treat as "no more data" but keep the device.
                return True

            _, value, typ, number = struct.unpack(self._FMT, data)
            # 0x80 是内核在设备打开时注入的初始化标志，逻辑类型需屏蔽该位才能正确分类
            typ_no_init = typ & ~self._TYPE_INIT
            if typ_no_init == self._TYPE_AXIS:
                if 0 <= number < len(self.axes):
                    v = float(value) / 32767.0  # 16 位有符号整数 → [-1.0, 1.0]
                    self.axes[number] = float(np.clip(v, -1.0, 1.0))
            elif typ_no_init == self._TYPE_BUTTON:
                if 0 <= number < len(self.buttons):
                    self.buttons[number] = 1 if value else 0

    def consume_rising_edges(self) -> list[int]:
        """检测并消费自上次调用以来发生的按键上升沿（0→1 跳变）。

        Returns:
            list[int]: 本帧发生上升沿的按键编号列表；若无则返回空列表。

        Side Effects: 更新 self._last_buttons 为当前快照，每次调用只触发一次。
        """
        edges: list[int] = []
        for i, (prev, cur) in enumerate(zip(self._last_buttons, self.buttons, strict=True)):
            if prev == 0 and cur == 1:
                edges.append(i)
        self._last_buttons = self.buttons.copy()
        return edges


class RawKeyboard:
    """将 stdin 切换到 cbreak 模式以实现非阻塞单字符读取。

    避免使用 tty.setraw()：raw 模式会禁用输出后处理（OPOST/ONLCR），导致
    loguru 等库的换行输出出现对角偏移。此处使用 cbreak-like 模式：关闭
    规范输入和回显，但保留正常输出后处理，同时解析 ANSI 方向键转义序列。
    """

    def __init__(self):
        self._enabled = False
        self._stdin_fd: int | None = None
        self._old_termios: list[int] | None = None
        self._pending = bytearray()

    def open(self) -> bool:
        """将 stdin 设置为非规范、无回显的原始输入模式。

        Returns:
            bool: True 表示成功；False 表示 stdin 不是 TTY（如管道/重定向）。

        Side Effects: 修改终端 termios 属性，close() 时恢复。
        """
        if not sys.stdin.isatty():
            return False
        self._stdin_fd = sys.stdin.fileno()
        try:
            self._old_termios = termios.tcgetattr(self._stdin_fd)
            # Avoid `tty.setraw()`: it disables output post-processing (OPOST/ONLCR) for the whole TTY,
            # which makes '\n' not return the cursor to column 0 and causes "diagonal"/indented logs.
            #
            # Instead, use a cbreak-like mode: non-canonical input, no echo, but keep normal output.
            new_attr = termios.tcgetattr(self._stdin_fd)
            new_attr[3] &= ~(termios.ECHO | termios.ICANON)
            new_attr[1] |= termios.OPOST
            if hasattr(termios, "ONLCR"):
                new_attr[1] |= termios.ONLCR
            new_attr[6][termios.VMIN] = 0   # 最少读取 0 个字节（立即返回）
            new_attr[6][termios.VTIME] = 0  # 超时为 0（非阻塞）
            termios.tcsetattr(self._stdin_fd, termios.TCSADRAIN, new_attr)
            self._enabled = True
            return True
        except Exception:
            self.close()
            return False

    def close(self) -> None:
        """恢复终端原始 termios 属性。

        Side Effects: 恢复 stdin termios，清空 _pending 缓冲区。
        """
        if self._stdin_fd is not None and self._old_termios is not None:
            try:
                termios.tcsetattr(self._stdin_fd, termios.TCSADRAIN, self._old_termios)
            except Exception:
                pass
        self._enabled = False
        self._stdin_fd = None
        self._old_termios = None
        self._pending.clear()

    def _read_available(self) -> None:
        """将 stdin 中当前可读的所有字节追加到 _pending 缓冲区。"""
        if not self._enabled or self._stdin_fd is None:
            return
        while True:
            r, _, _ = select.select([self._stdin_fd], [], [], 0.0)
            if not r:
                return
            try:
                chunk = os.read(self._stdin_fd, 64)
            except BlockingIOError:
                return
            if not chunk:
                return
            self._pending.extend(chunk)

    def poll_keys(self) -> list[str]:
        """解析并消费 _pending 缓冲区中所有完整的按键序列。

        Returns:
            list[str]: 本次调用识别到的按键名称列表，如 ["UP", "w", "CTRL_C"]。
                       未识别的多字节序列被丢弃，不返回。

        Side Effects: 消费（修改/清空）self._pending 缓冲区。
        """
        self._read_available()
        out: list[str] = []

        def take(n: int) -> bytes | None:
            if len(self._pending) < n:
                return None
            b = bytes(self._pending[:n])
            del self._pending[:n]
            return b

        while self._pending:
            # Arrow keys can arrive as:
            # - CSI: ESC [ A/B/C/D
            # - SS3: ESC O A/B/C/D (some terminals / application cursor mode)
            # - CSI w/ modifiers: ESC [ 1 ; 2 A  (etc.)
            if self._pending[:1] == b"\x1b":
                if len(self._pending) < 2:
                    break

                second = self._pending[1]
                if second == ord("O"):
                    # SS3 序列：ESC O {A|B|C|D}，固定 3 字节
                    seq = take(3)
                    if seq is None:
                        break
                    last = seq[-1:]
                    if last == b"A":
                        out.append("UP")
                    elif last == b"B":
                        out.append("DOWN")
                    elif last == b"C":
                        out.append("RIGHT")
                    elif last == b"D":
                        out.append("LEFT")
                    else:
                        out.append("ESC")
                    continue

                if second == ord("["):
                    # CSI: consume until final byte (0x40..0x7E). Cap length to avoid getting stuck.
                    # ANSI 规范：CSI 序列以 0x40–0x7E 范围内的字节结束
                    max_seq_len = 16
                    end_idx: int | None = None
                    scan_limit = min(len(self._pending), max_seq_len)
                    for i in range(2, scan_limit):
                        if 0x40 <= self._pending[i] <= 0x7E:
                            end_idx = i
                            break
                    if end_idx is None:
                        if len(self._pending) < max_seq_len:
                            break
                        # Malformed/too-long CSI: treat as ESC and advance 1 byte.
                        take(1)
                        out.append("ESC")
                        continue

                    seq = take(end_idx + 1)
                    if seq is None:
                        break
                    last = seq[-1:]
                    if last == b"A":
                        out.append("UP")
                    elif last == b"B":
                        out.append("DOWN")
                    elif last == b"C":
                        out.append("RIGHT")
                    elif last == b"D":
                        out.append("LEFT")
                    else:
                        out.append("ESC")
                    continue

            b1 = take(1)
            if b1 is None:
                break
            if b1 == b"\x03":
                out.append("CTRL_C")
            elif b1 == b"\x04":
                out.append("CTRL_D")
            elif b1 == b"\x1b":
                out.append("ESC")
            else:
                try:
                    out.append(b1.decode("utf-8"))
                except UnicodeDecodeError:
                    continue
        return out


class TeleopInput:
    """手柄与键盘的统一输入复用器，向上层提供标准化的 InputSnapshot。

    优先级规则：手柄连接时连续轴（速度指令）由手柄独占，键盘仅控制边缘事件
    和离散状态切换；手柄断连时键盘接管所有控制。

    手柄布局（参考 deploy/rl_controllers/config/joy.yaml）：
        - Axis 1（左摇杆纵轴）→ cmd_x（负值对应"推前"，因此取反）
        - Axis 0（左摇杆横轴）→ cmd_y
        - Axis 3（右摇杆横轴）→ cmd_yaw（取反以匹配直觉方向）
        - Button 4             → deadman
        - Button 7             → start_control
        - Button 4 + Button 0  → switch_mode
        - Button 2             → walk_mode
        - Button 3             → position_control
        - Button 5 + 1 + 6     → emergency_stop（三键联锁防误触）
    """

    # Matches deploy/rl_controllers/config/joy.yaml:
    # - cmd_vel deadman: button 4
    # - start_control: button 7
    # - switch_mode: deadman(button 4) + button 0
    # - walk_mode: button 2
    # - position_control: button 3
    # - emergency_stop: requires buttons 5 and 1, then press button 6
    def __init__(self, joystick_path: Path, init_cmd: tuple[float, float, float]):
        self._js = LinuxJoystick(joystick_path)
        self._kb = RawKeyboard()

        self._kb_deadman = False
        self._cmd_x, self._cmd_y, self._cmd_yaw = init_cmd
        self._last_js_reconnect_t = 0.0
        self._js_reconnect_period_s = 1.0

        # These maxima match joy.yaml scales.
        self._max_x = 2.4
        self._max_y = 1.5
        self._max_yaw = 1.0
        self._step_x = 0.12      # 键盘每次按键增量（约 20 步到最大速度）
        self._step_y = 0.075
        self._step_yaw = 0.05

        self.joystick_ok = self._js.open()
        self.keyboard_ok = self._kb.open()



    def _maybe_reconnect_joystick(self) -> None:
        """以 1 Hz 的频率尝试重新打开手柄设备文件。"""
        if self.joystick_ok:
            return
        now = time.monotonic()
        if now - self._last_js_reconnect_t < self._js_reconnect_period_s:
            return
        self._last_js_reconnect_t = now
        if self._js.open():
            self.joystick_ok = True
            logger.warning(f"Joystick reconnected: {self._js.path}")

    def close(self) -> None:
        """关闭手柄和键盘设备，恢复终端设置。"""
        self._js.close()
        self._kb.close()

    def _apply_cmd_limits(self) -> None:
        """将速度指令裁剪到物理安全范围内。"""
        self._cmd_x = float(np.clip(self._cmd_x, -self._max_x, self._max_x))
        self._cmd_y = float(np.clip(self._cmd_y, -self._max_y, self._max_y))
        self._cmd_yaw = float(np.clip(self._cmd_yaw, -self._max_yaw, self._max_yaw))

    def poll(self) -> InputSnapshot:
        """读取当前帧的所有输入并生成 InputSnapshot。

        应在每个控制 tick 调用一次。内部会排空手柄事件队列和键盘缓冲区。
        手柄断连时立即释放 deadman 并将速度指令归零，防止机器人在无操作员
        监视的情况下继续行走。

        Returns:
            InputSnapshot: 当前帧的完整输入快照，*_edge 字段为一次性触发。

        Side Effects:
            - 更新 self.joystick_ok；
            - 若手柄连接，用手柄轴值覆盖键盘速度指令（优先级）。
        """
        if self.joystick_ok:
            ok = self._js.poll()
            if not ok:
                self.joystick_ok = False
                # Safety defaults: stop walking command and release deadman.
                self._kb_deadman = False
                self._cmd_x = 0.0
                self._cmd_y = 0.0
                self._cmd_yaw = 0.0
                logger.warning("Joystick disconnected; holding mode and zeroing cmd (deadman released)")
        else:
            self._maybe_reconnect_joystick()

        edges = set(self._js.consume_rising_edges()) if self.joystick_ok else set()

        start_control_edge = 7 in edges
        switch_mode_edge = (0 in edges) and (self._js.buttons[4] == 1)
        walk_mode_edge = 2 in edges
        position_control_edge = 3 in edges
        # 三键联锁：需要同时按住 Button 1 和 Button 5 再触发 Button 6，防止单键误触
        emergency_stop_edge = (6 in edges) and (self._js.buttons[1] == 1) and (self._js.buttons[5] == 1)
        quit_edge = False

        keys = self._kb.poll_keys()
        for k in keys:
            if k in ("CTRL_C", "CTRL_D"):
                quit_edge = True
            elif k in (" ",):
                self._kb_deadman = not self._kb_deadman
                self._cmd_x = 0.0
                self._cmd_y = 0.0
                self._cmd_yaw = 0.0
            elif k in ("p", "P", "\r"):
                start_control_edge = True
            elif k in ("m", "M"):
                switch_mode_edge = True
            elif k in ("k", "K"):
                walk_mode_edge = True
            elif k in ("l", "L"):
                position_control_edge = True
            elif k in ("x", "X", "ESC"):
                emergency_stop_edge = True
            elif k in ("0",):
                self._cmd_x = 0.0
                self._cmd_y = 0.0
                self._cmd_yaw = 0.0
            elif k in ("UP", "w", "W"):
                self._cmd_x += self._step_x
            elif k in ("DOWN", "s", "S"):
                self._cmd_x -= self._step_x
            elif k in ("LEFT", "a", "A"):
                self._cmd_yaw += self._step_yaw
            elif k in ("RIGHT", "d", "D"):
                self._cmd_yaw -= self._step_yaw
            elif k in ("q", "Q"):
                self._cmd_y -= self._step_y
            elif k in ("e", "E"):
                self._cmd_y += self._step_y

        # Joystick takes precedence for continuous commands if present.
        if self.joystick_ok:
            # Many gamepads report "push stick forward" as negative; map to +cmd_x (forward).
            self._cmd_x = -self._js.axes[1] * self._max_x
            self._cmd_y = self._js.axes[0] * self._max_y
            # Match intuitive turning direction (left stick -> +yaw or vice versa depending on hardware).
            self._cmd_yaw = -self._js.axes[3] * self._max_yaw

        self._apply_cmd_limits()

        deadman = (self._js.buttons[4] == 1) if self.joystick_ok else self._kb_deadman
        if not deadman:
            cmd_x, cmd_y, cmd_yaw = 0.0, 0.0, 0.0
        else:
            cmd_x, cmd_y, cmd_yaw = self._cmd_x, self._cmd_y, self._cmd_yaw

        return InputSnapshot(
            cmd_x=cmd_x,
            cmd_y=cmd_y,
            cmd_yaw=cmd_yaw,
            deadman=deadman,
            start_control_edge=start_control_edge,
            switch_mode_edge=switch_mode_edge,
            walk_mode_edge=walk_mode_edge,
            position_control_edge=position_control_edge,
            emergency_stop_edge=emergency_stop_edge,
            quit_edge=quit_edge,
        )


class PolicyRunner(Protocol):
    """策略执行器的结构子类型协议，供 MotionFSM.step() 进行鸭子类型检查。"""

    def step(self, obs: np.ndarray, cmd_x: float, cmd_y: float, cmd_yaw: float) -> np.ndarray: ...


class MotionFSM:
    """四状态运动状态机，管理从操作员输入到腿部控制目标的映射。

    状态机负责安全性：所有模式切换均有 500 ms 去抖，所有位置过渡均有 2 秒线
    性插值，防止关节目标突变。紧急停止状态下输出零刚度+纯阻尼（软着陆），
    不再跟踪任何目标位置。

    Args:
        app_cfg: 提供默认关节角、PD 增益等运动参数。
    """

    def __init__(self, app_cfg: AppCfg):
        self.app_cfg = app_cfg

        self.mode = Mode.DEFAULT
        self.start_control = False
        self.emergency_stop = False

        self._switch_time = time.monotonic()
        self._debounce_s = 0.5

        self._transition_t = 0.0
        self._transition_duration_s = 2.0
        self._from_leg = np.zeros(12, dtype=np.float32)

        self._lie_pose = np.array(app_cfg.leg_default_joint_angles, dtype=np.float32).reshape(12)
        self._stand_pose = np.array(app_cfg.leg_default_joint_angles, dtype=np.float32).reshape(12)

        # LIE 模式使用 50% 增益，降低躺下时的刚度以减小地面冲击力
        self._lie_stiffness = np.array(app_cfg.leg_stiffness, dtype=np.float32) * 0.5
        self._lie_damping = np.array(app_cfg.leg_damping, dtype=np.float32) * 0.5
        self._stand_stiffness = np.array(app_cfg.leg_stiffness, dtype=np.float32)
        self._stand_damping = np.array(app_cfg.leg_damping, dtype=np.float32)
        self._emg_damping = np.array(app_cfg.leg_damping, dtype=np.float32)

    def _debounced(self) -> bool:
        """检测状态切换请求是否通过去抖时间窗口。

        Returns:
            bool: True 表示距上次切换已超过去抖间隔，允许执行；
                  False 表示在抖动窗口内，应忽略本次请求。

        Side Effects: 若允许则更新 self._switch_time 为当前时刻。
        """
        now = time.monotonic()
        if now - self._switch_time < self._debounce_s:
            return False
        self._switch_time = now
        return True

    def _start_transition(self, from_leg: np.ndarray) -> None:
        """记录过渡插值的起始关节位置，重置过渡计时器。

        Args:
            from_leg: 当前实际腿部关节位置 shape=(12,)，作为插值起点。
        """
        self._transition_t = 0.0
        self._from_leg = from_leg.astype(np.float32, copy=True).reshape(12)

    def on_input(self, snap: InputSnapshot, obs: np.ndarray) -> None:
        """根据输入快照执行状态转移，不产生任何控制输出。

        应在每个控制 tick 的 step() 之前调用，以保证 step() 看到最新状态。

        Args:
            snap: 本帧输入快照（由 TeleopInput.poll() 生成）。
            obs:  当前观测向量，用于提取关节位置作为过渡插值起点。

        Side Effects:
            - 修改 self.mode、self.start_control、self.emergency_stop；
            - 可能调用 _start_transition() 修改 _from_leg 和 _transition_t。
        """
        leg_pos_cur = obs[aimrl_sdk.OBS.leg_pos].astype(np.float32, copy=False)

        if snap.emergency_stop_edge and self._debounced():
            self.emergency_stop = True
            logger.warning("Emergency stop set (press start_control to clear)")

        if snap.start_control_edge and self._debounced():
            if not self.start_control:
                self.start_control = True
                self._start_transition(leg_pos_cur)
                self.mode = Mode.LIE
                logger.info("Start control -> LIE")
            else:
                self.start_control = False
                self.mode = Mode.DEFAULT
                logger.info("Shutdown control -> DEFAULT")

        if self.emergency_stop and self.start_control:
            self.emergency_stop = False
            self.start_control = False
            self.mode = Mode.DEFAULT
            logger.warning("Emergency stop cleared -> DEFAULT")

        if snap.switch_mode_edge and self._debounced():
            if self.start_control:
                if self.mode == Mode.STAND:
                    self._start_transition(leg_pos_cur)
                    self.mode = Mode.LIE
                    logger.info("STAND -> LIE")
                elif self.mode == Mode.LIE:
                    self._start_transition(leg_pos_cur)
                    self.mode = Mode.STAND
                    logger.info("LIE -> STAND")

        if snap.walk_mode_edge and self._debounced():
            if self.mode == Mode.STAND:
                self.mode = Mode.WALK
                logger.info("STAND -> WALK")

        if snap.position_control_edge and self._debounced():
            if self.mode == Mode.WALK:
                self._start_transition(leg_pos_cur)
                self.mode = Mode.STAND
                logger.info("WALK -> STAND")
            elif self.mode == Mode.DEFAULT:
                self._start_transition(leg_pos_cur)
                self.mode = Mode.LIE
                logger.info("DEFAULT -> LIE")

    def step(
        self, obs: np.ndarray, policy: PolicyRunner, snap: InputSnapshot, dt: float
    ) -> tuple[np.ndarray, np.ndarray, np.ndarray]:
        """根据当前状态计算腿部 PD 目标，返回 (位置目标, 刚度, 阻尼) 三元组。

        Args:
            obs:    当前观测向量（包含 leg_pos 等字段）。
            policy: 实现 PolicyRunner 协议的推理对象，仅 WALK 模式下调用。
            snap:   当前输入快照，提供速度指令给策略网络。
            dt:     距上一帧的实际时间间隔 [s]，用于推进过渡插值计时器。

        Returns:
            tuple[np.ndarray, np.ndarray, np.ndarray]:
                - leg_pos_des: 目标关节位置 shape=(12,), float64
                - leg_stiffness: PD 刚度系数 shape=(12,), float64
                - leg_damping:   PD 阻尼系数 shape=(12,), float64

        Side Effects: 推进 self._transition_t（仅 LIE/STAND 模式）。
        """
        leg_pos_cur = obs[aimrl_sdk.OBS.leg_pos].astype(np.float32, copy=False)

        if self.emergency_stop:
            return (
                leg_pos_cur.astype(np.float64),
                np.zeros(12, dtype=np.float64),
                self._emg_damping.astype(np.float64),
            )

        if self.mode == Mode.DEFAULT:
            return leg_pos_cur.astype(np.float64), np.zeros(12, dtype=np.float64), np.full(12, 0.1, dtype=np.float64)

        if self.mode == Mode.WALK:
            leg_pos_des = policy.step(obs, snap.cmd_x, snap.cmd_y, snap.cmd_yaw).astype(np.float64, copy=False)
            return leg_pos_des, self._stand_stiffness.astype(np.float64), self._stand_damping.astype(np.float64)

        # LIE / STAND：线性插值过渡，防止关节目标跳变
        self._transition_t = min(self._transition_duration_s, self._transition_t + dt)
        # alpha ∈ [0, 1]：0 为过渡起点，1 为完全到达目标姿态
        alpha = self._transition_t / self._transition_duration_s if self._transition_duration_s > 0 else 1.0

        if self.mode == Mode.LIE:
            pose = (1.0 - alpha) * self._from_leg + alpha * self._lie_pose
            return pose.astype(np.float64), self._lie_stiffness.astype(np.float64), self._lie_damping.astype(np.float64)

        if self.mode == Mode.STAND:
            pose = (1.0 - alpha) * self._from_leg + alpha * self._stand_pose
            return pose.astype(np.float64), self._stand_stiffness.astype(np.float64), self._stand_damping.astype(np.float64)

        raise RuntimeError(f"unknown mode: {self.mode}")
