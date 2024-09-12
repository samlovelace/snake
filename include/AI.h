#ifndef AI_H
#define AI_H

class AI
{
public:
    AI(/* args */);
    ~AI();

    /** 
     * TRY - enum for each direction for the AI to "try"
     */
    enum class TRY
    {
        FORWARD, 
        LEFT, 
        RIGHT
    }; 

    /** 
     * @brief move() determines the next direction of the snake
     */
    void move(); 

    /** 
     * @brief state() determines the reward for each direction the snake could move 
     *        based on its current position 
     */
    int state(TRY tryDir); 

    /** 
     * @brief turnSnakeLeft() turns the snake to the left based on its current direction
     */
    void turnSnakeLeft(); 

    /** 
     * @brief turnSnakeRight() turns the snake to the right based on its current direction
     */
    void turnSnakeRight(); 

private:
    /* data */

};

#endif // AI_H

