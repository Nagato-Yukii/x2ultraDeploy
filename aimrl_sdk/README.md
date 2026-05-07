# AIMRL SDK
AIMRL C++ SDK Python bindings via pybind11.

# 我们使用的python>=3.11


# 校准虚拟环境
source ~/rl_deploy_v2/lxx2ultra_aimdk/install/setup.bash

uv sync

uv sync -v 或 uv synd -vv，但是基本用不到

显式启动虚拟环境:source .venv/bin/activate

如果是快速调试:uv sync 2>&1 | grep --color=always -i -E "error:|failed:|fatal:|exception"

重新运行脚本来更新_bindings.pyi:
修改C++源代码并重新编译后,需要重新运行脚本来生成新_bindings.pyi
source ~/rl_deploy_v2/lxx2ultra_aimdk/install/setup.bash
python3 aimrl_sdk/tools/generate_pyi.py --write-to-src

# 调试

Q1:No module named pybind11_stubgen

A1:.venv中缺少了pybind11_stubgen,因为它在pyproject.toml中位于[project.optional-dependencies]/stubs下，uv sync默认不安装,需要运行uv sync --extra stubs，uv对环境百分百要求相同,所以再运行uv sync,pybind11_stubgen还会被再次删除