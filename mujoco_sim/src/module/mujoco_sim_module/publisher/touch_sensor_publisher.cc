// Copyright (c) 2023, AgiBot Inc.
// All rights reserved.

#include "mujoco_sim_module/publisher/touch_sensor_publisher.h"
#include <aimdk_msgs/msg/detail/touch_state__struct.hpp>
#include "mujoco_sim_module/common/xmodel_reader.h"

namespace YAML {
template <>
struct convert<aimrt_mujoco_sim::mujoco_sim_module::publisher::TouchSensorPublisherBase::Options> {
  using Options = aimrt_mujoco_sim::mujoco_sim_module::publisher::TouchSensorPublisherBase::Options;
  static YAML::Node encode(const Options& rhs) {
    YAML::Node node;

    YAML::Node names_node = YAML::Node();
    for (const auto& name : rhs.names) {
      names_node.push_back(name);
    }
    node["names"] = names_node;

    YAML::Node states_node = YAML::Node();
    for (const auto& state_group : rhs.states) {
      YAML::Node inner_states_node = YAML::Node();
      for (const auto& state : state_group) {
        YAML::Node state_node;
        state_node["bind_site"] = state.bind_site;
        state_node["bind_touch_sensor"] = state.bind_touch_sensor;
        inner_states_node.push_back(state_node);
      }
      states_node.push_back(inner_states_node);
    }
    node["states"] = states_node;

    return node;
  }

  static bool decode(const YAML::Node& node, Options& rhs) {
    if (node["names"] && node["names"].IsSequence()) {
      for (const auto& name_node : node["names"]) {
        rhs.names.push_back(name_node.as<std::string>());
      }
    }

    if (node["states"] && node["states"].IsSequence()) {
      for (const auto& state_group_node : node["states"]) {
        if (state_group_node.IsSequence()) {
          std::vector<Options::State> state_group;

          for (const auto& state_node : state_group_node) {
            Options::State state_options;

            if (state_node["bind_site"]) {
              state_options.bind_site = state_node["bind_site"].as<std::string>();
            }
            if (state_node["bind_touch_sensor"]) {
              state_options.bind_touch_sensor = state_node["bind_touch_sensor"].as<std::string>();
            }
            state_group.emplace_back(std::move(state_options));
          }
          rhs.states.emplace_back(std::move(state_group));
        }
      }
    }
    return true;
  }
};
}  // namespace YAML

namespace aimrt_mujoco_sim::mujoco_sim_module::publisher {
void TouchSensorPublisherBase::SetMj(mjModel* m, mjData* d) {
  m_ = m;
  d_ = d;
}

void TouchSensorPublisherBase::RegisterSensorAddr() {
  touch_sensor_group_num_ = options_.names.size();

  for (size_t index = 0; index < touch_sensor_group_num_; ++index) {
    name_vec_.emplace_back(options_.names[index]);

    std::vector<int32_t> addr_vec;
    for (const auto& state : options_.states[index]) {
      addr_vec.push_back(common::GetSensorIdBySensorName(m_, state.bind_touch_sensor).value_or(-1));
    }

    touch_sensor_num_vec_.emplace_back(addr_vec.size());

    sensor_addr_group_vec_.emplace_back(SensorAddrGroup{
        .addr_vec = std::move(addr_vec)});
  }
}

void TouchSensorPublisherBase::InitializeBase(YAML::Node options_node) {
  if (options_node && !options_node.IsNull())
    options_ = options_node.as<Options>();

  avg_interval_base_ = GetAvgIntervalBase(channel_frq_, m_);

  RegisterSensorAddr();

  options_node = options_;
}

void TouchSensorRos2Publisher::Initialize(YAML::Node options_node) {
  InitializeBase(options_node);
  AIMRT_CHECK_ERROR_THROW(aimrt::channel::RegisterPublishType<aimdk_msgs::msg::TouchState>(publisher_), "Register touch sensor publish type failed.");
}

void TouchSensorRos2Publisher::PublishSensorData() {
  static constexpr uint32_t ONE_MB = 1024 * 1024;

  if (counter_++ < avg_interval_) return;

  std::unique_ptr<SensorStateGroup[]> state_array(new SensorStateGroup[touch_sensor_group_num_]);

  for (size_t i = 0; i < touch_sensor_group_num_; i++) {
    const auto& addr_vec = sensor_addr_group_vec_[i].addr_vec;
    state_array[i].state_vec.reserve(touch_sensor_num_vec_[i]);
    for (int addr : addr_vec) {
      state_array[i].state_vec.push_back(addr < 0 ? 0.0 : d_->sensordata[addr]);
    }
  }

  executor_.Execute([this, state_array = std::move(state_array)]() {
    aimdk_msgs::msg::TouchState state;

    for (size_t i = 0; i < std::min<size_t>(touch_sensor_group_num_, 8); ++i) {
      if (!state_array[i].state_vec.empty()) {
        state.data[i] = static_cast<uint32_t>(state_array[i].state_vec[0]);
        state.is_touched[i] = (state.data[i] > 0);
      }
    }

    aimrt::channel::Publish(publisher_, state);
  });

  avg_interval_ += avg_interval_base_;

  if (counter_ > ONE_MB) {
    avg_interval_ -= ONE_MB;
    counter_ -= ONE_MB;
  }
}
}  // namespace aimrt_mujoco_sim::mujoco_sim_module::publisher
