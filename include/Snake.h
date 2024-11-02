#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "Segment.h"

class Snake
{

public:

    /**
     * @brief accessor for the snake instance
     * @return a pointer to the snake instance 
     */
    static Snake* get()
    {
        static Snake* snake = new Snake();
        return snake; 
    }

    // *********** Primary Functions *************** //
    void init(int x, int y); 
    void update(); 
    bool detectCollisions(); 
    void reset();  
    bool ateAnApple(); 
    void grow(); 

    // *********** GETTERS/SETTERS **************** // 
    std::vector<Segment*> getSegments(){return mSegments;}
    Segment* getSnakeHeadSegment() {return mSegments[0];}
    Segment* getSnakeTailSegment() {return mSegments[mSegments.size() - 1];}
    void setDirection(Segment::DIRECTION aDir) {mSegments[0]->setDirection(aDir);}

private: 
    Snake();
    ~Snake();

    // ****************** Helper Functions ***********// 
    void initializeSnakeSegments(); 
    void setSegmentStartingPosition(int aSegmentIndex, Segment& aSegment); 
    bool collidedWithBorder(); 
    bool collidedWithSelf(); 
    std::pair<int, int> determineNewTailPosition(Segment* aTailSegment); 

    // ***************** Member Variables **************// 
    std::vector<Segment*> mSegments; 
    int mInitialSnakeLength;
    std::pair<float, float> mSnakeHeadStartingPosition; 
    sf::Color mColor;
    Segment* mHead; 
};
#endif // SNAKE_H