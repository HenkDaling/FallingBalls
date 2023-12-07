/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#include "vect.h"
#include <cmath>

double M_PI = 3.14159265359;


// Constructors
Vector2D::Vector2D() : x(0.0), y(0.0) {}

Vector2D::Vector2D(double magnitude, double angleInRadians)
    : x(magnitude * cos(angleInRadians)), y(magnitude * sin(angleInRadians)) {}

Vector2D::Vector2D(const Vector2D& other) : x(other.x), y(other.y) {}


// Destructor
Vector2D::~Vector2D() {}

//creator methods

Vector2D Vector2D::createFromCartesian(double x, double y){
    Vector2D v;
    v.x = x;
    v.y = y;

    return v;
}

Vector2D Vector2D::createFromPolar(double r, double A){
    Vector2D v;

    while(A > 2 * M_PI){ A -= 2*M_PI; };
    while(A < 0) A += 2*M_PI;

    v.x = r * cos(A);
    v.y = r * sin(A);

    return v;
}

// Getter methods
double Vector2D::getX() const {
    return x;
}

double Vector2D::getY() const {
    return y;
}

double Vector2D::getMagnitude() const {
    return sqrt(x * x + y * y);
}

double Vector2D::getAngle() const {
    return atan2(y, x);
}

// Vector operations
double Vector2D::dotProduct(const Vector2D& other) const {
    return x * other.x + y * other.y;
}

double Vector2D::crossProduct(const Vector2D& other) const {
    return x * other.y - y * other.x;
}

Vector2D Vector2D::vectorAddition(const Vector2D& other) const {
    return Vector2D::createFromCartesian(x + other.x, y + other.y);
}

double Vector2D::angleBetween(const Vector2D& other) const {
    double dot = dotProduct(other);
    double magnitudes = getMagnitude() * other.getMagnitude();
    return acos(dot / magnitudes);
}

Vector2D Vector2D::perpendicular() const {
    return Vector2D(-y, x);
}

void Vector2D::normalize() {
    double magnitude = getMagnitude();

    if (magnitude != 0.0) {
        x /= magnitude;
        y /= magnitude;
    }
}

point2D Vector2D::rotate(double angleInRadians,const point2D &p, const point2D &pivot)
{
    point2D rotatedPoint;
    double tX = p.x - pivot.x;
    double tY = p.y - pivot.y;

    double rX = tX * cos(angleInRadians) - tY * sin(angleInRadians);
    double rY = tX * sin(angleInRadians) + tY * cos(angleInRadians);

    rotatedPoint.x = rX + pivot.x;
    rotatedPoint.y = rY + pivot.y;

    return rotatedPoint;
}

// Overloaded operators
Vector2D Vector2D::operator+(const Vector2D& other) const {
    return vectorAddition(other);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D::createFromCartesian(x - other.x, y - other.y);
}

double Vector2D::operator*(const Vector2D& other) const {
    return dotProduct(other);
}

Vector2D Vector2D::operator*(double scalar) const {
    return Vector2D::createFromCartesian(x * scalar, y * scalar);
}

Vector2D Vector2D::operator*(uint64_t scalar) const {
    return Vector2D::createFromCartesian(x * scalar, y * scalar);
}


double Vector2D::operator^(const Vector2D& other) const {
    return crossProduct(other);
}

bool Vector2D::operator==(const Vector2D& other) const {
    return x == other.x && y == other.y;
}
