#include "AI.h"
#include "utilities.h"


AI::AI(/* args */)
{
}

AI::~AI()
{
}

DIRECTION AI::move(const Observation& observation)
{
    // for current position of snake, try each possible direction to move
    int try_f = tryDir(observation, AI::TRY::FORWARD);
    int try_l = tryDir(observation, AI::TRY::LEFT);
    int try_r = tryDir(observation, AI::TRY::RIGHT);

    DIRECTION newDir = observation.direction;

    // based on the scores from above, if try_f is the largest value, 
    // no need to change the direction of the snake
    if(try_f >= try_l && try_f >= try_r)
    {
        // no change in direction
    }
    else if (try_l > try_r)
    {
        newDir = turnSnakeLeft(observation.direction);
    }
    else
    {
        newDir = turnSnakeRight(observation.direction);
    }

    return newDir;
}

int AI::tryDir(const Observation& observation, AI::TRY tryDir)
{
    int reward = 0;
    int try_x = observation.head.x;
    int try_y = observation.head.y;

    switch (observation.direction)
    {
    case DIRECTION::UP:
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_y += -CELL_SIZE;  
            break;
        case AI::TRY::LEFT:
            try_x += -CELL_SIZE; 
            break; 
        case AI::TRY::RIGHT: 
            try_x += CELL_SIZE; 
            break; 
        default:
            break;
        }
        break;
    case DIRECTION::DOWN: 
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_y += CELL_SIZE; 
            break;
        case AI::TRY::LEFT:
            try_x += CELL_SIZE; 
            break;
        case AI::TRY::RIGHT: 
            try_x += -CELL_SIZE; 
            break; 
        default:
            break;
        }
        break;
    case DIRECTION::LEFT:
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_x += -CELL_SIZE; 
            break;
        case AI::TRY::LEFT: 
            try_y += CELL_SIZE;  
            break;
        case AI::TRY::RIGHT: 
            try_y += -CELL_SIZE;  
            break; 
        default:
            break;
        }
        break;
    case DIRECTION::RIGHT: 
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_x += CELL_SIZE; 
            break;
        case AI::TRY::LEFT: 
            try_y += -CELL_SIZE; 
            break; 
        case AI::TRY::RIGHT:
            try_y += CELL_SIZE; 
            break;
        default:
            break;
        }
    
    default:
        break;
    }

    // dont run into yourself dummy
    for(const auto& seg : observation.segments)
    {
        if(try_x == seg.x && try_y == seg.y)
        {
            reward += -100;
        }
    }

    // dont hit the border
    if(try_x < MIN_X || try_x >= MAX_X)
    {
        reward += -100; 
    }

    // dont hit the other border dummy 
    if(try_y < MIN_Y || try_y >= MAX_Y)
    {
        reward += -100; 
    }

    int apple_x = observation.apple.x;
    int apple_y = observation.apple.y;

    // eat the apple..... or die!
    if(try_x == apple_x && try_y == apple_y)
    {
        reward += 100;
    }

    // get closer to the apple, so you can eat it
    int diff_x = abs(observation.head.x - apple_x);
    int diff_y = abs(observation.head.y - apple_y);
    int try_diff_x = abs(try_x - apple_x); 
    int try_diff_y = abs(try_y - apple_y); 

    if(try_diff_x < diff_x)
    {
        reward += 25; 
    }
    if(try_diff_y < diff_y)
    {
        reward += 25; 
    }

    return reward; 
}

DIRECTION AI::turnSnakeLeft(DIRECTION currentDir)
{
    DIRECTION newDir = currentDir;

    switch (currentDir)
    {
    case DIRECTION::UP:
        newDir = DIRECTION::LEFT;
        break;
    case DIRECTION::DOWN: 
        newDir = DIRECTION::RIGHT;  
        break; 
    case DIRECTION::LEFT:
        newDir = DIRECTION::DOWN; 
        break; 
    case DIRECTION::RIGHT:
        newDir = DIRECTION::UP; 
        break; 
    default:
        break;
    }
    
    return newDir;
}

DIRECTION AI::turnSnakeRight(DIRECTION currentDir)
{
    DIRECTION newDir = currentDir;

    switch (currentDir)
    {
    case DIRECTION::UP:
        newDir = DIRECTION::RIGHT;
        break;
    case DIRECTION::DOWN: 
        newDir = DIRECTION::LEFT;  
        break; 
    case DIRECTION::LEFT:
        newDir = DIRECTION::UP; 
        break; 
    case DIRECTION::RIGHT:
        newDir = DIRECTION::DOWN; 
        break; 
    default:
        break;
    }
    
    return newDir;
}