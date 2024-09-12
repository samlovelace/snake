#ifndef APPLE_H
#define APPLE_H

#include "Entity.h"
#include "Snake.h"

class Apple : public Entity
{
public:
    
    static Apple* get()
    {
        static Apple* apple = new Apple();
        return apple; 
    }

    void init();
    void reset(); 
    bool generatedOnSnake(); 


    // ************* Getters/Setters *********** // 
    int score() {return mScore; }
    int topScore() {return mTopScore; }
    void setTopScore(int aTopScore) {mTopScore = aTopScore; }
    void resetScore() {mScore = 0;}

private:
    
    // private constructor/destructor 
    Apple(/* args */);
    ~Apple(); 

    int mScore;  
    int mTopScore; 
    
};

#endif