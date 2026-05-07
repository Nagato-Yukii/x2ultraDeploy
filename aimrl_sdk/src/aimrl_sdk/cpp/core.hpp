// =============================================================================
// Purpose  : SDK 核心组件的接口声明。定义两个关键抽象：
//            1) Transport：传感器消息的发布/订阅接口（纯虚基类），
//               隔离底层 AIMRT 传输实现（iceoryx/ROS2），使 Core 不依赖
//               具体通信中间件，便于单元测试和后端切换；
//            2) Core：帧同步引擎 + 命令总线。消费来自 Transport 的异步
//               传感器消息，以固定频率（frame_hz）输出时间对齐的 Frame，
//               并将用户的关节指令经由 Transport 发布到硬件。
//
// Dependencies: types.hpp（数据结构）、ring_buffer.hpp（无锁环形缓冲区）、
//              statistics.hpp（遥测）、closed_ankle.hpp（踝关节闭链解算）；
//              被 bindings.cpp 实例化 Core（通过 open()）；
//              被 aimrt_transport.hpp 继承 Transport 接口。
//
// Architecture: 生产者-消费者 + 条件变量通知。
//   [传感器线程]  → on_*_state/on_imu() → 写入无锁环形缓冲区
//   [sync_loop_] → 按 frame_hz 节拍从缓冲区回溯取样 → 插值 → 组装 Frame
//                → 写入 frame_ring_ → notify_all(frame_cv_)
//   [Python 线程] → wait_next_frame_ex() → 等待 frame_cv_ → 读取 Frame
// =============================================================================
#pragma once
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <span>
#include <string>
#include <thread>  // jthread
#include <vector>

#include "ring_buffer.hpp"
#include "statistics.hpp"
#include "types.hpp"

#include "aimdk_msgs/msg/joint_state_array.hpp"
#include "aimdk_msgs/msg/joint_command_array.hpp"
#include "sensor_msgs/msg/imu.hpp"

namespace aimrl_sdk {

/** 传输层抽象基类。解耦 Core 与底层通信中间件（iceoryx/ROS2）。
 *
 *  实现类（AimrtTransport）在 start() 时订阅传感器 Topic，并在消息到达时
 *  回调 Callbacks 中对应的函数对象，将消息路由到 Core 的原始环形缓冲区。
 *  publish_*_command() 系列方法将 Core 组装好的指令消息发布到对应 Topic。
 */
class Transport {
 public:
  virtual ~Transport() = default;

  struct Callbacks {
    std::function<void(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &)>
        on_arm_state;
    std::function<void(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &)>
        on_leg_state;
    std::function<void(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &)>
        on_waist_state; // *
    std::function<void(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &)>
        on_head_state;  // *
    std::function<void(const std::shared_ptr<const sensor_msgs::msg::Imu> &)>
        on_imu;
  };

  // lifecycle
  virtual void start(Callbacks callbacks) = 0;
  virtual void stop() = 0;

  // publish (arm/leg/waist/head JointCommand)
  // TODO: use template to reduce virtual function calls
  virtual void publish_arm_command(TimestampNs stamp, Sequence32 seq,
                                   const PendingCommand<kArmDof> &cmd,
                                   std::span<const std::string> arm_names) = 0;
  virtual void publish_leg_command(TimestampNs stamp, Sequence32 seq,
                                   const PendingCommand<kLegDof> &cmd,
                                   std::span<const std::string> leg_names) = 0;
  virtual void publish_waist_command(TimestampNs stamp, Sequence32 seq,
                                   const PendingCommand<kWaistDof> &cmd,
                                   std::span<const std::string> waist_names) = 0;// *
  virtual void publish_head_command(TimestampNs stamp, Sequence32 seq,
                                   const PendingCommand<kHeadDof> &cmd,
                                   std::span<const std::string> head_names) = 0;// *

  /** Publish a sim-reset signal to /aima/sim/reset.
   *  Default no-op so non-simulation transports (real hardware) compile unchanged.
   */
  virtual void publish_sim_reset() {}
};

/** SDK 运行时核心：帧同步引擎 + 关节指令总线。
 *
 *  生命周期：Core(opt, transport) → start() → [运行] → stop() → ~Core()。
 *  不可复制/移动（内部持有线程和条件变量，生命周期与 shared_ptr 绑定）。
 *
 *  线程模型：
 *    - on_*_state/on_imu()：由 AIMRT 订阅线程调用，lock-free 写入对应原始环形缓冲区。
 *    - sync_loop_()：std::jthread，以 frame_hz 频率运行，执行多传感器数据对齐
 *      和帧组装；完成后写入 frame_ring_ 并 notify_all(frame_cv_)。
 *    - commit()：由 Python 策略线程调用，加锁读取 pending 指令后经 transport 发布。
 *    - wait_next_frame_ex()：由 Python 策略线程调用，在 frame_cv_ 上阻塞等待。
 */
class Core final {
 public:
  struct Options {
    std::uint32_t raw_ring{2048};
    std::uint32_t frame_ring{512};
    SyncConfig sync{};
    Statistics::Config statistics{};
    // internal fixed order name (not exposed to Python)
    std::vector<std::string> arm_names;  // size=14
    std::vector<std::string> leg_names;  // size=12
    std::vector<std::string> waist_names;  // size=3 // *
    std::vector<std::string> head_names;  // size=2 // *
  };

  Core(Options opt, std::unique_ptr<Transport> transport);
  ~Core();

  Core(const Core &) = delete;
  Core &operator=(const Core &) = delete;
  Core(Core &&) = delete;
  Core &operator=(Core &&) = delete;

  void start();
  void stop();
  bool running() const noexcept {
    return running_.load(std::memory_order_relaxed);
  }

  // ---- callbacks from transport subscriber ----
  void on_arm_state(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &msg);
  void on_leg_state(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &msg);
  void on_waist_state(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &msg);// *
  void on_head_state(const std::shared_ptr<const aimdk_msgs::msg::JointStateArray> &msg);// *
  void on_imu(const std::shared_ptr<const sensor_msgs::msg::Imu> &msg);

  // ---- frame read API (for pybind wrappers) ----
  /** 返回 frame_ring_ 中最新的一帧；若无任何帧则返回 nullopt。
   *  非阻塞，可能重复返回同一帧（调用方需比较 stamp 或 frame_seq 判断是否新帧）。
   */
  std::optional<Frame> latest_frame() const;

  /** 阻塞等待 after_seq 之后产生的新帧。
   *  @param after_seq  上次读取的帧序列号，等待 frame_seq_ > after_seq 时返回。
   *  @param timeout_s  超时秒数；nullopt 表示无限等待。
   *  @return 新帧；超时或 Core 停止时返回 nullopt（无法区分两种情况）。
   *  @note  GIL 在进入等待前必须释放（由 bindings.cpp 的 py::gil_scoped_release 保证）。
   */
  std::optional<Frame> wait_next_frame(std::uint64_t after_seq,
                                       std::optional<double> timeout_s);

  struct WaitNextFrameResult {
    WaitFrameStatus status{WaitFrameStatus::Stopped};
    std::optional<Frame> frame{};
  };
  WaitNextFrameResult wait_next_frame_ex(std::uint64_t after_seq,
                                         std::optional<double> timeout_s);

  /** 读取最近 n 帧（按旧→新顺序）。内存缓存区若历史帧不足 n 个，用默认帧（零值）前填充。
   *  用于策略网络的历史帧堆叠（frame stacking）场景。
   */
  std::vector<Frame> read_last_frames(int n) const;

  std::uint64_t frame_seq() const noexcept {
    return frame_seq_.load(std::memory_order_relaxed);
  }

  // ---- command API ----
  /** 写入臂关节指令缓冲区中的指定字段。
   *  @param f  要更新的字段（Position/Velocity/Effort/Stiffness/Damping）。
   *  @param v  关节值数组，v.size() 必须 == kArmDof(14)，否则抛出 invalid_argument。
   *  不会立即发送，需调用 commit() 才触发实际发布。
   */
  void set_arm(Field f, std::span<const double> v);  // v.size==14
  void set_leg(Field f, std::span<const double> v);  // v.size==12
  void set_waist(Field f, std::span<const double> v);  // v.size==3 // *
  void set_head(Field f, std::span<const double> v);  // v.size==2 // *
  /** 将标量广播填充至对应关节组的全部 DOF，适合设置统一刚度/阻尼等场景。 */
  void set_arm_scalar(Field f, double scalar);
  void set_leg_scalar(Field f, double scalar);
  void set_waist_scalar(Field f, double scalar);// *
  void set_head_scalar(Field f, double scalar);// *

  /** 将缓冲区中所有标记过的指令一次性发布到 Transport。
   *  @param stamp  可选时间戳（写入消息 header）；nullopt 时使用 now_system_ns()。
   *  @param seq    可选序列号；nullopt 时使用内部自增计数器。
   *  调用后清空 pending 缓冲区中的 has_any 标志。
   */
  void commit(std::optional<TimestampNs> stamp, std::optional<Sequence32> seq);

  /** Send std_msgs/msg/Empty to /aima/sim/reset, triggering MuJoCo ApplyStandState(). */
  void request_sim_reset();

  void set_statistics_config(Statistics::Config cfg) noexcept;
  Statistics::Config statistics_config() const noexcept;
  void reset_statistics() noexcept;
  StatisticsSnapshot statistics_snapshot() const noexcept;

 private:
  void sync_loop_(const std::stop_token &stoken);

  // find sample <= t (linear backtrack)
  template <class Ring, class Sample>
  static bool find_leq_(const Ring &ring, TimestampNs t,
                        std::uint64_t start_idx, int max_backtrack,
                        Sample &out);

 private:
  Options opt_;
  std::unique_ptr<Transport> transport_;

  // raw rings
  RingBuffer<JointSample<kArmDof>> arm_raw_;
  RingBuffer<JointSample<kLegDof>> leg_raw_;
  RingBuffer<JointSample<kWaistDof>> waist_raw_; // *
  RingBuffer<JointSample<kHeadDof>> head_raw_; // *
  RingBuffer<ImuSample> imu_raw_;

  // frame ring (store Frame directly)
  RingBuffer<Frame> frame_ring_;
  std::atomic<std::uint64_t> frame_seq_{0};

  // sync thread
  std::atomic<bool> running_{false};
  std::jthread sync_thread_;

  // waiters
  mutable std::mutex frame_mtx_;
  mutable std::condition_variable frame_cv_;

  // IMU-driven tick support
  mutable std::mutex imu_mtx_;
  mutable std::condition_variable imu_cv_;
  std::atomic<std::uint64_t> imu_seq_{0};

  // pending commands + publish atomicity
  std::mutex cmd_mtx_;
  PendingCommand<kArmDof> arm_pending_{};
  PendingCommand<kLegDof> leg_pending_{};
  PendingCommand<kWaistDof> waist_pending_{}; // *
  PendingCommand<kHeadDof> head_pending_{}; // *
  std::uint32_t commit_seq_{0};

  Statistics stats_{};
};

}  // namespace aimrl_sdk
