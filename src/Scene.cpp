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
