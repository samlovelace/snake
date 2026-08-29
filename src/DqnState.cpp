#include "DqnState.h"
#include "AI.h" // for TRY enum, turnSnakeLeft/turnSnakeRight
#include "constants.h" // grid size constants

namespace
{
    bool tryDir(const Observation& observation, AI::TRY tryDir)
    {
        bool danger = false;
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
                danger = true;
            }
        }

        // dont hit the border
        if(try_x < MIN_X || try_x >= MAX_X)
        {
            danger = true;
        }

        // dont hit the other border dummy
        if(try_y < MIN_Y || try_y >= MAX_Y)
        {
            danger = true;
        }

        return danger;
    }

    std::array<int, 4> foodDirFromObservation(const Observation& observation)
    {
        int dx = observation.apple.x - observation.head.x;
        int dy = observation.apple.y - observation.head.y;

        std::array<int, 4> food_direction = {
            dy < 0 ? 1 : 0,  // up
            dy > 0 ? 1 : 0,  // down
            dx < 0 ? 1 : 0,  // left
            dx > 0 ? 1 : 0   // right
        };

        return food_direction;
    }
}

namespace DqnState
{
    State build(const Observation& observation)
    {
        // one hot encoding of current direction
        std::array<int, 4> dir = {0, 0, 0, 0};
        dir[(int)observation.direction] = 1;

        // danger of doing each action
        std::array<AI::TRY, 3> tryDirs = {AI::TRY::FORWARD, AI::TRY::LEFT, AI::TRY::RIGHT};
        std::array<int, 3> danger = {0, 0, 0};
        for(int i = 0; i < 3; i++)
        {
            danger[i] = tryDir(observation, tryDirs[i]);
        }

        // relative direction to food
        std::array<int, 4> foodDir = foodDirFromObservation(observation);

        // relative distance to food
        float dx = (observation.head.x - observation.apple.x);
        float dy = (observation.head.y - observation.apple.y);
        float dist = sqrt(dx * dx + dy * dy);

        // normalize by the grid's max possible distance (the diagonal)
        const float maxDist = std::sqrt(2.0f) * GRID_DIM;
        float normalizedDist = dist / maxDist;

        State state;
        state.direction = dir;
        state.danger = danger;
        state.foodDir = foodDir;
        state.distance = normalizedDist;

        return state;
    }

    DIRECTION actionToDirection(int action, DIRECTION currentDir)
    {
        switch (static_cast<AI::TRY>(action))
        {
        case AI::TRY::LEFT:
            return AI::turnSnakeLeft(currentDir);
        case AI::TRY::RIGHT:
            return AI::turnSnakeRight(currentDir);
        case AI::TRY::FORWARD:
        default:
            return currentDir;
        }
    }
}
