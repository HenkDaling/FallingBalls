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
    force netForce;
    Vector2D velocity;
    Vector2D acceleration;
    uint64_t time;

    //void calculateAcceleration();


public:
    ball(point pos,double mass);
    ~ball();

    void applyForce(force F);
    void update(uint64_t deltaTime);

    using Object::handleCollision;
    void handleCollision(Object& other) override;
    void handleContCollisions(Object& other);

    void display(std::ostream& os) const;

};

#endif