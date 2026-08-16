#ifndef AGENT_H
#define AGENT_H
 
#include "Types.hpp"
 
class Agent 
{ 
public:
    virtual ~Agent() = default; 
    virtual DIRECTION move(int x, int y, DIRECTION dir) = 0;

private:
   
};
#endif //AGENT_H    