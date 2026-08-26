#ifndef TRAINING_AGENT_H
#define TRAINING_AGENT_H
 
#include "Agent.h"
#include "AI.h" // for TRY enum

#include <zmq.hpp>
 
class TrainingAgent : public Agent 
{ 
public:
    TrainingAgent();
    ~TrainingAgent() override;

    DIRECTION move(const Observation& observation) override;
    void reward(const Reward& reward) override; 

private:
    bool tryDir(const Observation& observation, AI::TRY tryDir);
    std::array<int, 4> foodDirFromObservation(const Observation& observation);
    State buildState(const Observation& observation);
    DIRECTION actionToDirection(int action, DIRECTION currentDir);

private:
    zmq::context_t mContext; 
    zmq::socket_t mSocket;

};
#endif //TRAINING_AGENT_H  