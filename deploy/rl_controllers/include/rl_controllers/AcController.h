// =============================================================================
// Purpose  : 基于 Actor-Critic 架构 ONNX 模型的具体 RL 行走控制器。
//            实现完整的策略推理流水线：历史观测堆叠 → ONNX 前向推理 →
//            动作裁剪 → 关节 PD 目标写入硬件接口。
//
// Dependencies: 继承 RLControllerBase（控制循环骨架）；
//              推理依赖 ONNX Runtime（onnxruntime_cxx_api.h）；
//              由 ros2_control 插件加载器通过 pluginlib 动态加载。
//
// Architecture: 覆盖基类的四个钩子方法：
//   loadModel()         → 从 YAML 配置路径加载 .onnx 模型到 ONNX Runtime Session。
//   loadRLCfg()         → 解析观测大小、历史帧数（numHist_）、指令阈值等。
//   computeObservation()→ 将当前帧与历史帧拼接，构造 [numHist_ × observationSize_]
//                         的输入张量（历史帧堆叠用于策略的时序感知）。
//   computeActions()    → 调用 ONNX Session::Run() → clip(actions, ±clipActions)
//                         → 缩放后写入 Posdes_（PD 目标位置）。
// =============================================================================
#pragma once

#include "rl_controllers/DreamWaqState.h"
#include "rl_controllers/RLControllerBase.h"

namespace legged {

/** Actor-Critic RL 行走控制器。
 *
 *  sw_mode_       : 是否启用相位时钟（phase-based gait switching）。
 *  numHist_       : 历史帧堆叠深度，observations_ 缓冲区大小 = numHist_ × observationSize_。
 *  isfirstRecObs_ : 首次接收观测时用当前帧填充全部历史槽（冷启动初始化）。
 *  lastActions_   : 前一 step 的动作输出，作为下一 step 观测的一部分（本体感知历史）。
 */
class AcController : public RLControllerBase {
  using tensor_element_t = float;

 public:
  AcController() : memoryInfo(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault)) {}
  ~AcController() override = default;

 protected:
  bool loadModel() override;
  bool loadRLCfg() override;
  void computeActions() override;
  void computeObservation() override;
  void handleWalkMode() override;
  void handleDreamWaqMode() override;
  void onEnterWalk() override;
  void onEnterDreamWaq() override;
  

 private:
  std::string policyFilePath_;
  std::shared_ptr<Ort::Env> onnxEnvPrt_;
  std::unique_ptr<Ort::Session> sessionPtr_;
  std::vector<const char*> inputNames_;
  std::vector<const char*> outputNames_;
  std::vector<Ort::AllocatedStringPtr> inputNodeNameAllocatedStrings;
  std::vector<Ort::AllocatedStringPtr> outputNodeNameAllocatedStrings;
  std::vector<std::vector<int64_t>> inputShapes_;
  std::vector<std::vector<int64_t>> outputShapes_;
  bool sw_mode_;
  double cmd_threshold_;
  //double phase_start_time_ = 0;
  std::string stacking_mode_ = "term"; // 默认值

  vector3_t baseLinVel_;
  vector3_t basePosition_;
  vector_t lastActions_;
  vector_t defaultJointAngles_;
  vector_t defaultJointAnglesActuated_;
  Ort::MemoryInfo memoryInfo;

  int observationSize_;
  int startCtrlSub_;
  int numHist_;
  std::vector<tensor_element_t> actions_;
  std::vector<tensor_element_t> observations_;
  Eigen::Matrix<tensor_element_t, Eigen::Dynamic, 1> proprioHistoryBuffer_;
  std::unique_ptr<DreamWaqState> dreamwaq_state_;

  bool isfirstRecObs_{true};
  bool pendingWalkColdStart_{false};
};

}  // namespace legged
