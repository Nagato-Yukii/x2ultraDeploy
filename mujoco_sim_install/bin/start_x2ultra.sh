#!/bin/bash

cd $(dirname $0)

set -e

export SIM_RESOURCE_MODEL_PATH=$(pwd)/../resource/model

start_iox_roudi() {
  if pgrep -f "./iox-roudi" > /dev/null
  then
      echo "iox-roudi is running"
  else
      echo "iox-roudi is not running, starting..."
      ./iox-roudi &
      echo "iox-roudi started"
  fi
}

start_iox_roudi

source /opt/ros/humble/setup.bash
source /home/suzumiyaharuhi/rl_deploy/lxx2ultra_aimdk/install/setup.bash
source ../share/ros2_plugin_proto/local_setup.bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd):$(pwd)/../lib

./aimrt_main --cfg_file_path=./cfg/x2ultra_cfg.yaml
