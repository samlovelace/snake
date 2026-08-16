#ifndef AGENT_H
#define AGENT_H
 
#include "Observation.h"

class Agent
{
public:
    virtual ~Agent() = default;
    virtual DIRECTION move(const Observation& observation) = 0;

private:
   
};
#endif //AGENT_H    