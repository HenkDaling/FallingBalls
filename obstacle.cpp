#include "obstacle.h"
#include <cmath>

obstacle::obstacle()
{
    

}

obstacle::~obstacle()
{

}

void obstacle::setCount(int num)
{
    count = num;
}

void obstacle::setColor(color newColor)
{
    c = newColor;
}

int obstacle::getCount() const{
    return count;
}

bool obstacle::isDead() const
{
    if(count <= 0){
        return true;
    }
    else{
        return false;
    }

}

bool obstacle::isTop() const
{
    if(getPosition().y < 0){
        return true;
    }
    else{
        return false;
    }
}

void obstacle::drawObstacle(SDL_Plotter &g)
{
    if(IsVissable){
        this->drawObject(g,draw_polygon_fill,c);
        this->drawObject(g,draw_polygon_outline, c);

        this->setText(to_string(count));
        
        drawText(g);
    }
}

void obstacle::handleCollision(Object& other){
    cout << "collision handler obstacle" ;
    count--;
};

