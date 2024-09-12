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

    inline void renderGrid(sf::RenderWindow& window)
    {
        std::vector<sf::RectangleShape> grid;
            for (int i = 0; i < GRID_SIZE+2; i++) {
                for (int j = 0; j < GRID_SIZE+2; j++) {
                    if(i== 0 || i == GRID_SIZE+1 || j == 0 || j == GRID_SIZE+1)
                    {
                        sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1)); // Slightly smaller for grid lines
                        
                        float x_position = GRID_X + ((i - 1) * CELL_SIZE); // Shift grid to the left by one CELL_SIZE
                        float y_position = GRID_Y + ((j - 1) * CELL_SIZE); // Shift grid upwards by one CELL_SIZE

                        cell.setPosition(x_position, y_position);   // Use offset for centering
                        cell.setFillColor(sf::Color::Black);  // Cells are black
                        cell.setOutlineThickness(1);          // Create a grid-like outline
                        cell.setOutlineColor(sf::Color::White);
                        grid.push_back(cell);
                    }
                    
                }
            }        

            for(auto& cell : grid)
            {
                window.draw(cell); 
            }
    }

    // Define a glyph for each number (0-9) using a 3x5 grid
    const int glyphs[10][5][3] = {
        { {1,1,1}, {1,0,1}, {1,0,1}, {1,0,1}, {1,1,1} }, // 0
        { {0,1,0}, {1,1,0}, {0,1,0}, {0,1,0}, {1,1,1} }, // 1
        { {1,1,1}, {0,0,1}, {1,1,1}, {1,0,0}, {1,1,1} }, // 2
        { {1,1,1}, {0,0,1}, {1,1,1}, {0,0,1}, {1,1,1} }, // 3
        { {1,0,1}, {1,0,1}, {1,1,1}, {0,0,1}, {0,0,1} }, // 4
        { {1,1,1}, {1,0,0}, {1,1,1}, {0,0,1}, {1,1,1} }, // 5
        { {1,1,1}, {1,0,0}, {1,1,1}, {1,0,1}, {1,1,1} }, // 6
        { {1,1,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1} }, // 7
        { {1,1,1}, {1,0,1}, {1,1,1}, {1,0,1}, {1,1,1} }, // 8
        { {1,1,1}, {1,0,1}, {1,1,1}, {0,0,1}, {1,1,1} }  // 9
    };

    // Function to render a blocky digit using a 3x5 glyph
    inline void renderGlyphDigit(sf::RenderWindow& window, int digit, float x, float y, float blockSize)
    {
        // Each digit is a 3x5 grid
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 3; ++col) {
                if (glyphs[digit][row][col] == 1) {
                    // Create a rectangle for each block in the glyph
                    sf::RectangleShape block(sf::Vector2f(blockSize, blockSize));
                    block.setPosition(x + col * blockSize, y + row * blockSize);
                    block.setFillColor(sf::Color::Blue);  // Block color
                    window.draw(block);
                }
            }
        }
    }

    // Function to render the entire score
    inline void renderScore(sf::RenderWindow& window, int score, float x, float y, float blockSize)
    {
        // Convert the score to a string of digits
        std::string scoreStr = std::to_string(score);

        // Render each digit using glyphs, spacing them
        float digitSpacing = 4 * blockSize;  // Add some space between digits
        for (size_t i = 0; i < scoreStr.size(); ++i) {
            int digit = scoreStr[i] - '0';  // Convert char to int
            renderGlyphDigit(window, digit, x + i * digitSpacing, y, blockSize);
        }
    }




} // namespace utils

#endif // UTILS_H