#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Object.h"

class obstacle : public Object
{
public:
    obstacle();
    ~obstacle();

    void setCount(int num);
    void setColor(color newColor);
    int getCount() const;
    bool isDead() const;
    bool isTop() const;

    void drawObstacle(SDL_Plotter &g);

    

    using Object::handleCollision;
    void handleCollision(Object& other) override;
    
private:
    int index;
    int count;
    color c;
};

#endif