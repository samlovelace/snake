#include "DqnAgent.h"
#include "DqnState.h"

#include <array>

namespace
{
    constexpr int OBS_DIM = 12;
    constexpr int N_ACTIONS = 3;
}

DqnAgent::DqnAgent(const std::string& modelPath)
    : mEnv(ORT_LOGGING_LEVEL_WARNING, "DqnAgent"),
      mMemInfo(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault))
{
    Ort::SessionOptions options;
    options.SetIntraOpNumThreads(1);
    mSession = std::make_unique<Ort::Session>(mEnv, modelPath.c_str(), options);
}

DqnAgent::~DqnAgent()
{
}

DIRECTION DqnAgent::move(const Observation& observation)
{
    State state = DqnState::build(observation);

    // must match the order train.py's parse_observation_tensor uses:
    // danger + direction + foodDir + [distance]
    std::array<float, OBS_DIM> input;
    int i = 0;
    for(int v : state.danger)    input[i++] = static_cast<float>(v);
    for(int v : state.direction) input[i++] = static_cast<float>(v);
    for(int v : state.foodDir)   input[i++] = static_cast<float>(v);
    input[i++] = state.distance;

    std::array<int64_t, 2> inputShape{1, OBS_DIM};
    Ort::Value inputTensor = Ort::Value::CreateTensor<float>(
        mMemInfo, input.data(), input.size(), inputShape.data(), inputShape.size());

    const char* inputNames[] = {"state"};
    const char* outputNames[] = {"q_values"};

    auto outputs = mSession->Run(Ort::RunOptions{nullptr}, inputNames, &inputTensor, 1,
                                  outputNames, 1);

    const float* qValues = outputs[0].GetTensorData<float>();
    int bestAction = 0;
    for(int a = 1; a < N_ACTIONS; a++)
    {
        if(qValues[a] > qValues[bestAction])
        {
            bestAction = a;
        }
    }

    return DqnState::actionToDirection(bestAction, observation.direction);
}

void DqnAgent::reward(const Reward& reward)
{
    // inference-only agent; no training feedback loop
}
