
#include "Apple.h"
#include "utilities.h"

Apple::Apple(/* args */) : Entity(CELL_SIZE-1, CELL_SIZE-1), mScore(0), mTopScore(0)
{
}

Apple::~Apple()
{
}

void Apple::init()
{
    Entity::setColor(sf::Color::Red);

    // need to generate random coords for the apple and check if that collides with
    // any segment of the snake 
    do
    {
        std::pair<float, float> appleLoc = utils::generateRandomPair(); 
        this->setPosition(GRID_X + (CELL_SIZE*appleLoc.first), GRID_Y + (CELL_SIZE*appleLoc.second)); 

    } while (generatedOnSnake());
    
    

}

bool Apple::generatedOnSnake()
{
    for (auto seg : Snake::get()->getSegments())
    {
        if(utils::isCollided(this->entity(), seg->entity()))
        {
            return true; 
        }
    }
    return false; 

}

void Apple::reset()
{
    // called when snake eats an apple so increment the score and re-init the apple
    mScore++;
    printf("mScore: %d\n", mScore);  
    init(); 
}