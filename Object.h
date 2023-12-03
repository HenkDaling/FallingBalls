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
    point position;
    point textPosition;
    double Theta_rotation;
protected:
    Image img;
    polygon boundry;    
    double CollisionAngle;
    bool IsVissable;
    string text;

public:
    Object();
    Object(Image &im);
    ~Object();

    void hide();
    void show();

    void updatePosition(const point &pos);
    void rotate(double angleInRadians, const point &pivot);

    point getPosition() const;
    polygon getBoundry() const;
    string getText() const;

    void setLocation(const point p);
    void setBoundry(polygon p);
    void setText(const string text);
    void setTextLocation(point &p);

    void drawImage(SDL_Plotter &g, double angle = 0 , point pivot = point());
    void drawPolygon(){};
    void drawObject(SDL_Plotter &g,drawMode mode, color = color());
    void drawText(SDL_Plotter &g);

    virtual void handleCollision(Object& other); //so it can be overided for different types of object


    bool isInside(point &p){
        //cout << p.x << " " << p.y <<endl;
        return boundry.intersects(p);
    }

    

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
