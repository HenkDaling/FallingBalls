#include "polygon.h"
#include <cmath>

polygon::polygon(){
    poly.clear();                
};

polygon::polygon(vector<point> points, point pos){
    poly = points;
    position = pos;
}

polygon::~polygon(){

}

void polygon::updatePosition(const point &pos){
    point diff;
    diff = pos - position;

    for(point &p : poly){
        p = p+diff;
    }

    position = pos;
}

void polygon::rotate(double angleInRadians, const point & pivot)
{
    
     for (size_t i = 0; i < poly.size(); ++i) {
        poly.at(i) = Vector2D::rotate(angleInRadians, poly.at(i), pivot);
    }
}

void polygon::addPoint(point p){
    poly.push_back(p);
}

void polygon::removePoint(point p){
    int i = 0;
    for(point x : poly){
        if(x==p){
            poly.erase(poly.begin() + i);
        }
        i++;
    }
}

const vector<point>& polygon::getPoints() const {
    return poly;
}

point polygon::calculateCenter(){
    int numPoints = poly.size();
    double sumX = 0.0, sumY = 0.0;

    for (const point& point : poly) {
        sumX += point.x;
        sumY += point.y;
    }

    return point(sumX / numPoints, sumY / numPoints);
}

polygon polygon::makeEquilateralTriangle(point midpoint, double side){
    polygon pol;
    pol.poly.clear();

    point p1, p2, p3;
    double h;

    h = (side/2)* tan(45);

    p1.x = midpoint.x;
    p1.y = midpoint.y + 0.5*h;

    p2.x = midpoint.x- 0.5*side;
    p2.y = midpoint.y - 0.5*h;

    p3.x = midpoint.x+ 0.5*side;
    p3.y = midpoint.y - 0.5*h;

    pol.poly = vector<point>{p1, p2, p3};

    return pol;

}

polygon polygon::makeCornerSquare(point corner, double width, double height){
    polygon pol;
    pol.poly.clear();

    point p1,p2,p3,p4;

    p1 = corner;

    p2 = corner;
    p2.x += width;

    p3 = p2;
    p3.y -= height;

    p4 = corner;
    p4.y -= height;

    pol.poly = vector<point>{p1,p2,p3,p4};

    return pol;

}

polygon polygon::makeCenterSquare(point midpoint, double width, double height){
    polygon pol;
    pol.poly.clear();

    point p1,p2,p3,p4;
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

    pol.poly = vector<point>{p1,p2,p3,p4};
    return pol;
}

polygon polygon::makeCircle(point midpoint, double radius, unsigned int resolution){
    polygon pol;

    double angle = 2 * M_PI / static_cast<double>(resolution); 
    double theta = 0;
    double x;
    double y;
    point p;

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
    vector<point> ipoints;

    angle = Vector2D();

    for (const point& vertex : other.getPoints()) {

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
        
        for (const point& vertex : getPoints()) {
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

bool polygon::intersects(const point& p) const {

    //reference : https://en.wikipedia.org/wiki/Point_in_polygon
    
    int intersectCount = 0;
    const std::vector<point>& vertices = getPoints();
    size_t numVertices = vertices.size();

    for (size_t i = 0; i < numVertices; ++i) {
        const point& v1 = vertices[i];
        const point& v2 = vertices[(i + 1) % numVertices];

        if ((v1.y <= p.y && p.y < v2.y) || (v2.y <= p.y && p.y < v1.y)) {
           
            double xIntersection = (v2.x - v1.x) * (p.y - v1.y) / (v2.y - v1.y) + v1.x;

            if (p.x < xIntersection) {
                ++intersectCount;
            }
        }
    }

    return (intersectCount % 2) == 1;
    
}


Vector2D polygon::angleOfIntersect(const point& intersectionPoint,const polygon* p) const{
    int index = p->getIndex(intersectionPoint);

    const point& vertex1 = p->getPoints()[(index + p->getPoints().size() - 1) % p->getPoints().size()]; //prev
    const point& vertex2 = p->getPoints()[(index + 1) % p->getPoints().size()]; //next

    Vector2D vector1 = Vector2D::createFromCartesian(intersectionPoint.x - vertex1.x, intersectionPoint.y - vertex1.y);
    Vector2D vector2 = Vector2D::createFromCartesian(intersectionPoint.x - vertex2.x , intersectionPoint.y - vertex2.y );
    
    Vector2D normal =  Vector2D::createFromCartesian(vector1.crossProduct(vector2), vector2.crossProduct(vector1));
    normal = vector1 + vector2;
    
    normal = normal * (-1.0);

    normal.normalize();
    return normal;
}

int polygon::getIndex(const point& p) const {
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