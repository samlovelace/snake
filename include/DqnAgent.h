#ifndef DQN_AGENT_H
#define DQN_AGENT_H

#include "Agent.h"

#include <onnxruntime_cxx_api.h>

#include <memory>
#include <string>

/**
 * @brief DqnAgent plays the game by running local inference against an ONNX
 *        export of a trained DQN (see train.py's save_checkpoint/export_onnx).
 *        Unlike TrainingAgent, there is no zmq/Python process involved.
 */
class DqnAgent : public Agent
{
public:
    explicit DqnAgent(const std::string& modelPath = "checkpoints/best/best.onnx");
    ~DqnAgent() override;

    DIRECTION move(const Observation& observation) override;
    void reward(const Reward& reward) override; // no-op: inference only

private:
    Ort::Env mEnv;
    Ort::MemoryInfo mMemInfo;
    std::unique_ptr<Ort::Session> mSession;
};

#endif // DQN_AGENT_H
