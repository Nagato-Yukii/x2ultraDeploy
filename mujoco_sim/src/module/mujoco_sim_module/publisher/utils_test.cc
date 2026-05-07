// Copyright (c) 2023, AgiBot Inc.
// All rights reserved.

#include "mujoco_sim_module/publisher/utils.h"
#include <gtest/gtest.h>

namespace aimrt_mujoco_sim::mujoco_sim_module::publisher {

namespace {

mjModel MakeModelWithTimestep(double timestep) {
  mjModel model{};
  model.opt.timestep = timestep;
  return model;
}

}  // namespace

TEST(GetAvgIntervalBaseTest, BoundaryValidation) {
  auto model = MakeModelWithTimestep(0.002);
  EXPECT_ANY_THROW(GetAvgIntervalBase(0, &model));
  EXPECT_ANY_THROW(GetAvgIntervalBase(501, &model));
}

TEST(GetAvgIntervalBaseTest, ExactFrequency) {
  auto model = MakeModelWithTimestep(0.002);
  const std::vector<std::pair<uint32_t, double>> cases = {
      {1, 500.0},
      {10, 50.0},
      {500, 1.0}};

  for (const auto& [freq, expected] : cases) {
    EXPECT_DOUBLE_EQ(GetAvgIntervalBase(freq, &model), expected);
  }
}

TEST(GetAvgIntervalBaseTest, ApproximateFrequency) {
  auto model = MakeModelWithTimestep(0.002);
  // valid_cases
  {
    const std::vector<uint32_t> valid_cases = {48, 100, 333};
    for (auto freq : valid_cases) {
      EXPECT_NO_THROW(GetAvgIntervalBase(freq, &model));
    }
  }

  // invalid_cases
  {
    const std::vector<uint32_t> invalid_cases = {499};
    for (auto freq : invalid_cases) {
      EXPECT_ANY_THROW(GetAvgIntervalBase(freq, &model));
    }
  }
}

}  // namespace aimrt_mujoco_sim::mujoco_sim_module::publisher
