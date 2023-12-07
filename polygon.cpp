/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#include "polygon.h"
#include <cmath>

polygon::polygon(){
    poly.clear();
};

polygon::polygon(vector<point2D> points, point2D pos){
    poly = points;
    position = pos;
}

polygon::~polygon(){

}

void polygon::updatePosition(const point2D &pos){
    point2D diff;
    diff = pos - position;

    for(point2D &p : poly){
        p = p+diff;
    }

    position = pos;
}

void polygon::rotate(double angleInRadians, const point2D & pivot)
{

     for (size_t i = 0; i < poly.size(); ++i) {
        poly.at(i) = Vector2D::rotate(angleInRadians, poly.at(i), pivot);
    }
}

void polygon::addPoint(point2D p){
    poly.push_back(p);
}

void polygon::removePoint(point2D p){
    int i = 0;
    for(point2D x : poly){
        if(x==p){
            poly.erase(poly.begin() + i);
        }
        i++;
    }
}

const vector<point2D>& polygon::getPoints() const {
    return poly;
}

point2D polygon::calculateCenter(){
    int numPoints = poly.size();
    double sumX = 0.0, sumY = 0.0;

    for (const point2D& point : poly) {
        sumX += point.x;
        sumY += point.y;
    }

    return point2D(sumX / numPoints, sumY / numPoints);
}

polygon polygon::makeEquilateralTriangle(point2D midpoint, double side){
    polygon pol;
    pol.poly.clear();

    point2D p1, p2, p3;
    double h;

    h = (side/2)* tan(45);

    p1.x = midpoint.x;
    p1.y = midpoint.y + 0.5*h;

    p2.x = midpoint.x- 0.5*side;
    p2.y = midpoint.y - 0.5*h;

    p3.x = midpoint.x+ 0.5*side;
    p3.y = midpoint.y - 0.5*h;

    pol.poly = vector<point2D>{p1, p2, p3};

    return pol;

}

polygon polygon::makeCornerSquare(point2D corner, double width, double height){
    polygon pol;
    pol.poly.clear();

    point2D p1,p2,p3,p4;

    p1 = corner;

    p2 = corner;
    p2.x += width;

    p3 = p2;
    p3.y -= height;

    p4 = corner;
    p4.y -= height;

    pol.poly = vector<point2D>{p1,p2,p3,p4};

    return pol;

}

polygon polygon::makeCenterSquare(point2D midpoint, double width, double height){
    polygon pol;
    pol.poly.clear();

    point2D p1,p2,p3,p4;
    double w,h;

    w = 0.5*width;
    h = 0.5*height;

    p1.x = midpoint.x - w;
    p1.y = midpoint.y + h;

    p2.x = midpoint.x + w;
    p2.y = midpoint.y + h;

    p3.x = midpoint.x + w;
    p3.y = midpoint.y - h;

    p4.x = midpoint.x - w;
    p4.y = midpoint.y - h;

    pol.poly = vector<point2D>{p1,p2,p3,p4};
    return pol;
}

polygon polygon::makeCircle(point2D midpoint, double radius, unsigned int resolution){
    polygon pol;

    double angle = 2 * M_PI / static_cast<double>(resolution);
    double theta = 0;
    double x;
    double y;
    point2D p;

    for (unsigned int i = 0; i < resolution; i++) {
        theta = angle * i;
        x = radius * sin(theta);
        y = radius * cos(theta);

        p.x = midpoint.x + x;
        p.y = midpoint.y + y;

        pol.poly.push_back(p);
    }

    return pol;
}

bool polygon::intersects(const polygon& other, Vector2D &angle) {

    bool intersect = false;
    int intersectCount = 0;
    vector<point2D> ipoints;

    angle = Vector2D();

    for (const point2D& vertex : other.getPoints()) {

        if (this->intersects(vertex)) {
            intersect = true;

            if(intersectCount == 0){
                angle = angleOfIntersect(vertex,&other) * -1.0 ;
            }
            else{
                angle = angle + angleOfIntersect(vertex,&other) * -1.0;
            }
            intersectCount++;
        }

    }


    if(!intersect){

        for (const point2D& vertex : getPoints()) {
            if (other.intersects(vertex)) {
                intersect = true;
                if(intersectCount == 0){
                    angle = angleOfIntersect(vertex,this) ;
                }else{
                    angle = angle + angleOfIntersect(vertex,this) ;
                }
                intersectCount++;
            }
        }

    }


    return intersect;
}

bool polygon::intersects(const point2D& p) const {

    //reference : https://en.wikipedia.org/wiki/Point_in_polygon

    int intersectCount = 0;
    const vector<point2D>& vertices = getPoints();
    size_t numVertices = vertices.size();

    for (size_t i = 0; i < numVertices; ++i) {
        const point2D& v1 = vertices[i];
        const point2D& v2 = vertices[(i + 1) % numVertices];

        if ((v1.y <= p.y && p.y < v2.y) || (v2.y <= p.y && p.y < v1.y)) {

            double xIntersection = (v2.x - v1.x) * (p.y - v1.y) / (v2.y - v1.y) + v1.x;

            if (p.x < xIntersection) {
                ++intersectCount;
            }
        }
    }

    return (intersectCount % 2) == 1;

}


Vector2D polygon::angleOfIntersect(const point2D& intersectionPoint,const polygon* p) const{
    int index = p->getIndex(intersectionPoint);

    const point2D& vertex1 = p->getPoints()[(index + p->getPoints().size() - 1) % p->getPoints().size()]; //prev
    const point2D& vertex2 = p->getPoints()[(index + 1) % p->getPoints().size()]; //next

    Vector2D vector1 = Vector2D::createFromCartesian(intersectionPoint.x - vertex1.x, intersectionPoint.y - vertex1.y);
    Vector2D vector2 = Vector2D::createFromCartesian(intersectionPoint.x - vertex2.x , intersectionPoint.y - vertex2.y );

    Vector2D normal =  Vector2D::createFromCartesian(vector1.crossProduct(vector2), vector2.crossProduct(vector1));
    normal = vector1 + vector2;

    normal = normal * (-1.0);

    normal.normalize();
    return normal;
}

int polygon::getIndex(const point2D& p) const {
    int r = -1;
    for (int i = 0; i < poly.size(); ++i) {
        if (poly[i] == p) {
            r = i;
        }
    }
    return r;
}

polygon& polygon::operator=(const polygon& p) {
        poly = p.getPoints();
        return *this;
    };
