#ifndef APPLE_H
#define APPLE_H

#include "Segment.h"

class Apple : public Segment
{
public:
    
    static Apple* get()
    {
        static Apple* apple = new Apple();
        return apple; 
    }

    void init();
    void reset(); 
    int score() {return mScore; }

private:
    
    Apple(/* args */);
    ~Apple(); 

    int mScore;  
    
};

#endif