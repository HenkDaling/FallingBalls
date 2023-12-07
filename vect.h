/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#ifndef VECT_H
#define VECT_H

#include <cmath>
#include <cstdint>

using namespace std;

//point2D
struct point2D{
   double x,y;

   //precondition: point
   //postcondition: redefines a point obj
   //return type: nothing
   //description of function: redefines a point from another point
   point2D(const point2D& p){
       *this = p;
   }

   //precondition: x and y
   //postcondition: makes a point obj
   //return type: nothing
   //description of function: constructor of point
   point2D(const double x = 0, const double y = 0){
      this->x = x;
      this->y = y;
   }

   //precondition: point
   //postcondition: makes a point obj
   //return type: nothing
   //description of function: makes a new point from another point
   point2D& operator= (const point2D& p){
       x = p.x;
       y = p.y;
       return *this;
   }

   //precondition: two points
   //postcondition: returns subtracted point
   //return type: point
   //description of function: subtracts one point from another
    point2D operator-(const point2D& p) const {
        point2D rp;
        rp.x = x - p.x;
        rp.y = y - p.y;
        return rp;
    }

   //precondition: two points
   //postcondition: returns added point
   //return type: point
   //description of function: adds two points together
    point2D operator+(const point2D& p) const {
        point2D rp;
        rp.x = x + p.x;
        rp.y = y + p.y;
        return rp;
    }

   //precondition: point by reference
   //postcondition: returns true or false
   //return type: bool
   //description of function: checks if points are equal
   bool operator==(const point2D &p) const {
        return x == p.x && y == p.y;
    }

   //precondition: two points
   //postcondition: returns a distance
   //return type: double
   //description of function: distance from one point to another
   double distance(point2D p) const{
       return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
   }
};


class Vector2D {
public:
    // Constructors----------

    //precondition: nothing
    //postcondition: creates an object
    //return type: nothing
    //description of function: creates a Vector2d object
    Vector2D();

    //precondition: 2 doubles
    //postcondition: creates an object
    //return type: nothing
    //description of function: creates object with passed values
    Vector2D(double magnitude, double angleInRadians);

    //precondition: Vector2D object
    //postcondition: creates an object
    //return type: nothing
    //description of function: creates an object from another obj
    Vector2D(const Vector2D& other);

    //Create methods----------

    //precondition: x and y
    //postcondition: returns an object with x and y
    //return type: object
    //description of function: uses coordinates to create an object
    static Vector2D createFromCartesian(double x, double y);

    //precondition: radius and angle
    //postcondition: returns a new object
    //return type: object
    //description of function: uses polar coordinate system to create
             //a new object
    static Vector2D createFromPolar(double r, double A);

    // Destructor----------

    //precondition: an object
    //postcondition: object is destroyed
    //return type: nothing
    //description of function: destroys an object
    ~Vector2D();

    // Getter methods----------

    //precondition: an object
    //postcondition: returns x value
    //return type: double
    //description of function: gets x value from object
    double getX() const;

    //precondition: an object
    //postcondition: returns y value
    //return type: double
    //description of function: gets y value from object
    double getY() const;

    //precondition: an object
    //postcondition: returns magnitude
    //return type: double
    //description of function: calculates and returns magnitude
    double getMagnitude() const;

    //precondition: an object
    //postcondition: returns the angle
    //return type: double
    //description of function: calculates and returns the angle
    double getAngle() const;

    // Vector operations----------

    //precondition: 2 objects
    //postcondition: returns double that is dot product
    //return type: double
    //description of function: calculates and returns dot product
    double dotProduct(const Vector2D& other) const;

    //precondition: 2 objects
    //postcondition: returns double cross product
    //return type: double
    //description of function: calculate and return cross product
    double crossProduct(const Vector2D& other) const;

    //precondition: 2 objects
    //postcondition: return a new added object
    //return type: object
    //description of function: adds two objects together using cartesian function
    Vector2D vectorAddition(const Vector2D& other) const;

    //precondition: 2 objects
    //postcondition: returns angle
    //return type: double
    //description of function: finds angle between objects
    double angleBetween(const Vector2D& other) const;

    //precondition: an object
    //postcondition: returns a perpendicular object
    //return type: object
    //description of function: creates an object that is perpendicular to original
    Vector2D perpendicular() const;

    //precondition: nothing
    //postcondition: vector length is changed
    //return type: nothing
    //description of function: Changes vector length to one unit
    void normalize();

    //precondition: an angle, two points, and pivot point
    //postcondition: returns rotated point
    //return type: point
    //description of function: Changes direction of vector
    static point2D rotate(double angleInRadians,const point2D &p, const point2D &pivot);

    // Overloaded operators----------

    //precondition: 2 objects
    //postcondition: returns added vector
    //return type: object
    //description of function: adds to vectors together
    Vector2D operator+(const Vector2D& other) const;

    //precondition: 2 objects
    //postcondition: returns subtracted vector
    //return type: object
    //description of function: subtracts one vector from another
    Vector2D operator-(const Vector2D& other) const;

    //precondition: 2 vectors
    //postcondition: returns dot product
    //return type: double
    //description of function: overloaded operator for dot product
    double operator*(const Vector2D& other) const; // Dot product

    //precondition: scalar double
    //postcondition: returns new object
    //return type: object
    //description of function: makes a new object multiplied by scalar
    Vector2D operator*(double scalar) const; //Scalar multiply

    //precondition: 64-bit scalar
    //postcondition: returns new object
    //return type: object
    //description of function: makes a new object multiplied by scalar
    Vector2D operator*(uint64_t scalar) const; //Scalar multiply

    //precondition: 2 objects
    //postcondition: returns cross product
    //return type: double
    //description of function: overloaded operator for cross product
    double operator^(const Vector2D& other) const; // Cross product

    //precondition: 2 objects
    //postcondition: returns true or false
    //return type: bool
    //description of function: checks if objects are equal
    bool operator==(const Vector2D& other) const;

private:
    double x;
    double y;

};

#endif // VECT_H
