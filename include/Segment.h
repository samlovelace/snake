#ifndef SEGMENT_H
#define SEGMENT_H

#include "Entity.h"
#include "Types.hpp"

class Segment : public Entity
{
public:
    Segment(int width, int height);
    ~Segment();

    DIRECTION direction() {return mDirection; }
    void setDirection(DIRECTION aDirection){mDirection = aDirection;}

private: 
    DIRECTION mDirection; 
};
#endif // SEGMENT_H

