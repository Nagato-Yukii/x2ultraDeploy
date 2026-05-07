# AIMRL SDK
AIMRL C++ SDK Python bindings via pybind11.

# 在aimrl_sdk的venv下配置环境

cd aimrl_sdk

source .venv/bin/deactivate

cd ../mujoco_sim

对mujoco_sim额外补充依赖

uv pip install "scikit-build-core>=0.9.0" "pybind11>=2.11.0" "empy>=3.3.4,<4.0" "catkin_pkg>=0.5.2" "lark>=1.1.5" "numpy>=1.21"

./format.sh    统一配置代码风格

./build.sh    编译mujoco_sim