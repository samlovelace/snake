#include "Segment.h"

Segment::Segment(const int width, const int height) : 
        mSegment(sf::RectangleShape(sf::Vector2f(width, height)))
{
}

Segment::~Segment()
{
}
