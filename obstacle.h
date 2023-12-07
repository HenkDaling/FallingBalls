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
    //precondition: nothing
    //postcondition: object is created
    //return type: object
    //description of function: default constructor
    obstacle();

    //precondition: nothing
    //postcondition: object is destroyed
    //return type: nothing
    //description of function: destroys obstacle object
    ~obstacle();

    //precondition: integer
    //postcondition: changes count to passed number
    //return type: nothing
    //description of function: sets count to a integer
    void setCount(int num);

    //precondition: color
    //postcondition: color is changed to passed color
    //return type: nothing
    //description of function: changes color to a new color
    void setColor(color newColor);

    //precondition: nothing
    //postcondition: returns count
    //return type: integer
    //description of function: gets count
    int getCount() const;

    //precondition: nothing
    //postcondition: returns true or false
    //return type: bool
    //description of function: determines if object has died if count <= 0
    bool isDead() const;

    //precondition: nothing
    //postcondition: returns true if y < 0 otherwise false
    //return type: bool
    //description of function: determines if obstacle is at top of screen
    bool isTop() const;

    //precondition: SDL_Plotter object
    //postcondition: object is drawn
    //return type: nothing
    //description of function: draws obstacle on screen
    void drawObstacle(SDL_Plotter &g);

    //precondition: object is passed
    //postcondition: count is decreased on obstacle
    //return type: nothing
    //description of function: counts number of collisions
    void handleCollision(Object& other) override;

private:
    int index;
    int count;
    color c;
};

#endif
