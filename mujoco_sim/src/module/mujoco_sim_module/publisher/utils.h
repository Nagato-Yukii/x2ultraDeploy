// Copyright (c) 2023, AgiBot Inc.
// All rights reserved.

#pragma once

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include "mujoco/mujoco.h"
#include "mujoco_sim_module/global.h"

namespace aimrt_mujoco_sim::mujoco_sim_module::publisher {

inline uint32_t GetSimFrequencyHz(const mjModel* m) {
  AIMRT_CHECK_ERROR_THROW(m != nullptr, "MuJoCo model is null.");
  AIMRT_CHECK_ERROR_THROW(m->opt.timestep > 0.0, "Invalid MuJoCo timestep {}.", m->opt.timestep);
  return static_cast<uint32_t>(std::llround(1.0 / m->opt.timestep));
}

inline double GetAvgIntervalBase(const uint32_t channel_frq, const mjModel* m) {
  constexpr static double kErrorRate = 0.05;
  const uint32_t sim_freq_hz = GetSimFrequencyHz(m);

  AIMRT_CHECK_ERROR_THROW((channel_frq <= sim_freq_hz && channel_frq > 0),
                          "Invalid channel frequency {}, exceeds the maximum frequency ({} Hz)",
                          channel_frq, sim_freq_hz);

  double avg_interval_base = static_cast<double>(sim_freq_hz) / static_cast<double>(channel_frq);

  auto result = std::div(static_cast<int64_t>(sim_freq_hz), static_cast<int64_t>(channel_frq));

  if (result.rem != 0) {
    const uint32_t lower_interval = result.quot;
    const uint32_t upper_interval = lower_interval + 1;

    const double lower_error = std::abs(lower_interval - avg_interval_base) / avg_interval_base;
    const double upper_error = std::abs(upper_interval - avg_interval_base) / avg_interval_base;

    AIMRT_CHECK_ERROR_THROW((lower_error <= kErrorRate && upper_error <= kErrorRate),
                            "Invalid channel frequency {}, which causes the frequency error is more than {} ",
                            channel_frq, kErrorRate);
  }
  return avg_interval_base;
}

}  // namespace aimrt_mujoco_sim::mujoco_sim_module::publisher
