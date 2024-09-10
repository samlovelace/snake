#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "Segment.h"

class Snake
{

public:

    static Snake* get()
    {
        static Snake* snake = new Snake();
        return snake; 
    }

    void init(); 
    void update(); 
    bool detectCollisions(); 
    void reset();  
    bool ateAnApple(); 
    void grow(); 

    // *********** GETTERS/SETTERS **************** // 
    std::vector<Segment*> getSegments(){return mSegments;}
    void setDirection(Segment::DIRECTION aDir) {mSegments[0]->setDirection(aDir);}

private: 
    Snake();
    ~Snake();

    std::vector<Segment*> mSegments; 
    int mNumSegments;
    sf::Color mColor;
};
#endif // SNAKE_H