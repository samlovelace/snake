#include "Snake.h"
#include "Apple.h"
#include "utilities.h"

Snake::Snake() : mNumSegments(3), mColor(sf::Color::Green)
{

}

Snake::~Snake() 
{
    // do nothing 
}

void Snake::init(const int x, const int y)
{ 
    for(int i = 0; i < mNumSegments; i++)
    {
        Segment* seg = new Segment(CELL_SIZE, CELL_SIZE); 
        mSegments.push_back(seg);   
    }

    std::pair<float,float> head = utils::generateRandomPair(); 

    // initialize the segments of the starting size of the snake 
    for(int i = 0; i < mSegments.size(); i++)
    {
        // set the color of the entire snake
        mSegments[i]->setColor(mColor); 

        // starting direction is always up
        mSegments[i]->setDirection(Segment::DIRECTION::UP); 

        if(i == 0)
        {
            mSegments[i]->setPosition(GRID_X + (head.first*CELL_SIZE), GRID_Y + (head.second*CELL_SIZE)); 
        }
        else
        {
            // set offset from head 
            // probably equal to the index of the segment times segment size 
            int seg_x = mSegments[0]->x(); 
            int seg_y = mSegments[0]->y() + (i* CELL_SIZE); 
        
            mSegments[i]->setPosition(seg_x, seg_y); 
            
        }
        
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
    
    
}

bool Snake::detectCollisions()
{
    // get the head of the snake
    auto head = mSegments[0]; 
    //printf("#############head_x: %4.3f, head_y: %4.3f################\n", head->x(), head->y()); 

    // check if head is outside of the grid 
    if(head->x() < MIN_X || head->x() >= MAX_X || head->y() < MIN_Y || head->y() >= MAX_Y)
    {
        return true; 
    }

    // check if the head collides with any of its body segments 
    for(int i = 1; i < mSegments.size(); i++)
    {
        auto seg = mSegments[i]; 

        if(head->x() == seg->x() && head->y() == seg->y())
        {
            return true; 
        }
    }

    return false; 
}

bool Snake::ateAnApple()
{
    auto head = mSegments[0]; 

    if(head->x() == Apple::get()->x() && head->y() == Apple::get()->y())
    {
        return true; 
    }
    return false; 
}

void Snake::grow()
{ 
    auto tail = mSegments[mSegments.size()-1];  

    Segment* seg = new Segment(CELL_SIZE, CELL_SIZE); 
    
    int seg_x, seg_y; 

    switch (tail->direction())
    {
    case Segment::DIRECTION::UP:
        seg_x = tail->x();
        seg_y = tail->y() - CELL_SIZE;
        break;
    case Segment::DIRECTION::DOWN: 
        seg_x = tail->x();
        seg_y = tail->y() + CELL_SIZE;
        break;
    case Segment::DIRECTION::LEFT: 
        seg_x = tail->x() + CELL_SIZE;
        seg_y = tail->y();
        break;
    case Segment::DIRECTION::RIGHT: 
        seg_x = tail->x() - CELL_SIZE;
        seg_y = tail->y();
        break;
    default:
        break;
    }


    seg->setPosition(seg_x, seg_y);
    seg->setColor(sf::Color::Green); 

    mSegments.push_back(seg);

}

void Snake::reset()
{
    // delete the pointers to each segment in the vector
    for(auto seg : mSegments)
    {
        delete seg; 
    }

    // re-init the starting size of the snake
    mNumSegments = 3;
    mSegments.clear(); 
    
    // re-init the snake 
    init(GRID_X, GRID_Y); 
}