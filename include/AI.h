#ifndef AI_H
#define AI_H

class AI
{
private:
    /* data */
public:
    AI(/* args */);
    ~AI();

    enum class TRY
    {
        FORWARD, 
        LEFT, 
        RIGHT
    }; 

    void move(); 
    int state(TRY tryDir); 

    void turnSnakeLeft(); 
    void turnSnakeRight(); 


};

#endif // AI_H

