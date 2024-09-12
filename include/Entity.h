#ifndef ENTITY_H
#define ENTITY_H

#include "SFML/Graphics.hpp"
#include "constants.h"

/** 
 * @brief Entity - base class for all entities being rendered 
 */
class Entity
{

public:
    Entity(int width, int height) : mDrawable(sf::RectangleShape(sf::Vector2f(width, height))) {}
    virtual ~Entity() {}

    // ******* Getters/Setters ********** // 
    void setPosition(int x, int y) {mDrawable.setPosition(x,y); }
    void setColor(sf::Color aColor) {mDrawable.setFillColor(aColor); }
    int height() {return mDrawable.getSize().y; }
    int width() {return mDrawable.getSize().x; }
    float x() {return mDrawable.getPosition().x; }
    float y() {return mDrawable.getPosition().y; }
    sf::RectangleShape getDrawable() {return mDrawable;}
    Entity entity() {return static_cast<Entity>(*this);}

private:
    sf::RectangleShape mDrawable; 
};

#endif //ENTITY_H