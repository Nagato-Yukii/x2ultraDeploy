#include <rl_controllers/RLControllerBase.h>
#include <rl_controllers/RotationTools.h>
#include <rl_controllers/Utilities.h>
#include <algorithm>
#include <cctype>
#include <cmath>

namespace {
constexpr double kDeltaPosThreshold = 0.05;
constexpr double kStandDirectDeadband = 0.03;
constexpr double kMaxCmdVx = 0.5;
constexpr double kMaxCmdVy = 1.0;
constexpr double kMaxCmdYaw = 0.5;

double getGainOrDefault(const std::map<std::string, float> &gains, const std::string &jointName) {
  const auto it = gains.find(jointName);
  return it == gains.end() ? 0.0 : static_cast<double>(it->second);
}
/*
std::string toLowerCopy(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(),
                 [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
  return value;
}*/
}  // namespace

namespace legged {

using config_type = controller_interface::interface_configuration_type;

RLControllerBase::RLControllerBase() : controller_interface::ControllerInterface() {}

// First
controller_interface::CallbackReturn RLControllerBase::on_init() {
  // X2 Ultra joint names — must match baseline.yaml `joints:` order
  jointNames = {
    // leg (0..11) — policy-controlled
    "left_hip_pitch_joint", "left_hip_roll_joint", "left_hip_yaw_joint",
    "left_knee_joint", "left_ankle_pitch_joint", "left_ankle_roll_joint",
    "right_hip_pitch_joint", "right_hip_roll_joint", "right_hip_yaw_joint",
    "right_knee_joint", "right_ankle_pitch_joint", "right_ankle_roll_joint",
    // waist (12..14)
    "waist_yaw_joint", "waist_pitch_joint", "waist_roll_joint",
    // arm (15..28)
    "left_shoulder_pitch_joint", "left_shoulder_roll_joint", "left_shoulder_yaw_joint",
    "left_elbow_joint", "left_wrist_yaw_joint", "left_wrist_pitch_joint", "left_wrist_roll_joint",
    "right_shoulder_pitch_joint", "right_shoulder_roll_joint", "right_shoulder_yaw_joint",
    "right_elbow_joint", "right_wrist_yaw_joint", "right_wrist_pitch_joint", "right_wrist_roll_joint",
    // head (29..30)
    "head_yaw_joint", "head_pitch_joint"};

  // Store variables from the parameter server into joint_names_, command_interface_types_, and state_interface_types_
  joint_names_ = auto_declare<std::vector<std::string>>("joints", joint_names_);
  command_interface_types_ =
      auto_declare<std::vector<std::string>>("command_interfaces", command_interface_types_);
  state_interface_types_ =
      auto_declare<std::vector<std::string>>("state_interfaces", state_interface_types_);
  //start_mode_name_ = auto_declare<std::string>("start_mode", "default_stand");
  //startup_walk_command_x_ = auto_declare<double>("startup_walk_command_x", 0.0);
  //startup_walk_command_y_ = auto_declare<double>("startup_walk_command_y", 0.0);
  //startup_walk_command_yaw_ = auto_declare<double>("startup_walk_command_yaw", 0.0);
  //configureStartupMode();
  actuatedDofNum_ = joint_names_.size();

  tfBroadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(get_node());

  // Load policy model and rl cfg
  if (!loadModel()) {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"), "[RLControllerBase] Failed to load the model. Ensure the path is correct and accessible.");
    return CallbackReturn::FAILURE;
  }
  if (!loadRLCfg()) {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"), "[RLControllerBase] Failed to load the rl config. Ensure the yaml is correct and accessible.");
    return CallbackReturn::FAILURE;
  }

  /*这里去掉了关于导入mocap data的语句*/

  switchTime = rclcpp::Clock(RCL_ROS_TIME).now();
  standJointAngles_.resize(actuatedDofNum_);
  lieJointAngles_.resize(actuatedDofNum_);
  allJointPos_.resize(actuatedDofNum_);
  standHoldRefAngles_.resize(actuatedDofNum_);

  auto &StandState = standJointState;
  auto &LieState = lieJointState;

  // Stand pose (action=0, default joint angles from sim2sim_details.txt， not zero), same for lie
  lieJointAngles_ <<
    LieState.left_hip_pitch_joint,  LieState.left_hip_roll_joint,  LieState.left_hip_yaw_joint,
    LieState.left_knee_joint,       LieState.left_ankle_pitch_joint, LieState.left_ankle_roll_joint,
    LieState.right_hip_pitch_joint, LieState.right_hip_roll_joint, LieState.right_hip_yaw_joint,
    LieState.right_knee_joint,      LieState.right_ankle_pitch_joint, LieState.right_ankle_roll_joint,
    LieState.waist_yaw_joint,       LieState.waist_pitch_joint,    LieState.waist_roll_joint,
    LieState.left_shoulder_pitch_joint, LieState.left_shoulder_roll_joint, LieState.left_shoulder_yaw_joint,
    LieState.left_elbow_joint, LieState.left_wrist_yaw_joint, LieState.left_wrist_pitch_joint, LieState.left_wrist_roll_joint,
    LieState.right_shoulder_pitch_joint, LieState.right_shoulder_roll_joint, LieState.right_shoulder_yaw_joint,
    LieState.right_elbow_joint, LieState.right_wrist_yaw_joint, LieState.right_wrist_pitch_joint, LieState.right_wrist_roll_joint,
    LieState.head_yaw_joint, LieState.head_pitch_joint;

  standJointAngles_ <<
    StandState.left_hip_pitch_joint,  StandState.left_hip_roll_joint,  StandState.left_hip_yaw_joint,
    StandState.left_knee_joint,       StandState.left_ankle_pitch_joint, StandState.left_ankle_roll_joint,
    StandState.right_hip_pitch_joint, StandState.right_hip_roll_joint, StandState.right_hip_yaw_joint,
    StandState.right_knee_joint,      StandState.right_ankle_pitch_joint, StandState.right_ankle_roll_joint,
    StandState.waist_yaw_joint,       StandState.waist_pitch_joint,    StandState.waist_roll_joint,
    StandState.left_shoulder_pitch_joint, StandState.left_shoulder_roll_joint, StandState.left_shoulder_yaw_joint,
    StandState.left_elbow_joint, StandState.left_wrist_yaw_joint, StandState.left_wrist_pitch_joint, StandState.left_wrist_roll_joint,
    StandState.right_shoulder_pitch_joint, StandState.right_shoulder_roll_joint, StandState.right_shoulder_yaw_joint,
    StandState.right_elbow_joint, StandState.right_wrist_yaw_joint, StandState.right_wrist_pitch_joint, StandState.right_wrist_roll_joint,
    StandState.head_yaw_joint, StandState.head_pitch_joint;

  realJointPosPublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/real_joint_pos", 1); //
  realJointVelPublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/real_joint_vel", 1);
  realJointTorquePublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/real_joint_torque", 1);

  rlPlannedJointPosPublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/rl_planned_joint_pos", 1);
  rlPlannedJointVelPublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/rl_planned_joint_vel", 1);
  rlPlannedTorquePublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/rl_planned_joint_torque", 1);

  realImuAngularVelPublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/imu_angular_vel", 1);
  realImuLinearAccPublisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/imu_linear_acc", 1);
  realImuEulerXyzPulbisher_ = get_node()->create_publisher<std_msgs::msg::Float64MultiArray>("data_analysis/imu_euler_xyz", 1);
  simResetPublisher_ = get_node()->create_publisher<std_msgs::msg::Empty>("/aima/sim/reset", 1);

  cmdVelSub_ = get_node()->create_subscription<geometry_msgs::msg::Twist>(
      "/cmd_vel", 1, std::bind(&RLControllerBase::cmdVelCallback, this, std::placeholders::_1));
  joyInfoSub_ = get_node()->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 1, std::bind(&RLControllerBase::joyInfoCallback, this, std::placeholders::_1));

  auto emergencyStopCallback = [this](const std_msgs::msg::Float32::SharedPtr /*msg*/) {
    emergency_stop = true;
    RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Emergency Stop");
  };
  emgStopSub_ = get_node()->create_subscription<std_msgs::msg::Float32>(
      "/emergency_stop", 1, emergencyStopCallback);

  // start control
  // DEFAULT --> LIE
  // ANY_MODE --> DEFAULT
  auto startControlCallback = [this](const std_msgs::msg::Float32::SharedPtr /*msg*/) {
    if (start_control_latched.exchange(true)) {
      return;
    }

    // RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Current Time s: " << rclcpp::Clock(RCL_ROS_TIME).now().seconds());
    // RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Switch Time s: " << switchTime.seconds());
    // RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Current Time nano: " << rclcpp::Clock(RCL_ROS_TIME).now());
    // RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Switch Time nano: " << switchTime);

    rclcpp::Duration t(0, 500000000);
    auto currentTime = rclcpp::Clock(RCL_ROS_TIME).now();
    if (currentTime - switchTime > t) {
      if (!start_control) { // 若当前未启动
        start_control = true; // 则启动机器人,DEFAULT->LIE,需要插值
        standPercent_ = 0;
        for (size_t i = 0; i < hybridJointHandles_.size(); i++) {
          currentJointAngles_[i] = hybridJointHandles_[i]->getPosCurr();
        }
        mode_ = Mode::LIE;
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Start Control");
      } else {
        start_control = false; // 如果已经启动,则直接脱力, mode_ = Mode::DEFAULT
        mode_ = Mode::DEFAULT;
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "ShutDown Control, mode_ = Mode::DEFAULT");
      }
      switchTime = rclcpp::Clock(RCL_ROS_TIME).now();
    }
  };
  startCtrlSub_ = get_node()->create_subscription<std_msgs::msg::Float32>(
      "/start_control", 1, startControlCallback);

  // switch mode
  // LIE <--> STAND
  auto switchModeCallback = [this](const std_msgs::msg::Float32::SharedPtr /*msg*/) {
    if (switch_mode_latched.exchange(true)) {
      return;
    }

    rclcpp::Duration t(0, 500000000);
    auto currentTime = rclcpp::Clock(RCL_ROS_TIME).now();
    if (currentTime - switchTime > t) {
      if (start_control == true) {
        if (mode_ == Mode::STAND) {
          standPercent_ = 0;
          for (size_t i = 0; i < hybridJointHandles_.size(); i++) {
            currentJointAngles_[i] = hybridJointHandles_[i]->getPosCurr();
          }
          mode_ = Mode::LIE;
          RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "STAND2LIE");
        } else if (mode_ == Mode::LIE) {
          standPercent_ = 0;
          mode_ = Mode::STAND;
          RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "LIE2STAND");
        }
      }
      switchTime = rclcpp::Clock(RCL_ROS_TIME).now();
    }
  };
  switchModeSub_ = get_node()->create_subscription<std_msgs::msg::Float32>(
      "/switch_mode", 1, switchModeCallback);

  // walkMode
  // STAND --> WALK
  auto walkModeCallback = [this](const std_msgs::msg::Float32::SharedPtr /*msg*/) {
    if (walk_mode_latched.exchange(true)) {
      return;
    }

    rclcpp::Duration t(0, 500000000);
    auto currentTime = rclcpp::Clock(RCL_ROS_TIME).now();
    if (currentTime - switchTime > t) {
      if (mode_ == Mode::STAND) {
        simResetPublisher_->publish(std_msgs::msg::Empty{});
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "Publish /aima/sim/reset before STAND2WALK");
        loopCount_ = 0;
        standPercent_ = 0;
        onEnterWalk();
        mode_ = Mode::WALK;
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "STAND2WALK");
      }
      switchTime = rclcpp::Clock(RCL_ROS_TIME).now();
    }
  };
  walkModeSub_ = get_node()->create_subscription<std_msgs::msg::Float32>("/walk_mode", 1, walkModeCallback);

  // positionMode
  // WALK --> STAND or DEFAULT --> LIE
  auto positionModeCallback = [this](const std_msgs::msg::Float32::SharedPtr /*msg*/) {
    if (position_control_latched.exchange(true)) {
      return;
    }

    rclcpp::Duration t(0, 500000000);
    auto currentTime = rclcpp::Clock(RCL_ROS_TIME).now();
    if (currentTime - switchTime > t) {
      if (mode_ == Mode::WALK) {
        mode_ = Mode::STAND;
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "WALK2STAND");
      } else if (mode_ == Mode::DEFAULT) {
        standPercent_ = 0;
        for (size_t i = 0; i < hybridJointHandles_.size(); i++) {
          currentJointAngles_[i] = hybridJointHandles_[i]->getPosCurr();
        }
        mode_ = Mode::LIE;
        RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "DEF2LIE");
      }
      switchTime = rclcpp::Clock(RCL_ROS_TIME).now();
    }
  };
  positionCtrlSub_ = get_node()->create_subscription<std_msgs::msg::Float32>("/position_control", 1, positionModeCallback);

  RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "rl_controller initialized!!!!!!!!!");
  return CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn RLControllerBase::on_configure([[maybe_unused]] const rclcpp_lifecycle::State &previous_state) {
  return controller_interface::CallbackReturn::SUCCESS;
}


// Second
controller_interface::InterfaceConfiguration RLControllerBase::command_interface_configuration()
    const {
  // Initialize configuration
  controller_interface::InterfaceConfiguration conf = {config_type::INDIVIDUAL, {}};
  // Reserve space: number of joints × number of command interfaces
  conf.names.reserve(joint_names_.size() * command_interface_types_.size());
  // Iterate over all joint names and command interface types, and store them in conf.names
  for (const auto &joint_name : joint_names_) {
    for (const auto &interface_type : command_interface_types_) {
      conf.names.push_back(joint_name + "/" + interface_type);
    }
  }

  return conf;
}

controller_interface::InterfaceConfiguration RLControllerBase::state_interface_configuration() const {
  controller_interface::InterfaceConfiguration conf = {config_type::INDIVIDUAL, {}};

  conf.names.reserve(joint_names_.size() * state_interface_types_.size());
  for (const auto &joint_name : joint_names_) {
    for (const auto &interface_type : state_interface_types_) {
      conf.names.push_back(joint_name + "/" + interface_type);
    }
  }

  // ===== IMU state interfaces =====
  conf.names.push_back("imu/orientation.x");
  conf.names.push_back("imu/orientation.y");
  conf.names.push_back("imu/orientation.z"); 
  conf.names.push_back("imu/orientation.w");

  conf.names.push_back("imu/angular_velocity.x");
  conf.names.push_back("imu/angular_velocity.y");
  conf.names.push_back("imu/angular_velocity.z");

  conf.names.push_back("imu/linear_acceleration.x");
  conf.names.push_back("imu/linear_acceleration.y");
  conf.names.push_back("imu/linear_acceleration.z");

  return conf;
}

controller_interface::CallbackReturn RLControllerBase::on_activate(const rclcpp_lifecycle::State &) {
  // sim !!!
  std::map<std::string, std::vector<int>> state_interface_indices;
  for (int i = 0; i < static_cast<int>(state_interfaces_.size()); i++) {
    state_interface_indices[state_interfaces_[i].get_prefix_name()].push_back(i);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", state_interfaces_[i].get_name().c_str());
  }

  RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "size: " << state_interfaces_.size());

  std::map<std::string, std::vector<int>> command_interface_indices;
  for (int i = 0; i < static_cast<int>(command_interfaces_.size()); i++) {
    command_interface_indices[command_interfaces_[i].get_prefix_name()].push_back(i);
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%s", command_interfaces_[i].get_name().c_str());
  }

  RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "command success");

  for (const auto &joint_name : joint_names_) {
    auto state_indices = state_interface_indices[joint_name][0];
    auto command_indices = command_interface_indices[joint_name][0];
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "%d", state_indices);
    auto joint_handle = std::make_unique<HybridJointHandle>(
        std::ref(state_interfaces_[state_indices]),          // Position feedback
        std::ref(state_interfaces_[state_indices + 1]),      // Velocity feedback
        std::ref(state_interfaces_[state_indices + 2]),      // Torque feedback
        std::ref(command_interfaces_[command_indices]),      // Position desired
        std::ref(command_interfaces_[command_indices + 1]),  // Velocity desired
        std::ref(command_interfaces_[command_indices + 2]),  // Torque desired
        std::ref(command_interfaces_[command_indices + 3]),  // Kp desired
        std::ref(command_interfaces_[command_indices + 4])   // Kd desired
    );
    hybridJointHandles_.push_back(std::move(joint_handle));
  }

  starting();

  return CallbackReturn::SUCCESS;
}

void RLControllerBase::starting() {
  updateStateEstimation(get_node()->now(), rclcpp::Duration::from_seconds(0.002)); // 500Hz Control frequency in Controller
  currentJointAngles_.resize(actuatedDofNum_);

  // sim
  for (size_t i = 0; i < hybridJointHandles_.size(); i++) {
    currentJointAngles_[i] = hybridJointHandles_[i]->getPosCurr(); // 开始时强行刷新内部状态为机器人posCurr
  }
  resetStandHoldReference();

  scalar_t durationSecs = 2.0;
  standDuration_ = durationSecs * 1000.0;
  standPercent_ = 0;
  mode_ = Mode::DEFAULT;
  loopCount_ = 0;
}

void RLControllerBase::resetStandHoldReference() {
  standHoldRefAngles_.resize(actuatedDofNum_);

  for (int i = 0; i < actuatedDofNum_; ++i) {
    const bool hasMeasuredPosition = i < allJointPos_.size() && std::isfinite(allJointPos_(i));
    standHoldRefAngles_(i) = hasMeasuredPosition ? allJointPos_(i) : standJointAngles_(i);
  }
}

controller_interface::return_type RLControllerBase::update(
    const rclcpp::Time &time, const rclcpp::Duration &period) {
  // Read IMU first so updateStateEstimation() sees the current cycle's data.
  size_t imu_offset = joint_names_.size() * state_interface_types_.size();
  imuData_.orientation.x = state_interfaces_[imu_offset + 0].get_value();
  imuData_.orientation.y = state_interfaces_[imu_offset + 1].get_value();
  imuData_.orientation.z = state_interfaces_[imu_offset + 2].get_value();
  imuData_.orientation.w = state_interfaces_[imu_offset + 3].get_value();
  imuData_.angular_velocity.x = state_interfaces_[imu_offset + 4].get_value();
  imuData_.angular_velocity.y = state_interfaces_[imu_offset + 5].get_value();
  imuData_.angular_velocity.z = state_interfaces_[imu_offset + 6].get_value();
  imuData_.linear_acceleration.x = state_interfaces_[imu_offset + 7].get_value();
  imuData_.linear_acceleration.y = state_interfaces_[imu_offset + 8].get_value();
  imuData_.linear_acceleration.z = state_interfaces_[imu_offset + 9].get_value();

  // // RCLCPP_INFO_STREAM(rclcpp::get_logger("rclcpp"), "123");
  updateStateEstimation(time, period);
  // std::cout << " loopCount_" << loopCount_ << " standPercent_" << standPercent_ << " initJointAngles_[0]" << initJointAngles_[0]
  //         << "\n";

  switch (mode_) {
    case Mode::DEFAULT:
      handleDefaultMode();
      break;
    case Mode::LIE:
      handleLieMode();
      break;
    case Mode::STAND:
      handleStandMode();
      break;
    case Mode::WALK:
      handleWalkMode();
      break;
    default:
      RCLCPP_ERROR_STREAM(rclcpp::get_logger("rclcpp"), "Unexpected mode encountered: " << static_cast<int>(mode_));
      break;
  }

  if (emergency_stop) {
    for (size_t j = 0; j < hybridJointHandles_.size(); ++j) {
      hybridJointHandles_[j]->setCommand(0, 0, 0, 3, 0);
    }
    emergency_stop = false;
    starting();
  }

  loopCount_++;

  return controller_interface::return_type::OK;
}

void RLControllerBase::holdJointsAtZero(const std::vector<int> &indices) {
  constexpr double targetPosition = 0.0;
  const int jointCount = static_cast<int>(hybridJointHandles_.size());
  const int measuredCount = static_cast<int>(allJointPos_.size());

  // 越界保护
  for (int idx : indices) {
    if (idx < 0 || idx >= jointCount || idx >= measuredCount) {
      continue;
    }

    const std::string &partName = jointNames[idx];
    const double stiffness = getGainOrDefault(robotCfg_.controlCfg.stiffness, partName);
    const double damping = getGainOrDefault(robotCfg_.controlCfg.damping, partName);
    const double delta_pos = targetPosition - allJointPos_(idx);

    //平滑过渡
    const double clamped_ref_pos = delta_pos > 0 // this example,ref_pos = targetPosition = 0.0
                                       ? std::min(targetPosition, allJointPos_(idx) + kDeltaPosThreshold)
                                       : std::max(targetPosition, allJointPos_(idx) - kDeltaPosThreshold);
    hybridJointHandles_[idx]->setCommand(clamped_ref_pos, 0, stiffness, damping, 0);
    //setCommand(des_pos, des_vel, kp, kd, des_tau)
  }
}

// input:非rl关节的序号vector, output:void, function:对非rl关节固定在cfg中stand_joint的关节角度,而非zero
void RLControllerBase::holdJointsAtStand(const std::vector<int> &indices)
{
  const int jointCount = static_cast<int>(hybridJointHandles_.size());
  const int measuredCount = static_cast<int>(allJointPos_.size());

  for (int idx : indices) {
    if (idx < 0 || idx >= jointCount || idx >= measuredCount) {
      continue;
    }

    const std::string &partName = jointNames[idx];
    const double stiffness = getGainOrDefault(robotCfg_.controlCfg.stiffness, partName);
    const double damping = getGainOrDefault(robotCfg_.controlCfg.damping, partName);
    const double targetPosition = standJointAngles_[idx];
    const double delta_pos = targetPosition - allJointPos_(idx);

    const double clamped_ref_pos = delta_pos > 0
                                       ? std::min(targetPosition, allJointPos_(idx) + kDeltaPosThreshold)
                                       : std::max(targetPosition, allJointPos_(idx) - kDeltaPosThreshold);
    hybridJointHandles_[idx]->setCommand(clamped_ref_pos, 0, stiffness, damping, 0);
  }
}

void RLControllerBase::holdJointsAtStandDirect(const std::vector<int> &indices)
{
  const int jointCount = static_cast<int>(hybridJointHandles_.size());

  for (int idx : indices) {
    if (idx < 0 || idx >= jointCount || idx >= standJointAngles_.size()) {
      continue;
    }

    const std::string &partName = jointNames[idx];
    const double stiffness = getGainOrDefault(robotCfg_.controlCfg.stiffness, partName);
    const double damping = getGainOrDefault(robotCfg_.controlCfg.damping, partName);
    hybridJointHandles_[idx]->setCommand(standJointAngles_[idx], 0, stiffness, damping, 0);
  }
}

void RLControllerBase::holdJointsAtStandWithDeadband(const std::vector<int> &indices)
{
  const int jointCount = static_cast<int>(hybridJointHandles_.size());
  if (standHoldRefAngles_.size() != standJointAngles_.size()) {
    resetStandHoldReference();
  }

  for (int idx : indices) {
    if (idx < 0 || idx >= jointCount || idx >= standJointAngles_.size() || idx >= standHoldRefAngles_.size()) {
      continue;
    }

    const std::string &partName = jointNames[idx];
    const double stiffness = getGainOrDefault(robotCfg_.controlCfg.stiffness, partName);
    const double damping = getGainOrDefault(robotCfg_.controlCfg.damping, partName);
    const double targetPosition = standJointAngles_[idx];
    const double currentRefPosition =
        std::isfinite(standHoldRefAngles_(idx)) ? standHoldRefAngles_(idx) : targetPosition;
    const double delta_pos = targetPosition - currentRefPosition;

    const double ref_pos = std::abs(delta_pos) <= kStandDirectDeadband
                               ? targetPosition
                               : currentRefPosition + std::clamp(delta_pos, -kDeltaPosThreshold, kDeltaPosThreshold);
    standHoldRefAngles_(idx) = ref_pos;
    hybridJointHandles_[idx]->setCommand(ref_pos, 0, stiffness, damping, 0);
  }
}


void RLControllerBase::handleDefaultMode() {
  // 对rl控制的关节,脱力
  for (int j = 0; j < actionsSize_; j++) {
  hybridJointHandles_[j]->setCommand(0, 0, 0, 0.1, 0); 
  }
  // 对非rl控制关节锁死回零
  holdJointsAtZero(joint_mapping_fixed);
}

void RLControllerBase::handleLieMode() {
  if (standPercent_ <= 1) {
    for (int j = 0; j < actionsSize_; j++) {
      const std::string &partName = jointNames[j];
      const double stiffness = getGainOrDefault(robotCfg_.controlCfg.stiffness, partName);
      const double damping = getGainOrDefault(robotCfg_.controlCfg.damping, partName);
      scalar_t pos_des = currentJointAngles_[j] * (1 - standPercent_) + lieJointAngles_[j] * standPercent_;
      hybridJointHandles_[j]->setCommand(pos_des, 0, stiffness, damping, 0);
    }
    standPercent_ += 1 / standDuration_;
    standPercent_ = std::min(standPercent_, scalar_t(1));
  }

  //holdJointsAtZero(joint_mapping_fixed);
  holdJointsAtStand(joint_mapping_fixed);
}

void RLControllerBase::handleStandMode() {
  if (standPercent_ <= 1) {
    for (int j = 0; j < actionsSize_; j++) {
      const std::string &partName = jointNames[j];
      const double stiffness = getGainOrDefault(robotCfg_.controlCfg.stiffness, partName);
      const double damping = getGainOrDefault(robotCfg_.controlCfg.damping, partName);
      scalar_t pos_des = lieJointAngles_[j] * (1 - standPercent_) + standJointAngles_[j] * standPercent_;
      hybridJointHandles_[j]->setCommand(pos_des, 0, stiffness, damping, 0);
    }
    standPercent_ += 1 / standDuration_;
    standPercent_ = std::min(standPercent_, scalar_t(1));
  }

  //holdJointsAtZero(joint_mapping_fixed);
  holdJointsAtStand(joint_mapping_fixed);
}

controller_interface::CallbackReturn RLControllerBase::on_deactivate(const rclcpp_lifecycle::State &) {
  release_interfaces();

  return CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn RLControllerBase::on_cleanup(const rclcpp_lifecycle::State &) {
  return CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn RLControllerBase::on_error(const rclcpp_lifecycle::State &) {
  return CallbackReturn::SUCCESS;
}

controller_interface::CallbackReturn RLControllerBase::on_shutdown(const rclcpp_lifecycle::State &) {
  return CallbackReturn::SUCCESS;
}

void RLControllerBase::updateStateEstimation(const rclcpp::Time &time, const rclcpp::Duration &period [[maybe_unused]]) {
  vector_t jointPos(hybridJointHandles_.size()), jointVel(hybridJointHandles_.size()),
      jointTor(hybridJointHandles_.size()), output_torque(hybridJointHandles_.size()),
      pos_des_output(hybridJointHandles_.size()), vel_des_output(hybridJointHandles_.size());

  vector_t imuEulerXyz(3);
  quaternion_t quat;
  vector3_t angularVel, linearAccel;
  matrix3_t orientationCovariance, angularVelCovariance, linearAccelCovariance;

  for (size_t i = 0; i < hybridJointHandles_.size(); ++i) {
    jointPos(i) = hybridJointHandles_[i]->getPosCurr();
    allJointPos_(i) = hybridJointHandles_[i]->getPosCurr();
    jointVel(i) = hybridJointHandles_[i]->getVelCurr();
    jointTor(i) = hybridJointHandles_[i]->getTauCurr();

    pos_des_output(i) = hybridJointHandles_[i]->getPosDes();
    vel_des_output(i) = hybridJointHandles_[i]->getVelDes();
    output_torque(i) = hybridJointHandles_[i]->getFeedforward() +
                       hybridJointHandles_[i]->getKp() * (hybridJointHandles_[i]->getPosDes() - hybridJointHandles_[i]->getPosCurr()) +
                       hybridJointHandles_[i]->getKd() * (hybridJointHandles_[i]->getVelDes() - hybridJointHandles_[i]->getVelCurr());
  }

  // sim
  quat.x() = imuData_.orientation.x;
  quat.y() = imuData_.orientation.y;
  quat.z() = imuData_.orientation.z;
  quat.w() = imuData_.orientation.w;

  angularVel(0) = imuData_.angular_velocity.x;
  angularVel(1) = imuData_.angular_velocity.y;
  angularVel(2) = imuData_.angular_velocity.z;
  linearAccel(0) = imuData_.linear_acceleration.x;
  linearAccel(1) = imuData_.linear_acceleration.y;
  linearAccel(2) = imuData_.linear_acceleration.z;
  for (size_t i = 0; i < 9; ++i) {
    orientationCovariance(i) = imuData_.orientation_covariance[i];
    angularVelCovariance(i) = imuData_.angular_velocity_covariance[i];
    linearAccelCovariance(i) = imuData_.linear_acceleration_covariance[i];
  }

  // Reorder leg joints from hardware order (all-L then all-R) to policy order
  // (interleaved L/R: L_pitch,R_pitch, L_roll,R_roll, ...) using leg_joint_mapping.
  vector_t selectedJointPos(12);
  for (size_t i = 0; i < 12; ++i) {
    selectedJointPos(i) = jointPos(leg_joint_mapping[i]);
  }
  vector_t selectedJointVel(12);
  for (size_t i = 0; i < 12; ++i) {
    selectedJointVel(i) = jointVel(leg_joint_mapping[i]);
  }
  propri_.jointPos = selectedJointPos;
  propri_.jointVel = selectedJointVel;
  propri_.baseAngVel = angularVel;

  vector3_t gravityVector(0, 0, -1);
  vector3_t zyx = quatToZyx(quat);
  matrix_t inverseRot = getRotationMatrixFromZyxEulerAngles(zyx).inverse();
  propri_.projectedGravity = inverseRot * gravityVector;
  propri_.baseEulerXyz = quatToXyz(quat);
  for (size_t i = 0; i < 3; ++i) {
    imuEulerXyz(i) = propri_.baseEulerXyz[i];
  }

  realJointPosPublisher_->publish(createFloat64MultiArrayFromVector(jointPos));
  realJointVelPublisher_->publish(createFloat64MultiArrayFromVector(jointVel));
  realJointTorquePublisher_->publish(createFloat64MultiArrayFromVector(jointTor));

  realImuAngularVelPublisher_->publish(createFloat64MultiArrayFromVector(angularVel));
  realImuLinearAccPublisher_->publish(createFloat64MultiArrayFromVector(linearAccel));
  realImuEulerXyzPulbisher_->publish(createFloat64MultiArrayFromVector(imuEulerXyz));

  rlPlannedJointPosPublisher_->publish(createFloat64MultiArrayFromVector(pos_des_output));
  rlPlannedJointVelPublisher_->publish(createFloat64MultiArrayFromVector(vel_des_output));
  rlPlannedTorquePublisher_->publish(createFloat64MultiArrayFromVector(output_torque));
}

void RLControllerBase::cmdVelCallback(const geometry_msgs::msg::Twist::SharedPtr msg) {
  command_.x = std::clamp(static_cast<double>(msg->linear.x), -kMaxCmdVx, kMaxCmdVx);
  command_.y = std::clamp(static_cast<double>(msg->linear.y), -kMaxCmdVy, kMaxCmdVy);
  command_.yaw = std::clamp(static_cast<double>(msg->angular.z), -kMaxCmdYaw, kMaxCmdYaw);
}

void RLControllerBase::joyInfoCallback(const sensor_msgs::msg::Joy::SharedPtr msg) {
  // memcpy(joyInfo.axes, msg.axes, sizeof(joyInfo.axes));
  // memcpy(joyInfo.buttons, msg.buttons, sizeof(joyInfo.buttons));
  for (int i = 0; i < static_cast<int>(msg->axes.size()); i++) {
    joyInfo.axes[i] = msg->axes[i];
    // std::cout << joyInfo.axes[i];
    // std::cout << std::endl;
  }
  for (int i = 0; i < static_cast<int>(msg->buttons.size()); i++) {
    joyInfo.buttons[i] = msg->buttons[i];
    // std::cout << joyInfo.buttons[i];
    // std::cout << std::endl;
  }

  if (msg->buttons.size() > 7 && msg->buttons[7] == 0) {
    start_control_latched = false;
  }
  if (msg->buttons.size() > 0 && msg->buttons[0] == 0) {
    switch_mode_latched = false;
  }
  if (msg->buttons.size() > 2 && msg->buttons[2] == 0) {
    walk_mode_latched = false;
  }
  if (msg->buttons.size() > 3 && msg->buttons[3] == 0) {
    position_control_latched = false;
  }
}

}  // namespace legged

#include "pluginlib/class_list_macros.hpp"

PLUGINLIB_EXPORT_CLASS(
    legged::RLControllerBase, controller_interface::ControllerInterface)
