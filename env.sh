#!/bin/bash
# 严格按照从底层到顶层的顺序 Source
source /opt/ros/humble/setup.bash
source ~/x2ultraDeploy/lx2501_3-v0.9.0.4/install/setup.bash
source ~/x2ultraDeploy/deploy/install/setup.bash

# 激活 Python RL 运行环境
source ~/x2ultraDeploy/aimrl_sdk/.venv/bin/activate

echo "Deployed Workspace Activated."
