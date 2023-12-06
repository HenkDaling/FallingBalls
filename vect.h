#ifndef VECT_H
#define VECT_H

#include <cmath>
#include <cstdint>

using namespace std;

//point2D
struct point2D{
   double x,y;

   point2D(const point2D& p){
       *this = p;
   }

   point2D(const double x = 0, const double y = 0){
      this->x = x;
      this->y = y;
   }

   point2D& operator= (const point2D& p){
       x = p.x;
       y = p.y;
       return *this;
   }

    point2D operator-(const point2D& p) const {
        point2D rp;
        rp.x = x - p.x;
        rp.y = y - p.y;
        return rp;
    }

    point2D operator+(const point2D& p) const {
        point2D rp;
        rp.x = x + p.x;
        rp.y = y + p.y;
        return rp;
    }

   bool operator== (const point2D& p){
        return x==p.x && y==p.y;
   }

   bool operator==(const point2D &p) const { 
        return x == p.x && y == p.y;
    }

    
   double distance(point2D p) const{
       return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
   }
};


class Vector2D {
public:
    // Constructors
    Vector2D();
    Vector2D(double magnitude, double angleInRadians);
    Vector2D(const Vector2D& other);

    //Create methods

    static Vector2D createFromCartesian(double x, double y);
    static Vector2D createFromPolar(double r, double A);

    // Destructor
    ~Vector2D();

    // Getter methods
    double getX() const;
    double getY() const;
    double getMagnitude() const;
    double getAngle() const;

    // Vector operations
    double dotProduct(const Vector2D& other) const;
    double crossProduct(const Vector2D& other) const;
    Vector2D vectorAddition(const Vector2D& other) const;
    double angleBetween(const Vector2D& other) const;
    Vector2D perpendicular() const;
    void normalize();

    static point2D rotate(double angleInRadians,const point2D &p, const point2D &pivot);

    // Overloaded operators
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    double operator*(const Vector2D& other) const; // Dot product
    Vector2D operator*(double scalar) const; //Scalar multipy
    Vector2D operator*(uint64_t scalar) const; //Scalar multiply

    double operator^(const Vector2D& other) const; // Cross product
    bool operator==(const Vector2D& other) const;

private:
    double x;
    double y;

};

#endif // VECT_H
