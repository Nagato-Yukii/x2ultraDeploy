# =============================================================================
# Purpose  : aimrl_sdk 的 Python 公共入口。负责三项运行时引导工作：
#            1) 将 C++ 插件目录注入 LD_LIBRARY_PATH，使 AIMRT 能找到自己的
#               共享库插件（iceoryx/ROS2 后端 .so）；
#            2) 以 RTLD_GLOBAL 方式预加载 ROS2 typesupport 动态库，规避
#               pybind11 扩展以 RTLD_LOCAL 加载时 ROS2 类型注册表失效的问题；
#            3) 将用户传入的后端名称（"iceoryx"/"ros2"）映射到内置 YAML 配置
#               文件路径，屏蔽 C++ 层路径细节。
#
# Dependencies: _bindings（pybind11 C++ 扩展）、obs.py（观测布局）。
#              被下游 RL 策略脚本通过 `import aimrl_sdk` 调用；
#              被 examples/rl_deploy_basic.py 等演示脚本使用。
#
# Architecture: 纯胶水层，无独立状态。所有 SDK 状态由 C++ Core 实例持有。
#              此文件唯一的职责是把正确的配置路径解析出来，
#              然后转发给 _bindings.open()，返回 (StateInterface, CommandInterface)。
# =============================================================================
import os
import sys
from pathlib import Path
from typing import Any, Tuple

if sys.platform != "linux":
    raise RuntimeError(f"aimrl_sdk only supports Linux (sys.platform={sys.platform!r}).")

from . import _bindings as _bindings
from .obs import OBS as OBS
from .obs import ObsSlices as ObsSlices

def _ensure_runtime_env() -> None:
    """将 C++ 扩展所在目录追加到 LD_LIBRARY_PATH,并触发 typesupport 预加载。

    必须在模块导入时同步调用（非懒加载），因为 AIMRT 插件在首次 open() 之前
    就需要 LD_LIBRARY_PATH 中存在正确路径。AIMRT_PLUGIN_DIR 环境变量只在
    未被用户预先设置时写入，避免覆盖外部配置。

    Side Effects:
        修改 os.environ["LD_LIBRARY_PATH"] 和 os.environ["AIMRT_PLUGIN_DIR"]。
    """
    plugin_dir = getattr(_bindings, "__file__", "")
    if plugin_dir:
        plugin_dir = os.path.dirname(plugin_dir)
    if not plugin_dir:
        return

    os.environ.setdefault("AIMRT_PLUGIN_DIR", plugin_dir)

    ld_path = os.environ.get("LD_LIBRARY_PATH", "")
    parts = [p for p in ld_path.split(":") if p] if ld_path else []
    if plugin_dir not in parts:
        os.environ["LD_LIBRARY_PATH"] = ":".join([plugin_dir] + parts)

    _preload_typesupport_libraries(plugin_dir)

def _preload_typesupport_libraries(plugin_dir: str) -> None:
    """强制以 RTLD_GLOBAL 加载 ROS2 typesupport 动态库。

    pybind11 默认以 RTLD_LOCAL 加载扩展模块，导致扩展内的 ROS2 类型注册表
    对后续动态加载的 AIMRT 插件不可见，从而引发 rcutils 类型查找失败。
    此函数提前以 RTLD_GLOBAL 加载所有 typesupport .so,使其符号对全进程可见。
    加载失败时静默跳过（插件可能不存在于当前后端配置中）。

    Args:
        plugin_dir: _bindings.so 所在目录,typesupport .so 也应位于此处。

    Side Effects:
        向进程全局符号表注入多个共享库。
    """
    try:
        import ctypes
        import glob
    except Exception:
        return

    patterns = [
        "libros2_plugin_proto__rosidl_typesupport_*.so",
        "libaimdk_msgs__rosidl_typesupport_*.so", # for x2
    ]
    for pattern in patterns:
        for path in glob.glob(os.path.join(plugin_dir, pattern)):
            try:
                ctypes.CDLL(path, mode=ctypes.RTLD_GLOBAL)
            except OSError:
                pass

_ensure_runtime_env()

_open_native = _bindings.open
close = _bindings.close
StateInterface = _bindings.StateInterface
CommandInterface = _bindings.CommandInterface


_AIMRT_BUILTIN_CONFIGS = {
    "iceoryx": "aimrt_iceoryx_backend.yaml",
    "ros2": "aimrt_ros2_backend.yaml",
}

def aimrt_config_path(backend: str) -> str:
    """将后端名称解析为对应的内置 AIMRT 配置文件的绝对路径。

    优先从 _bindings.so 旁边的 config/ 目录查找（wheel 安装场景），
    回退到当前 Python 包的 config/ 目录（开发/editable 安装场景）。

    Args:
        backend: 后端名称，大小写不敏感，目前支持 "iceoryx" 和 "ros2"。

    Returns:
        内置 YAML 配置文件的绝对路径字符串；若文件不存在仍返回预期路径
        （由 C++ 端在 open 时报错）。

    Raises:
        ValueError: 当 backend 不在已知键集合中时。
    """
    key = str(backend).strip().lower()
    if key not in _AIMRT_BUILTIN_CONFIGS:
        supported = ", ".join(sorted(_AIMRT_BUILTIN_CONFIGS.keys()))
        raise ValueError(f"unsupported aimrt backend: {backend!r} (supported: {supported})")

    filename = _AIMRT_BUILTIN_CONFIGS[key]
    plugin_dir = os.path.dirname(getattr(_bindings, "__file__", "") or "")
    if plugin_dir:
        candidate = os.path.join(plugin_dir, "config", filename)
        if os.path.exists(candidate):
            return candidate

    return os.path.join(os.path.dirname(__file__), "config", filename)

def open(
    *args: Any,
    aimrt_backend: str = "iceoryx",
    config_path: str | os.PathLike | None = None,
    **kwargs: Any,
) -> Tuple[StateInterface, CommandInterface]:
    """启动 AIMRT 传输层并返回 (StateInterface, CommandInterface) 对。

    这是用户代码的主入口。三种调用约定：
      - open()                        → 使用内置 iceoryx 后端配置
      - open(aimrt_backend="ros2")    → 使用内置 ROS2 后端配置
      - open(config_path="/path/to/cfg.yaml") → 使用用户指定配置

    Args:
        *args: 若提供，第一个位置参数作为 config_path 直传 C++ 层（向后兼容）。
        aimrt_backend: 当未指定 config_path 时使用的内置后端名称，
                       可选 "iceoryx"（默认）或 "ros2"。
        config_path: 自定义 AIMRT YAML 配置文件路径，指定时忽略 aimrt_backend。
        **kwargs: 透传给 _bindings.open() 的所有关键字参数
                  (sync_hz、max_skew_ms、align_delay_ms 等)。
                  sync_delay_ms 为已废弃的别名，等价于 align_delay_ms。

    Returns:
        (StateInterface, CommandInterface) 元组，两者共享同一个底层 Core 实例。

    Raises:
        TypeError: config_path 同时以位置参数和关键字参数指定时；
                   或 sync_delay_ms 与 align_delay_ms 同时出现时。
        ValueError: aimrt_backend 不受支持时（由 aimrt_config_path 抛出）。
        RuntimeError: C++ 层初始化失败（传输层无法连接等）。
    """
    if args and (config_path is not None):
        raise TypeError("config_path specified both positionally and by keyword")

    if "sync_delay_ms" in kwargs:
        if "align_delay_ms" in kwargs:
            raise TypeError("both sync_delay_ms and align_delay_ms specified")
        kwargs["align_delay_ms"] = kwargs.pop("sync_delay_ms")

    if args:
        return _open_native(*args, **kwargs)

    if config_path is not None:
        cfg = str(Path(config_path))
        return _open_native(cfg, **kwargs)

    return _open_native(aimrt_config_path(aimrt_backend), **kwargs)

__all__ = [
    "open",
    "close",
    "StateInterface",
    "CommandInterface",
    "OBS",
    "ObsSlices",
    "aimrt_config_path",
]


