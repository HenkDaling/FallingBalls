/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#ifndef POLYGON_H
#define POLYGON_H

#include "SDL_Plotter.h"
#include "vect.h"

class polygon
{
private:
    vector<point2D> poly;
    point2D position;

    //precondition: a point
    //postcondition: returns the index of point
    //return type: integer
    //description of function: finds point in vector poly
    int getIndex(const point2D& p) const;

    public :

    //precondition: nothing
    //postcondition: vector is cleared
    //return type: vector
    //description of function: clears a vector
    polygon();

    //precondition: vector and a point
    //postcondition: returns new polygon
    //return type: object
    //description of function: constructor of polygon
    polygon(vector<point2D> points, point2D pos);

    //precondition: nothing
    //postcondition: object is destroyed
    //return type: nothing
    //description of function: destroys polygon object
    ~polygon();

    //precondition: a point
    //postcondition: point is added to vector
    //return type: nothing
    //description of function: adds point to vector
    void addPoint(point2D p);

    //precondition: a point
    //postcondition: the point is removed from poly
    //return type: nothing
    //description of function: removes a point from the vector
    void removePoint(point2D p);

    //precondition: a new point
    //postcondition: changes position point
    //return type: nothing
    //description of function: changes position of point
    void updatePosition(const point2D &pos);

    //precondition: angle and pivot point
    //postcondition: rotates all points in poly
    //return type: nothing
    //description of function: changes direction a polygon is facing
    void rotate(double angleInRadians, const point2D &pivot);

    //precondition: nothing
    //postcondition: returns the vector poly
    //return type: vector
    //description of function: returns points of a polygon
    const std::vector<point2D>& getPoints() const;

    //precondition: nothing
    //postcondition: returns center of all points
    //return type: point
    //description of function: calculate center of polygon
    point2D calculateCenter();

    //precondition: midpoint and side
    //postcondition: returns a new object with new points
    //return type: object
    //description of function: creates an equilateral triangle polygon object
    static polygon makeEquilateralTriangle(point2D midpoint, double side);

    //precondition: corner, width, and height
    //postcondition: returns a new object with new points
    //return type: object
    //description of function: creates a square from the corners
    static polygon makeCornerSquare(point2D corner, double width, double height);

    //precondition: midpoint, width, height
    //postcondition: returns a new object with points
    //return type: object
    //description of function: creates a square from the midpoint
    static polygon makeCenterSquare(point2D midpoint, double width, double height);

    //precondition: midpoint, radius, and resolution
    //postcondition: returns a new object with new points
    //return type: object
    //description of function: creates a circle polygon object
    static polygon makeCircle(point2D midpoint, double radius, unsigned int resolution);

    //precondition: a polygon, and an angle
    //postcondition: returns true or false
    //return type: bool
    //description of function: checks if polygons intersect
    bool intersects(const polygon& other, Vector2D& angle);

    //precondition: point
    //postcondition: returns true or false
    //return type: bool
    //description of function: checks if vertices intersect
    bool intersects(const point2D& p) const;

    //precondition: intersection point, polygon
    //postcondition: returns vector after intersection
    //return type: Vector2D object
    //description of function: calculates the vector after polygons intersect
    Vector2D angleOfIntersect(const point2D &intersectionPoint, const polygon *p) const;

    //precondition: a polygon
    //postcondition: polygon vector is changed to passed polygon vector
    //return type: poly
    //description of function: Makes one polygon equal to another
    polygon& operator=(const polygon& p);


};

#endif

