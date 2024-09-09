#ifndef SEGMENT_H
#define SEGMENT_H

#include "SFML/Graphics.hpp"


class Segment
{
public:
    Segment(int width, int height);
    ~Segment();

    enum class DIRECTION
    {
        UP, 
        DOWN, 
        LEFT, 
        RIGHT
    };

    // ******* Trivial Getters/Setters ********** // 
    void setPosition(int x, int y) {mSegment.setPosition(x,y); }
    void setColor(sf::Color aColor) {mSegment.setFillColor(aColor); }
    int height() {return mHeight; }
    int width() {return mWidth; }
    float x() {return mSegment.getPosition().x; }
    float y() {return mSegment.getPosition().y; }
    DIRECTION direction() {return mDirection; }
    void setDirection(Segment::DIRECTION aDirection){mDirection = aDirection;}

    sf::RectangleShape getDrawable() {return mSegment;}

private:
    sf::RectangleShape mSegment;
    int mHeight, mWidth; 
    DIRECTION mDirection; 
};
#endif // SEGMENT_H

