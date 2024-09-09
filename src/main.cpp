#include "SFML/Graphics.hpp"
#include "constants.h"
#include "Snake.h" 
#include "Apple.h"
#include "glyphs.h"

int main()
{
    // the window where the game is played
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake"); 

    // new snake object
    Snake* snake = new Snake(); 

    // initialize snake at random point on board
    snake->init(); 

    // init the apple
    Apple::get()->init(); 

    // render the snake while the window is open
    while(window.isOpen())
    {
        sf::Event event; 
        while(window.pollEvent(event))
        {   
            // if user hit the close button or esc key, close the game
            if(event.type == sf::Event::Closed || 
                                sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close(); 
            }
            
            // check which arrow key was pressed 
            if(event.type == sf::Event::KeyPressed)
            {
               switch (event.key.code)
               {
                case sf::Keyboard::Up:
                    snake->setDirection(Segment::DIRECTION::UP); 
                    break;
                case sf::Keyboard::Down:
                    snake->setDirection(Segment::DIRECTION::DOWN); 
                    break; 
                case sf::Keyboard::Left: 
                    snake->setDirection(Segment::DIRECTION::LEFT); 
                    break; 
                case sf::Keyboard::Right: 
                    snake->setDirection(Segment::DIRECTION::RIGHT); 
                    break;
                default:
                    break;
               }
            }
        }

        // update the snakes position based on current direction 
        snake->update(); 

        // if collision occurs, reset the snake 
        if(snake->detectCollisions())
        { 
            snake->reset(); 
        } 

        // if the snake ate an apple, increase snake size and re-init the apple
        if(snake->ateAnApple())
        { 
            snake->grow(); 
            Apple::get()->init(); 
        }

        // clear the graphics buffer
        window.clear(); 

        // optional to draw grid 
        if(draw_grid)
        {
            std::vector<sf::RectangleShape> grid;
            for (int i = 0; i < GRID_WIDTH; ++i) {
                for (int j = 0; j < GRID_HEIGHT; ++j) {
                    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1)); // Slightly smaller for grid lines
                    cell.setPosition(MIN_X + i * CELL_SIZE, MIN_Y + j * CELL_SIZE);   // Use offset for centering
                    cell.setFillColor(sf::Color::Black);  // Cells are black
                    cell.setOutlineThickness(1);          // Create a grid-like outline
                    cell.setOutlineColor(sf::Color::White);
                    grid.push_back(cell);
                }
            }        

            for(auto& cell : grid)
            {
                window.draw(cell); 
            }

        }

        // draw each segment of the snake  
        for(auto seg : snake->getSegments())
        { 
            window.draw(seg->getDrawable());
        }

        // draw the apple 
        window.draw(Apple::get()->getDrawable()); 

        renderGlyph(window, ONE, gridWithPixels + gridWithPixels/4, 10, 5); 
        
        // actually display what was drawn
        window.display(); 

        // delay to make the game playable 
        sf::sleep(sf::milliseconds(150)); 
    }


    return 0; 
}