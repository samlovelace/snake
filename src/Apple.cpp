
#include "Apple.h"
#include "constants.h"
#include <random>

Apple::Apple(/* args */) : Entity(CELL_SIZE-1, CELL_SIZE-1)
{
}

Apple::~Apple()
{
}

void Apple::init()
{
    // Create random number generators
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range for the x and y positions (0 to GRID_WIDTH-1, and 0 to GRID_HEIGHT-1)
    std::uniform_int_distribution<> disX(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<> disY(0, GRID_HEIGHT - 1);

    // Generate random x and y positions within the grid
    float head_x = MIN_X + disX(gen)*CELL_SIZE;
    float head_y = MIN_Y + disY(gen)*CELL_SIZE;

    Entity::setPosition(head_x, head_y);
    Entity::setColor(sf::Color::Red); 

}

void Apple::reset()
{
    // called when snake eats an apple so increment the score and re-init the apple
    mScore++; 
    init(); 
}