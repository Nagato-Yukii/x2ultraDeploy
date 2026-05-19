# RL_Deploy_aimdkv9
- [该仓库由此仓库适配灵犀X2机器人得到，原仓库链接由此去](https://github.com/Link-U-OS/rl_deploy.git)

- lx2501_3-v0.9.0.4来自灵犀官方文档
  
- 第一次开始
    - 编译lxsdkv9
        
        ```
        cd lx2501_3-v0.9.0.4
        colcon build
        ```
        
        ```
        运行日志:
        suzumiyaharuhi@suzumiyaharuhi-ASUS-TUF-Gaming-F16-FX607JV-FX607JV:~/rl_deploy_aimdkv9/lx2501_3-v0.9.0.4$ colcon build
        Starting >>> aimdk_msgs
        Starting >>> ruckig
        Finished <<< ruckig [0.26s]                                    
        Finished <<< aimdk_msgs [1.34s]                     
        Starting >>> examples
        Starting >>> py_examples
        Finished <<< examples [0.68s]                                                 
        --- stderr: py_examples
        /home/suzumiyaharuhi/.local/lib/python3.10/site-packages/setuptools/command/build_py.py:215: _Warning: Package 'py_examples.data' is absent from the `packages` configuration.
        !!
        
                ********************************************************************************
                ############################
                # Package would be ignored #
                ############################
                Python recognizes 'py_examples.data' as an importable package[^1],
                but it is absent from setuptools' `packages` configuration.
        
                This leads to an ambiguous overall configuration. If you want to distribute this
                package, please make sure that 'py_examples.data' is explicitly added
                to the `packages` configuration field.
        
                Alternatively, you can also rely on setuptools' discovery methods
                (for example by using `find_namespace_packages(...)`/`find_namespace:`
                instead of `find_packages(...)`/`find:`).
        
                You can read more about "package discovery" on setuptools documentation page:
        
                - https://setuptools.pypa.io/en/latest/userguide/package_discovery.html
        
                If you don't want 'py_examples.data' to be distributed and are
                already explicitly excluding 'py_examples.data' via
                `find_namespace_packages(...)/find_namespace` or `find_packages(...)/find`,
                you can try to use `exclude_package_data`, or `include-package-data=False` in
                combination with a more fine grained `package-data` configuration.
        
                You can read more about "package data files" on setuptools documentation page:
        
                - https://setuptools.pypa.io/en/latest/userguide/datafiles.html
        
                [^1]: For Python, any directory (with suitable naming) can be imported,
                      even if it does not contain any `.py` files.
                      On the other hand, currently there is no concept of package data
                      directory, all directories are treated like packages.
                ********************************************************************************
        
        !!
          check.warn(importable)
        ---
        Finished <<< py_examples [0.71s]
        
        Summary: 4 packages finished [2.17s]
          1 package had stderr output: py_examples
        解读：
        	py_examples.data下只有一个rgb_head_rear_mask.png暂时用不到，该警告被暂时搁置
        
        ```
        
    - 创建虚拟环境 venv
        
        ```
        cd aimrl_sdk
        source /home/suzumiyaharuhi/rl_deploy_aimdkv9/lx2501_3-v0.9.0.4/install/setup.bash
        uv sync
        source .venv/bin/activate
        
        ```
        
        ```
        运行日志:
        Resolved 23 packages in 1ms
              Built aimrl-sdk @ file:///home/suzumiyaharuhi/rl_deploy_aimdkv9/aimrl_sdk
        Prepared 1 package in 1m 37s
        Installed 17 packages in 32ms
         + aimrl-sdk==0.2.0 (from file:///home/suzumiyaharuhi/rl_deploy_aimdkv9/aimrl_sdk)
         + antlr4-python3-runtime==4.9.3
         + autopep8==2.3.2
         + cmake-format==0.6.13
         + cmakelang==0.6.13
         + flatbuffers==25.12.19
         + loguru==0.7.3
         + mpmath==1.3.0
         + numpy==2.4.4
         + omegaconf==2.3.0
         + onnxruntime==1.24.4
         + packaging==26.1
         + protobuf==7.34.1
         + pycodestyle==2.14.0
         + pyyaml==6.0.3
         + six==1.17.0
         + sympy==1.14.0
        ```
        
    - 编译仿真
        
        ```
        cd aimrl_sdk
        source .venv/bin/activate
        cd ../mujoco_sim
        ./build.sh
        ```
        
        ```
        问题日志
        情况
        SSL connect error / unexpected eof while reading
        解决
        代理软件切换为规则全局或换节点或关闭ssl校验
        ```
        
    - 启动仿真
        
        ```
        cd ../mujoco_sim_install/bin
        source /opt/ros/humble/setup.bash
        ./start_x2ultra.sh
        ```
        
        ![image.png](RL_Deploy_aimdkv9/image.png)
        
        ```
        通信测试
        source /home/suzumiyaharuhi/rl_deploy_aimdkv9/lx2501_3-v0.9.0.4/install/setup.bash
        ros2 topic echo /aima/hal/joint/head/state --once
        打印结果是
        A message was lost!!!
        	total count change:1
        	total count: 1---
        A message was lost!!!
        	total count change:1
        	total count: 2---
        A message was lost!!!
        	total count change:1
        	total count: 3---
        A message was lost!!!
        	total count change:1
        	total count: 4---
        A message was lost!!!
        	total count change:1
        	total count: 5---
        A message was lost!!!
        	total count change:1
        	total count: 6---
        A message was lost!!!
        	total count change:1
        	total count: 7---
        A message was lost!!!
        	total count change:1
        	total count: 8---
        A message was lost!!!
        	total count change:1
        	total count: 9---
        A message was lost!!!
        	total count change:1
        	total count: 10---
        header:
          stamp:
            sec: 1778148107
            nanosec: 377391121
          frame_id: x2_head
          sequence: 865248
          meas_stamp:
            sec: 0
            nanosec: 0
        state:
          value: 0
        joints:
        - name: head_yaw_joint
          position: 0.0004629580648514087
          velocity: -2.0745912991932963e-05
          effort: 0.0
          coil_temp: 40
          motor_temp: 40
          motor_vol: 24
        - name: head_pitch_joint
          position: -0.2989850234776714
          velocity: -1.8394082567103802e-07
          effort: 0.0
          coil_temp: 40
          motor_temp: 40
          motor_vol: 24
        ---
        ```
        
    - 编译运控
        
        另起终端
        
        ```
        cd deploy/
        source /home/suzumiyaharuhi/rl_deploy_aimdkv9/lx2501_3-v0.9.0.4/install/setup.bash
        colcon build
        ```
        
        运行日志
        
        ```
        ---
        Finished <<< legged_system [2min 2s]
                                  
        Summary: 5 packages finished [2min 2s]
          3 packages had stderr output: joint_msgs legged_system rl_controllers
        
        ```
        
    - 运行运控
        
        ```
        cd deploy/
        bash install/deploy_assets/scripts/start_rl_control_sim.sh
        ```
        
        ```
        日志
        [ros2_control_node-1] [INFO] [1778149210.467014374] [rclcpp]: 87
        [ros2_control_node-1] [INFO] [1778149210.467016293] [rclcpp]: 90
        [spawner-6] [INFO] [1778149210.470790596] [spawner_rl_controllers]: Configured and activated rl_controllers
        [INFO] [spawner-6]: process has finished cleanly [pid 284688]
        ```
        
    - 运行虚拟摇杆
        
        另起终端
        
        ```
        cd rl_deploy
        python3 install/deploy_assets/scripts/joy_interface.py
        ```
        
        ![image.png](RL_Deploy_aimdkv9/image%201.png)
        
        选中start/stop Control后，机器人变为默认姿态，Mode Switch转换到站立姿态后，点击EnterWalkMode，机器人站起来了
        
        ![image.png](RL_Deploy_aimdkv9/image%202.png)
        
        ![image.png](RL_Deploy_aimdkv9/image%203.png)
