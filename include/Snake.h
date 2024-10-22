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

    void init(int x, int y); 
    void update(); 
    bool detectCollisions(); 
    void reset();  
    bool ateAnApple(); 
    void grow(); 

    // *********** GETTERS/SETTERS **************** // 
    std::vector<Segment*> getSegments(){return mSegments;}
    Segment* getSnakeHeadSegment() {return mSegments[0];}
    void setDirection(Segment::DIRECTION aDir) {mSegments[0]->setDirection(aDir);}

private: 
    Snake();
    ~Snake();

    void initializeSnakeSegments(); 

    void setSegmentStartingPosition(int aSegmentIndex, Segment& aSegment); 

    bool collidedWithBorder(); 

    bool collidedWithSelf(); 

    std::vector<Segment*> mSegments; 
    int mInitialSnakeLength;
    std::pair<float, float> mSnakeHeadStartingPosition; 
    sf::Color mColor;

    Segment* mHead; 
};
#endif // SNAKE_H