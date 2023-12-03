#include "Object.h"
#include "draw.h"
#include <sstream>
#include <iomanip>
#include "SDL_Plotter.h"

Object::Object()
{
    IsVissable = false;
}

Object::Object(Image &im){
    IsVissable = true;
    position.x = 10;
    position.y = 10;
    img = im;
}

Object::~Object()
{

}

void Object::hide(){
    IsVissable = false;
}

void Object::show(){
    IsVissable = true;
}

void Object::updatePosition(const point &pos){
    this->position = pos;
    boundry.updatePosition(pos);
}

void Object::rotate(double angleInRadians, const point & pivot)
{
    boundry.rotate(angleInRadians,pivot);
}

point Object::getPosition() const {
    return position;
}

polygon Object::getBoundry() const{
    return boundry;
}

void Object::setLocation(const point p){
    updatePosition(p);
}

void Object::setBoundry(polygon p){
    boundry = p;
}

void Object::setText(const string text)
{
    this->text = text;
}

void Object::setTextLocation(point & p)
{
    textPosition = p;
}

void Object::drawImage(SDL_Plotter &g, double angle  , point pivot ){
    draw::drawImagePixels(position, img, g, angle, pivot);
}

void Object::drawText(SDL_Plotter &g)
{
    draw::drawText(textPosition , this->text, g, this->Theta_rotation, this->getPosition());
    
}

void Object::drawObject(SDL_Plotter &g, drawMode mode, color c){

    if(IsVissable){
        
        switch (mode) {
            case draw_image: 
                drawImage(g);
                break;
            case draw_polygon_fill:      
                draw::drawFillPolygon(boundry.getPoints(),g,c);
                break;
            case draw_polygon_outline:      
                draw::drawOutline(boundry.getPoints(),g,c);
                break;
            case draw_image_and_polygon:
                drawImage(g);
                //boundry.updatePosition(upd);
                draw::drawOutline(boundry.getPoints(),g,c);
                break;            
            default:
                break;
        }
        
    }
}

void Object::handleCollision(Object& other){
    cout << "collision handler object" << endl;
}