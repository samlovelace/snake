#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <vector>
#include "Types.hpp"

struct Position
{
    int x;
    int y;
};

struct Observation
{
    Position head;
    DIRECTION direction;
    std::vector<Position> segments;  // full body, head included at [0]
    Position apple;
};

#endif // OBSERVATION_H
