/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#ifndef BALL_H
#define BALL_H

#include "Object.h"
#include "force.h"
#include "images/beachBall.c"
#include "vect.h"

#include <cmath>
#include <chrono>

const static struct {
    double right = 0.0;
    double up = 0.5 * M_PI;
    double left = M_PI;
    double down = (3.0/2.0) * M_PI;

} directions;


class ball : public Object
{
private:
    double mass = 1.0;
    Vector2D netForce;
    Vector2D velocity;
    Vector2D acceleration;
    uint64_t time;

public:
    ball();
    ball(point2D pos,Vector2D force);
    ~ball();

    void applyForce(Vector2D F);
    void setForce(Vector2D F);

    void giveVelocity(Vector2D V);
    void update(uint64_t deltaTime);

    using Object::handleCollision;
    void handleCollision(Object& other) override;
    void handleContCollisions(Object& other);

    void display(std::ostream& os) const;

};

#endif
