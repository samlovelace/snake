#ifndef TRAINING_AGENT_H
#define TRAINING_AGENT_H
 
#include "Agent.h"

#include <zmq.hpp>
 
class TrainingAgent : public Agent 
{ 
public:
    TrainingAgent();
    ~TrainingAgent() override;

    DIRECTION move(const Observation& observation) override;

private:
    zmq::context_t mContext; 
    zmq::socket_t mSocket;

};
#endif //TRAINING_AGENT_H  