// =============================================================================
// Purpose  : 以链式 constexpr 偏移量声明 Frame.x 观测向量的精确内存布局。
//            是 C++ 帧打包（core.cpp）与 Python 帧解析（obs.py/ObsSlices）
//            共用的单一事实来源（Single Source of Truth）。
//            修改此处会在编译期通过 static_assert 立即暴露布局不一致问题。
//
// Dependencies: 依赖 types.hpp 中的 kArmDof/kLegDof/kWaistDof/kHeadDof/kFrameDim。
//              被 core.cpp 用于将插值后的关节/IMU 数据写入 Frame.x；
//              被 bindings.cpp 导出各 *0 常量到 Python 模块，
//              供 ObsSlices.from_bindings() 同步布局。
//
// Architecture: 纯 constexpr 结构体，所有偏移量通过前序段的末尾位置连续叠加，
//              确保各段紧密排列无空洞。末尾 static_assert 在编译期强制校验
//              总维度 == kFrameDim，防止布局修改后 Python 端未同步更新。
// =============================================================================
#pragma once

#include "types.hpp"

namespace aimrl_sdk {

/** X2 Ultra 观测帧 (Frame.x) 的内存段偏移量集合。
 *
 *  各 *0 常量为对应段的起始下标（含）。段长度由对应的 k*Dof 常量决定。
 *  布局顺序：arm → leg → waist → head → imu (quat, gyro, acc)。
 *  总维度 103 == kFrameDim，由末尾 static_assert 编译期强制保证。
 */
struct FrameLayout final {
  static constexpr int ArmPos0 = 0;
  static constexpr int ArmVel0 = ArmPos0 + kArmDof;
  static constexpr int ArmEff0 = ArmVel0 + kArmDof;

  static constexpr int LegPos0 = ArmEff0 + kArmDof;
  static constexpr int LegVel0 = LegPos0 + kLegDof;
  static constexpr int LegEff0 = LegVel0 + kLegDof;

  /* for X2.5 settings*/
  static constexpr int WaistPos0 = LegEff0 + kLegDof; 
  static constexpr int WaistVel0 = WaistPos0 + kWaistDof;
  static constexpr int WaistEff0 = WaistVel0 + kWaistDof;

  static constexpr int HeadPos0 = WaistEff0 + kWaistDof;
  static constexpr int HeadVel0 = HeadPos0 + kHeadDof;
  static constexpr int HeadEff0 = HeadVel0 + kHeadDof;

  static constexpr int ImuQuat0 = HeadEff0 + kHeadDof;
  static constexpr int ImuGyro0 = ImuQuat0 + 4;
  static constexpr int ImuAcc0 = ImuGyro0 + 3;

  static constexpr int Dim = kFrameDim;
  static_assert(Dim == 103);// for X2.5 settings(Dim == 88);
  static_assert(ImuAcc0 + 3 == Dim);
};

}  // namespace aimrl_sdk
