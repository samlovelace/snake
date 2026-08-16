#ifndef AI_H
#define AI_H

#include "Agent.h"

class AI : public Agent
{
public:
    AI();
    ~AI();

    DIRECTION move(const Observation& observation) override;

protected:

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
    DIRECTION move(); 

    /** 
     * @brief try() determines the reward for each direction the snake could move 
     *        based on its current position 
     */
    int tryDir(const Observation& observation, AI::TRY tryDir);

    /** 
     * @brief turnSnakeLeft() turns the snake to the left based on its current direction
     */
    DIRECTION turnSnakeLeft(DIRECTION currentDir); 

    /** 
     * @brief turnSnakeRight() turns the snake to the right based on its current direction
     */
    DIRECTION turnSnakeRight(DIRECTION currentDir); 

private:
    /* data */

};

#endif // AI_H

