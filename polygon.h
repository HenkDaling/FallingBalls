#ifndef POLYGON_H
#define POLYGON_H

#include "SDL_Plotter.h"
#include "vect.h"

class polygon
{
private:
    vector<point2D> poly;
    point2D position;
    int getIndex(const point2D& p) const;

    public :
        
    polygon();
    polygon(vector<point2D> points, point2D pos);
    ~polygon();

    void addPoint(point2D p);
    void removePoint(point2D p);

    void updatePosition(const point2D &pos);
    void rotate(double angleInRadians, const point2D &pivot);

    const std::vector<point2D>& getPoints() const;
    point2D calculateCenter();

    static polygon makeEquilateralTriangle(point2D midpoint, double side);
    static polygon makeCornerSquare(point2D corner, double width, double height);
    static polygon makeCenterSquare(point2D midpoint, double width, double height);
    static polygon makeCircle(point2D midpoint, double radius, unsigned int resolution);

    bool intersects(const polygon& other, Vector2D& angle);
    bool intersects(const point2D& p) const;

    Vector2D angleOfIntersect(const point2D &intersectionPoint, const polygon *p) const;
    
    polygon& operator=(const polygon& p);

    
};

#endif

