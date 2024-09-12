#include "AI.h"
#include "Snake.h"
#include "utilities.h"
#include "Apple.h"


AI::AI(/* args */)
{
}

AI::~AI()
{
}

int AI::state(AI::TRY tryDir)
{
    int reward = 0; 
    Segment* head = Snake::get()->getSegments()[0]; 

    int try_x = head->x(); 
    int try_y = head->y();  

    switch (head->direction())
    {
    case Segment::DIRECTION::UP:
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
    case Segment::DIRECTION::DOWN: 
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
    case Segment::DIRECTION::LEFT:
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
    case Segment::DIRECTION::RIGHT: 
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

    
    
    // check conditions for reward 
    // THERE IS A BUG HERE THAT CAUSES THE AI TO DIE WHEN GOING INTO THE LEFT WALL
    if(try_x < MIN_X || try_x >= MAX_X)
    {
        reward += -100; 
    }
    if(try_y < MIN_Y || try_y >= MAX_Y)
    {
        reward += -100; 
    }

    int apple_x = Apple::get()->x(); 
    int apple_y = Apple::get()->y(); 

    if(try_x == apple_x && try_y == apple_y)
    {
        reward += 100; 
    }

    int diff_x = abs(head->x() - apple_x); 
    int diff_y = abs(head->y() - apple_y); 
    int try_diff_x = abs(try_x - apple_x); 
    int try_diff_y = abs(try_y - apple_y); 

    if(try_diff_x < diff_x)
    {
        reward += 5; 
    }
    if(try_diff_y < diff_y)
    {
        reward += 5; 
    }
    
    return reward; 
}



void AI::move()
{
    int try_f = state(AI::TRY::FORWARD); 
    int try_l = state(AI::TRY::LEFT); 
    int try_r = state(AI::TRY::RIGHT); 

    //printf("try_f: %d, try_l: %d, try_r: %d\n", try_f, try_l, try_r); 

    if(try_f >= try_l && try_f >= try_r)
    {
        // no change in direction
    }
    else if (try_l > try_r)
    {
        turnSnakeLeft();  
    }
    else
    {
        turnSnakeRight();  
    }
}

void AI::turnSnakeLeft()
{
    // get the direction for the head of the snake
    Snake* snake = Snake::get(); 

    Segment::DIRECTION currentDir = snake->getSegments()[0]->direction(); 

    switch (currentDir)
    {
    case Segment::DIRECTION::UP:
        snake->setDirection(Segment::DIRECTION::LEFT);
        break;
    case Segment::DIRECTION::DOWN: 
        snake->setDirection(Segment::DIRECTION::RIGHT); 
        break; 
    case Segment::DIRECTION::LEFT:
        snake->setDirection(Segment::DIRECTION::DOWN); 
        break; 
    case Segment::DIRECTION::RIGHT:
        snake->setDirection(Segment::DIRECTION::UP); 
        break; 
    default:
        break;
    }
}


void AI::turnSnakeRight()
{
    // get the direction for the head of the snake
    Snake* snake = Snake::get(); 

    Segment::DIRECTION currentDir = snake->getSegments()[0]->direction(); 

    switch (currentDir)
    {
    case Segment::DIRECTION::UP:
        snake->setDirection(Segment::DIRECTION::RIGHT);
        break;
    case Segment::DIRECTION::DOWN: 
        snake->setDirection(Segment::DIRECTION::LEFT); 
        break; 
    case Segment::DIRECTION::LEFT:
        snake->setDirection(Segment::DIRECTION::UP); 
        break; 
    case Segment::DIRECTION::RIGHT:
        snake->setDirection(Segment::DIRECTION::DOWN); 
        break; 
    default:
        break;
    }

}