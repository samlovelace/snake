#ifndef UTILS_H   
#define UTILS_H

#include <utility>
#include <random> 
#include "constants.h"
#include "Entity.h"

namespace utils
{
    inline std::pair<int, int> generateRandomPair()
    {
        // Create random number generators
        std::random_device rd;
        std::mt19937 gen(rd());

        // Define the range for the x and y positions (0 to GRID_WIDTH-1, and 0 to GRID_HEIGHT-1)
        std::uniform_int_distribution<> disX(0, GRID_SIZE/2 + GRID_SIZE/4);
        std::uniform_int_distribution<> disY(0, GRID_SIZE/2 + GRID_SIZE/4);

        // Generate random x and y positions within the grid
        //float x = MIN_X + disX(gen)*CELL_SIZE;
        //float y = MIN_Y + disY(gen)*CELL_SIZE;

        int x = disX(gen); 
        int y = disY(gen); 

        std::pair<float, float> coord = std::make_pair(x, y);  

        return coord; 
    }

    inline bool isCollided(Entity e1, Entity e2)
    {
        if(e1.x() == e2.x() && e1.y() == e2.y())
        {
            return true; 
        }
        return false; 
    }

} // namespace utils

#endif // UTILS_H