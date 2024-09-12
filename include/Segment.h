#ifndef SEGMENT_H
#define SEGMENT_H

#include "Entity.h"

class Segment : public Entity
{
public:
    Segment(int width, int height);
    ~Segment();

    enum class DIRECTION
    {
        UP,
        DOWN,
        LEFT, 
        RIGHT
    };

    //******* Getters/Setters **********// 
    DIRECTION direction() {return mDirection; }
    void setDirection(Segment::DIRECTION aDirection){mDirection = aDirection;}


private: 
    DIRECTION mDirection; 
};
#endif // SEGMENT_H

