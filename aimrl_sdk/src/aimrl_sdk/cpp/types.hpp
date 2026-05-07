// =============================================================================
// Purpose  : SDK 基础数据类型的内存契约。定义 DOF 常量、传感器采样结构体、
//            合成帧、待发指令、同步配置等所有核心路径依赖的 POD 类型。
//
// Dependencies: 纯头文件，仅依赖标准库 <array>/<cstdint>。
//              被 core.hpp、layout.hpp、bindings.cpp 以及所有使用传感器数据
//              的编译单元包含。
//
// Architecture: 全部使用 std::array（栈上固定大小），无动态分配，对实时线程
//              友好。DOF 常量以 constexpr int 形式集中声明，修改此处即可联动
//              所有下游结构体的编译期数组大小。
// =============================================================================
#pragma once

#include <array>
#include <cstdint>

namespace aimrl_sdk {

  /* A2 origin settings
constexpr int kArmDof = 14;
constexpr int kLegDof = 12;
constexpr int kFrameDim = 88;
  */
  // for X2.5 settings
constexpr int kArmDof = 14;
constexpr int kLegDof = 12;
constexpr int kWaistDof = 3; // *
constexpr int kHeadDof = 2; // *
constexpr int kFrameDim = 103; // for X2.5,(14+12+3+2)*3(pos,vel,eff)+imu_10(orientation,wel,vel)=103

/** 同步节拍的时钟来源。
 *  Fixed: 使用 system_clock 定时，节拍间隔固定，延迟确定性强。
 *  Imu  : 以 IMU 硬件时间戳驱动节拍，消除传感器到系统时钟的静态偏差，
 *          代价是节拍间隔受 IMU 抖动影响。
 */
enum class SyncClockSource : std::uint8_t {
  Fixed = 0,
  Imu = 1,
};

/** 纳秒级系统时间戳（与 ROS2 header.stamp 的 sec*1e9+nanosec 等价）。
 *  使用强类型包装而非裸 int64，防止与 Sequence32 混用。
 */
struct TimestampNs {
  std::int64_t value{0};
  friend constexpr auto operator<=>(TimestampNs, TimestampNs) = default;
};

/** 32 位消息序列号，用于检测数据包丢失和乱序。 */
struct Sequence32 {
  std::uint32_t value{0};
  friend constexpr auto operator<=>(Sequence32, Sequence32) = default;
};

/** 关节控制字段掩码，以位域组合表示一次 commit 涉及的字段集合。
 *  位域设计允许在单次 set_arm/set_leg 调用中只指定部分字段（如只更新 Kp/Kd），
 *  Core::commit() 据此决定发布哪些字段的指令。
 */
enum class Field : std::uint32_t {
  Position = 1u << 0,
  Velocity = 1u << 1,
  Effort = 1u << 2,
  Stiffness = 1u << 3,
  Damping = 1u << 4,
};

constexpr std::uint32_t to_mask(Field f) noexcept {
  return static_cast<std::uint32_t>(f);
}

// 对于state的消息，我只需要stamp{sec,nanosec},pos,vel,eff
/** 单次关节状态采样，直接映射自 JointStateArray 消息的最小必要字段。
 *  @tparam DOF 关节自由度（kArmDof=14 / kLegDof=12 / kWaistDof=3 / kHeadDof=2）。
 */
template <int DOF> struct JointSample {
  TimestampNs stamp{};
  Sequence32 header_seq{};
  std::array<double, DOF> pos{};
  std::array<double, DOF> vel{};
  std::array<double, DOF> eff{};
};

// 对于imu的消息,我只需要stamp{sec,nanosec},frame_id(eg.x2_head,x2_arm,x2_leg,x2_waist)
/** 单次 IMU 采样，四元数采用 xyzw 约定（与 ROS2 sensor_msgs/Imu 一致）。
 *  quat_xyzw 默认值为单位四元数 (0,0,0,1)。
 */
struct ImuSample {
  TimestampNs stamp{};
  Sequence32 header_seq{};
  std::array<double, 4> quat_xyzw{0, 0, 0, 1};
  std::array<double, 3> gyro{};
  std::array<double, 3> acc{};
};

// JointSample + ImuSample = Frame
/** 一个已对齐的多传感器融合观测帧，是策略网络的直接输入。
 *
 *  stamp   : 对应节拍的目标时间戳（sync_loop_ 内插/外插后的结果）。
 *  complete: arm + leg + waist + head + imu 五路数据在本 tick 均有样本可回溯。
 *  aligned : complete 且任意传感器最近样本与 tick 的时间偏差 <= max_skew_ns。
 *  skew_ns : 所有传感器中最大的时间偏差（绝对值），用于诊断对齐质量。
 *  x       : kFrameDim 维 float32 观测向量，内存布局定义于 layout.hpp。
 *            Python 端通过 ObsSlices 的各 @property 切片访问各分量。
 */
struct Frame {
  TimestampNs stamp{}; // 插值结果
  // `complete`: whether arm+leg+imu+waist+head samples were all available for this tick.
  // `aligned`: whether the complete frame passes the time-skew check.
  bool aligned{false};
  bool complete{false};
  std::int64_t skew_ns{0};
  std::array<float, kFrameDim> x{}; //kFrameDim大小的浮点数,具体顺序和含义需要在layout.hpp确认
};

// 待发送指令的消息字样
/** 等待 commit() 发出的关节控制指令缓冲区。
 *  mask 记录哪些 Field 已被 set_*() 写入；commit() 时只发布 mask 中置位的字段。
 *  has_any 标志避免在无任何指令时触发空发布，减少总线流量。
 */
template <int DOF> struct PendingCommand {
  bool has_any{false};
  std::uint32_t mask{0};

  std::array<double, DOF> pos{};
  std::array<double, DOF> vel{};
  std::array<double, DOF> eff{};
  std::array<double, DOF> kp{};
  std::array<double, DOF> kd{};
};

// 同步配置
/** 帧同步引擎的调度参数。控制节拍频率、时钟源、对齐容差和相位偏移。 */
struct SyncConfig {
  //double frame_hz{100.0};
  double frame_hz{50.0}; // 推理频率for x2 omnidirectional walking
  std::int64_t max_skew_ns{3'000'000}; // 3ms // 最大允许偏差
  int max_backtrack{200}; // 最大回溯搜索
  SyncClockSource clock_source{SyncClockSource::Fixed};
  // Additional delay (ns) before producing a frame for a given tick. This
  // intentionally trades a small, bounded latency for better cross-sensor
  // alignment by allowing access to samples after the tick for interpolation.
  /*在生成某一时刻的帧之前，人为增加一段额外的延迟。这是一种“由延迟换取精度”的权衡。*/
  std::int64_t align_delay_ns{0};
  // Tick schedule phase offset relative to integer multiples of the tick period.
  // This can be used to align ticks to sensor timestamp phases (e.g. 0.5ms).
  // Range: [0, tick_period_ns).
  /*相对于周期整数倍的“相位偏移”。用于让 SDK 的节拍与传感器的物理采样节拍“对齐”。*/
  std::int64_t phase_ns{0};
};

} // namespace aimrl_sdk
