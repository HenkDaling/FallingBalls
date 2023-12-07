/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#ifndef OBJECT_H
#define OBJECT_H

#include "SDL_Plotter.h"
#include "image.h"
#include "polygon.h"
#include "draw.h"

enum drawMode{
    draw_image,
    draw_polygon_fill,
    draw_polygon_outline,
    draw_image_and_polygon
};

class Object
{
private:
    point2D position;
    point2D textPosition;
    double Theta_rotation;
protected:
    Image img;
    polygon boundry;
    double CollisionAngle;
    bool IsVissable;
    string text;

public:
    //precondition: nothing
    //postcondition: object is created
    //return type: nothing
    //description of function: default constructor
    Object();

    //precondition: nothing
    //postcondition: object is created as an image
    //return type: nothing
    //description of function: creates object with image components
    Object(Image &im);

    //precondition: nothing
    //postcondition: object is destroyed
    //return type: nothing
    //description of function: deletes an object
    ~Object();

    //precondition: nothing
    //postcondition: object is not visible
    //return type: bool
    //description of function: hides object
    void hide();

    //precondition: nothing
    //postcondition: object is visible
    //return type: bool
    //description of function: shows object
    void show();

    //precondition: position point
    //postcondition: position is assigned with pos
    //return type: nothing
    //description of function: changes position of object

    bool isVisable();

    void updatePosition(const point2D &pos);

    //precondition: angle and pivot point
    //postcondition: polygon is rotated
    //return type: nothing
    //description of function: rotates polygon boundry
    void rotate(double angleInRadians, const point2D &pivot);

    //precondition: nothing
    //postcondition: returns point of object
    //return type: point
    //description of function: position is retrieved using the point
    point2D getPosition() const;

    //precondition: nothing
    //postcondition: returns boundry of object
    //return type: polygon
    //description of function: boundry is received
    polygon getBoundry() const;

    //precondition: point
    //postcondition: position is updated
    //return type: nothing
    //description of function: updates point of position
    void setLocation(const point2D p);

    //precondition: polygon
    //postcondition: boundry is updated
    //return type: nothing
    //description of function: updates boundry with new polygon
    void setBoundry(polygon p);

    //precondition: point
    //postcondition: text position is updated
    //return type: nothing
    //description of function: changes where text is located
    void setTextLocation(point2D &p);

    //precondition: SDL_Plotter object, pivot point, color
    //postcondition: calls drawImagePixels
    //return type: nothing
    //description of function: draws the object
    void drawImage(SDL_Plotter &g, double angle = 0 , point2D pivot = point2D(), const color &DontDraw = color(255,0,255) );

    //precondition: SDL_Plotter object, drawMode, and color
    //postcondition: displays object to plotter
    //return type: nothing
    //description of function: calls draw functions
    void drawObject(SDL_Plotter &g,drawMode mode, color = color());


    //precondition: object
    //postcondition: prints statement
    //return type: nothing
    //description of function: created to be used for handling other objects
    virtual void handleCollision(Object& other); //so it can be overrided for different types of object


    //precondition: point
    //postcondition: returns true if boundries intersect
    //return type: bool
    //description of function: checks if objects are inside one another
    bool isInside(const point2D &p){
        //cout << p.x << " " << p.y <<endl;
        return boundry.intersects(p);
    }


    //precondition: object
    //postcondition: returns true if objects are colliding
    //return type: bool
    //description of function: checks if objects are touching
    bool isColiding(Object& other){
        Vector2D v;
        if(boundry.intersects(other.getBoundry(), v)){
            CollisionAngle = v.getAngle();
            handleCollision(other);
            }
        return boundry.intersects(other.getBoundry(), v);
    }

};

#endif
