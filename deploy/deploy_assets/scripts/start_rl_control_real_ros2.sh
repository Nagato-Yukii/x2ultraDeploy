#!/bin/bash

set -e -o pipefail

source /opt/ros/humble/setup.bash
#export ROS_DOMAIN_ID=232
export ROS_LOCALHOST_ONLY=0

#export FASTRTPS_DEFAULT_PROFILES_FILE=/agibot/software/v0/entry/bin/cfg/ros_dds_configuration.xml
#export FASTRTPS_DEFAULT_PROFILES_FILE=/home/suzumiyaharuhi/rl_deploy_aimdkv9/deploy/ros_dds_configuration.xml

SHELL_FOLDER=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
DEPLOY_OR_INSTALL_DIR=$(cd "$SHELL_FOLDER"/../../ && pwd)

if [ -f "$DEPLOY_OR_INSTALL_DIR/setup.bash" ]; then
    INSTALL_DIR="$DEPLOY_OR_INSTALL_DIR"
    DEPLOY_DIR=$(cd "$INSTALL_DIR"/.. && pwd)
else
    DEPLOY_DIR="$DEPLOY_OR_INSTALL_DIR"
    INSTALL_DIR="$DEPLOY_DIR/install"
fi

if [ "${USE_FASTRTPS_PROFILE:-0}" = "1" ] && [ -f "$DEPLOY_DIR/ros_dds_configuration.xml" ]; then
    export FASTRTPS_DEFAULT_PROFILES_FILE="$DEPLOY_DIR/ros_dds_configuration.xml"
else
    unset FASTRTPS_DEFAULT_PROFILES_FILE
fi

echo "$SHELL_FOLDER"
pushd "$INSTALL_DIR" || exit
    source setup.bash
    export LD_LIBRARY_PATH="$PWD/deploy_assets/thirdparty/onnxruntime-linux-x64-1.19.2/lib:$LD_LIBRARY_PATH"
    export PLUGIN_INSTALL_DIR="$PWD/legged_system/bin"
    export AIMRT_CFG_PATH="$PWD/legged_system/bin/ros2_chn_cfg.yaml"
    ros2 launch rl_controllers rl_control_real.launch.py "$@"
popd || exit
