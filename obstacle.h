/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

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
