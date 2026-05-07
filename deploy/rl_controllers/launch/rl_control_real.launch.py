import os
import tempfile
import yaml
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.substitutions import (
    LaunchConfiguration,
    PathJoinSubstitution,
    PythonExpression,
)
from ament_index_python.packages import get_package_share_directory

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def launch_setup(context, *args, **kwargs):
    robot_descriptions_pkg_dir = get_package_share_directory('robot_descriptions_public')
    urdf_file_path = os.path.join(robot_descriptions_pkg_dir, 'urdf', 'x2_ultra.urdf')  # X2 Ultra URDF
    with open(urdf_file_path, 'r') as infile:
        robot_description_content = infile.read()
    robot_description = {"robot_description": robot_description_content}

    rl_pkg_dir = get_package_share_directory("rl_controllers")
    controllers_config_name = LaunchConfiguration("controllers_config").perform(context)
    if not controllers_config_name.endswith(".yaml"):
        controllers_config_name = controllers_config_name + ".yaml"
    controllers_config = os.path.join(rl_pkg_dir, "config", controllers_config_name)

    with open(controllers_config, "r") as infile:
        controllers_cfg = yaml.safe_load(infile)

    controller_params = controllers_cfg.setdefault("rl_controllers", {}).setdefault("ros__parameters", {})
    controller_manager_params = controllers_cfg.setdefault("controller_manager", {}).setdefault("ros__parameters", {})
    legged_cfg = controller_params.setdefault("LeggedRobotCfg", {})
    control_cfg = legged_cfg.setdefault("control", {})

    controller_manager_params["update_rate"] = int(LaunchConfiguration("controller_update_rate").perform(context))
    control_cfg["decimation"] = int(LaunchConfiguration("policy_decimation").perform(context))
    controller_params["start_mode"] = LaunchConfiguration("start_mode").perform(context)
    controller_params["startup_walk_command_x"] = float(LaunchConfiguration("startup_walk_command_x").perform(context))
    controller_params["startup_walk_command_y"] = float(LaunchConfiguration("startup_walk_command_y").perform(context))
    controller_params["startup_walk_command_yaw"] = float(LaunchConfiguration("startup_walk_command_yaw").perform(context))

    runtime_cfg = tempfile.NamedTemporaryFile(
        mode="w", delete=False, suffix=".yaml", prefix="rl_controllers_runtime_", dir="/tmp"
    )
    yaml.safe_dump(controllers_cfg, runtime_cfg, sort_keys=False)
    runtime_cfg.close()

    parameters_file = os.path.join(rl_pkg_dir, "config", "joy.yaml")

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            robot_description,
            runtime_cfg.name,
            {"aimrt_cfg_path": LaunchConfiguration('aimrt_cfg_path')},
            {"state_align_delay_ms": LaunchConfiguration('state_align_delay_ms')},
            {"state_buffer_capacity": LaunchConfiguration('state_buffer_capacity')},
        ],
        output="both",
    )

    robot_state_pub_node = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        output="both",
        parameters=[robot_description]
    )

    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "controller_manager"],
    )

    robot_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["rl_controllers", "--controller-manager", "controller_manager"],
    )

    # Delay start of robot_controller after `joint_state_broadcaster`
    delay_robot_controller_spawner_after_joint_state_broadcaster_spawner = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=joint_state_broadcaster_spawner,
            on_exit=[robot_controller_spawner],
        )
    )

    joy_node = Node(
        package='joy',
        executable='joy_node',
        name='joy_node',
        parameters=[{'autorepeat_rate': 20.0}],
    )

    teleop_node = Node(
        package='joy_teleop',
        executable='joy_teleop',
        parameters=[parameters_file]
    )

    return [
        control_node,
        robot_state_pub_node,
        joint_state_broadcaster_spawner,
        delay_robot_controller_spawner_after_joint_state_broadcaster_spawner,
        joy_node,
        teleop_node,
    ]


def generate_launch_description():
    parameters_file = PathJoinSubstitution(
        [
            FindPackageShare("rl_controllers"),
            "config",
            "joy.yaml",
        ]
    )

    return LaunchDescription([
        DeclareLaunchArgument('teleop_config', default_value=parameters_file),
        DeclareLaunchArgument('controllers_config', default_value='x2_baseline'),
        DeclareLaunchArgument('aimrt_cfg_path', default_value=os.environ.get('AIMRT_CFG_PATH')),
        DeclareLaunchArgument('start_mode', default_value='default_stand'),
        DeclareLaunchArgument('startup_walk_command_x', default_value='0.0'),
        DeclareLaunchArgument('startup_walk_command_y', default_value='0.0'),
        DeclareLaunchArgument('startup_walk_command_yaw', default_value='0.0'),
        DeclareLaunchArgument('controller_update_rate', default_value='500'),
        DeclareLaunchArgument('policy_decimation', default_value='10'),
        DeclareLaunchArgument('state_align_delay_ms', default_value='2.0'),
        DeclareLaunchArgument('state_buffer_capacity', default_value='512'),
        OpaqueFunction(function=launch_setup),
    ])
