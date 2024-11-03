#include "Snake.h"
#include "Apple.h"
#include "utilities.h"

Snake::Snake() : mInitialSnakeLength(3), mColor(sf::Color::Green)
{

}

Snake::~Snake() 
{
    // do nothing 
}

void Snake::init()
{ 
    // generate the random starting position (x,y) of the snake's head
    mSnakeHeadStartingPosition = utils::generateRandomPair(); 

    for(int segmentIndex = 0; segmentIndex < mInitialSnakeLength; segmentIndex++)
    {
        // create new segment object 
        Segment* seg = new Segment(CELL_SIZE, CELL_SIZE); 

        // set the color segment
        seg->setColor(mColor); 

        // starting direction is always up
        seg->setDirection(Segment::DIRECTION::UP); 

        // set the starting position of the initial snake segments
        setSegmentStartingPosition(segmentIndex, *seg); 
        
        mSegments.push_back(seg);   
    }

    // set the head segment 
    mHead = mSegments[0]; 

}

void Snake::setSegmentStartingPosition(int aSegmentIndex, Segment& aSegmentOut)
{
    // zeroth index is the snake head
    if(aSegmentIndex == 0)
    {
        aSegmentOut.setPosition(GRID_X + (mSnakeHeadStartingPosition.first*CELL_SIZE), GRID_Y + (mSnakeHeadStartingPosition.second*CELL_SIZE)); 
    }
    else
    {
        // set offset from head 
        // equal to the index of the segment times segment size
        int seg_x = getSnakeHeadSegment()->x(); 
        int seg_y = getSnakeHeadSegment()->y() + (aSegmentIndex * CELL_SIZE); 
    
        aSegmentOut.setPosition(seg_x, seg_y); 
        
    }
}

void Snake::update()
{
    for(int i = mSegments.size()-1; i >=0; i--)
    {     
        // move the head by a cell size, then move body segments into the parent
        // segment's position
        if(i == 0)
        {
            auto seg = mSegments[i]; 

            switch (seg->direction())
            {
            case Segment::DIRECTION::UP:
                seg->setPosition(seg->x(), seg->y() - CELL_SIZE); 
                break;
            case Segment::DIRECTION::DOWN: 
                seg->setPosition(seg->x(), seg->y() + CELL_SIZE);
                break; 
            case Segment::DIRECTION::LEFT: 
                seg->setPosition(seg->x() - CELL_SIZE, seg->y()); 
                break; 
            case Segment::DIRECTION::RIGHT: 
                seg->setPosition(seg->x() + CELL_SIZE, seg->y());
                break; 
            default:
                break;
            }
        }
        else
        {
            mSegments[i]->setPosition(mSegments[i-1]->x(), mSegments[i-1]->y());
            mSegments[i]->setDirection(mSegments[i-1]->direction());  
        }
    }

    // set the head segment
    mHead = mSegments[0]; 
    
}

bool Snake::detectCollisions()
{
    // check if head is outside of the grid 
    if(collidedWithBorder())
    {
        return true; 
    }

    if(collidedWithSelf())
    {
        return true; 
    }

    // if it makes it here, there have been no collisions
    return false; 
}

bool Snake::collidedWithBorder()
{
    if(mHead->x() < MIN_X || mHead->x() >= MAX_X || mHead->y() < MIN_Y || mHead->y() >= MAX_Y)
    {
        return true; 
    }

    return false; 
}

bool Snake::collidedWithSelf()
{
    // check if the head collides with any of its body segments 
    for(long unsigned int i = 1; i < mSegments.size(); i++)
    {
        auto seg = mSegments[i]; 

        if(mHead->x() == seg->x() && mHead->y() == seg->y())
        {
            return true; 
        }
    }

    return false; 

}

bool Snake::ateAnApple()
{
    if(mHead->x() == Apple::get()->x() && mHead->y() == Apple::get()->y())
    {
        return true; 
    }
    return false; 
}

void Snake::grow()
{ 
    // get the current tail segment
    Segment* tail = getSnakeTailSegment();  
    
    // determine the new position of the tail segment
    std::pair<int,int> newTailPosition = determineNewTailPosition(tail); 

    // create new segment object
    Segment* seg = new Segment(CELL_SIZE, CELL_SIZE); 

    // set the position and color of the segment 
    seg->setPosition(newTailPosition.first, newTailPosition.second);
    seg->setColor(sf::Color::Green); 

    // push new tail segment to end of segments vector 
    mSegments.push_back(seg);

}

std::pair<int,int> Snake::determineNewTailPosition(Segment* aTailSegment)
{
    int seg_x, seg_y; 

    switch (aTailSegment->direction())
    {
    case Segment::DIRECTION::UP:
        seg_x = aTailSegment->x();
        seg_y = aTailSegment->y() - CELL_SIZE;
        break;
    case Segment::DIRECTION::DOWN: 
        seg_x = aTailSegment->x();
        seg_y = aTailSegment->y() + CELL_SIZE;
        break;
    case Segment::DIRECTION::LEFT: 
        seg_x = aTailSegment->x() + CELL_SIZE;
        seg_y = aTailSegment->y();
        break;
    case Segment::DIRECTION::RIGHT: 
        seg_x = aTailSegment->x() - CELL_SIZE;
        seg_y = aTailSegment->y();
        break;
    default:
        break;
    }

    return std::pair<int, int>(seg_x, seg_y); 

}

void Snake::reset()
{
    // delete the pointers to each segment in the vector
    for(auto seg : mSegments)
    {
        delete seg; 
    }
    
    Apple* apple = Apple::get(); 

    if(apple->score() >= apple->topScore())
    {
        apple->setTopScore(apple->score()); 
        apple->resetScore(); 
    }

    // re-init the starting size of the snake
    mInitialSnakeLength = 3;
    mSegments.clear(); 
    
    // re-init the snake 
    init(); 
}
