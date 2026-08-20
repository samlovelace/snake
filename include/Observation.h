#ifndef OBSERVATION_H
#define OBSERVATION_H

#include <vector>

#include <nlohmann/json.hpp>

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

    nlohmann::json toJson() const
    {
        nlohmann::json json; 
        json["head"] = {{"x", head.x}, {"y", head.y}};
        json["direction"] = static_cast<int>(direction);
        json["segments"] = nlohmann::json::array();
        for (const auto& segment : segments)
        {
            json["segments"].push_back({{"x", segment.x}, {"y", segment.y}});
        }
        json["apple"] = {{"x", apple.x}, {"y", apple.y}};

        return json; 
    }
};

#endif // OBSERVATION_H
