#ifndef POLYGON_H
#define POLYGON_H

#include "SDL_Plotter.h"
#include "vect.h"

class polygon
{
private:
    vector<point> poly;
    point position;
    int getIndex(const point& p) const;

    public :
        
    polygon();
    polygon(vector<point> points, point pos);
    ~polygon();

    void addPoint(point p);
    void removePoint(point p);

    void updatePosition(const point &pos);
    void rotate(double angleInRadians, const point &pivot);

    const std::vector<point>& getPoints() const;
    point calculateCenter();

    static polygon makeEquilateralTriangle(point midpoint, double side);
    static polygon makeCornerSquare(point corner, double width, double height);
    static polygon makeCenterSquare(point midpoint, double width, double height);
    static polygon makeCircle(point midpoint, double radius, unsigned int resolution);

    bool intersects(const polygon& other, Vector2D& angle);
    bool intersects(const point& p) const;

    Vector2D angleOfIntersect(const point &intersectionPoint, const polygon *p) const;
    
    polygon& operator=(const polygon& p);

    
};

#endif

