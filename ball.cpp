/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#include "ball.h"

using namespace std::chrono;

ball::ball()
{
    this->updatePosition(point2D(0,0));
    this->netForce = Vector2D::createFromCartesian(0,0);
    this->acceleration = Vector2D::createFromCartesian(0,0);
    time = 0;
}

ball::ball(point2D pos,Vector2D force)
{
    this->updatePosition(pos);
    mass = 1;

    netForce = force;

    acceleration =  Vector2D(netForce.getMagnitude() / mass, netForce.getAngle());
    velocity = Vector2D::createFromCartesian(0,0);

    img = Image(beachBall);
    time =  0;
}

ball::~ball()
{

}

void ball::update(uint64_t deltaTime) {
    uint64_t t =  deltaTime;
    point2D pos = this->getPosition();

    deltaTime = deltaTime - time;

    double timeFactor = 10;

    acceleration = Vector2D(netForce.getMagnitude() / mass, netForce.getAngle());

    velocity = velocity + (acceleration * (deltaTime / timeFactor));

    // Update position using the equations of motion
    pos.x += velocity.getX() * (deltaTime / timeFactor);
    pos.y += velocity.getY() * (deltaTime / timeFactor);

    this->updatePosition(pos);

    time = t;
}

void ball::handleCollision(Object& other)  {
    handleContCollisions(other);
}


void ball::handleContCollisions(Object& other) {

        cout << "collision handler ball" << endl;

        point2D pos = this->getPosition();

        Vector2D separationVector = Vector2D::createFromPolar(1.0, CollisionAngle);

        double separationDistance = 3.0;

        pos.x += separationVector.getX() * separationDistance;
        pos.y += separationVector.getY() * separationDistance;

        this->updatePosition(pos);

        Vector2D normalVector = Vector2D::createFromPolar(1.0, CollisionAngle);
        normalVector = normalVector  ;

        // Calculate the dot product of velocity and the normal vector
        double dotProduct = velocity.dotProduct(normalVector);

        // Calculate the reflection using the reflection formula
        Vector2D reflection = velocity - (normalVector * (dotProduct * 2.0));

        // Update the velocity of the ball
        velocity = reflection * 1.0 ;

}

void ball::setForce(Vector2D F)
{
    netForce = F;
}


void ball::display(std::ostream& os) const {
        point2D position = getPosition();
        os << "Position: (" << position.x << ", " << position.y << ")" << std::endl;
        os << "Velocity: (" << velocity.getX() << ", " << velocity.getY() << ")" << std::endl;
        os << "Acceleration: (" << acceleration.getX() << ", " << acceleration.getY() << ")" << std::endl;
    }


void ball::applyForce(Vector2D F){
    netForce = netForce + F;
}

void ball::giveVelocity(Vector2D V)
{
    velocity = V;
}

