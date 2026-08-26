#include "Scene.h"
#include "Snake.h"
#include "Apple.h"

Observation Scene::getObservation() const
{
    Snake* snake = Snake::get();
    Apple* apple = Apple::get();

    Observation obs;
    obs.head.x = snake->getX();
    obs.head.y = snake->getY();
    obs.direction = snake->getDirection();
    obs.apple.x = apple->x();
    obs.apple.y = apple->y();

    for (auto seg : snake->getSegments())
    {
        obs.segments.push_back({ (int)seg->x(), (int)seg->y() });
    }

    return obs;
}

float Scene::updateReward(const Observation& prev, const Observation& curr)
{
    float prevDx = prev.head.x - prev.apple.x; 
    float prevDy = prev.head.y - prev.apple.y;
    float prevDist = sqrt(prevDx*prevDx + prevDy*prevDy); 

    float currDx = curr.head.x - curr.apple.x; 
    float currDy = curr.head.y - curr.apple.y; 
    float currDist = sqrt(currDx*currDx + currDy*currDy); 

    float reward = 0; 
    if(currDist < prevDist)
    {
        reward = 1; 
    }
    else if(prevDist < currDist)
    {
        reward = -1; 
    }
    return reward; 
}


