#include <iostream>
#include <cmath>
#include <chrono>

#include "SDL_Plotter.h"
#include "image.h"
#include "images/dots.c"
#include "images/2703.c"
#include "images/button.c"
#include "images/background.c"
#include "images/backgroundStart.c"
#include "images/arrow.c"

#include "obstacle.h"
#include "Object.h"

#include "ball.h"

#include "draw.h"

using namespace std;
using namespace std::chrono;

void generateLevel (int level,vector<obstacle> &o ){
	int count = level + level / 2;
	for(int i = 0; (i < count ) && (i < 10); i++){
		obstacle newO;

		int randomX = rand() % 720 + 40;
        int randomY =  700 - rand() % 100 ; 
		int R = rand() % 100 + 155; 
		int G = rand() % 100 + 155;
		int B = rand() % 100 + 155;

		newO.updatePosition(point2D(randomX, randomY));

		int shape = rand() % 3;

		switch(shape){
			case 0: 
				 newO.setBoundry(polygon::makeCenterSquare(newO.getPosition(), 100, 100)); 
				 break;
			case 1: 
				newO.setBoundry(polygon::makeCircle(newO.getPosition(), 50,6)); 
				 break;
			case 2: 
				newO.setBoundry(polygon::makeEquilateralTriangle(newO.getPosition(), 100)); 
				 break;

		}

		newO.show();
		newO.setCount(level + rand() % level / 2);

		newO.setColor(color(R,G,B));

		o.push_back(newO);
	}
		
}

void moveUp(vector<obstacle> &o){
	point2D pos;

	for (size_t i = 0; i < o.size(); i++) {
		
		pos = o.at(i).getPosition();
		pos.y -= 150.0;

		o.at(i).updatePosition(pos);
	}

}

bool drawStartScreen(SDL_Plotter &g){
	bool startGame = false;

	//data abstraction
	Image startImage(backgroundStart);
	Object startText;

	Image sButtonImage(ButtonImg);
	Object startButton(sButtonImage);
	point2D mouseClick;

	startButton.setText("START");
	startText.setText("Hello worls");
	point2D buttonLoc(330, 340);
	point2D textLoc = buttonLoc + point2D(60,30);
	startButton.setLocation(buttonLoc);
	startButton.setTextLocation(textLoc);
	startButton.setBoundry(polygon::makeCornerSquare(point2D(330,350),200,-80));

	if(g.mouseClick()){
		
		point mouse = g.getMouseClick();
		mouseClick.x = mouse.x;		
		mouseClick.y = mouse.y;	

		if(startButton.isInside(mouseClick)){
			startGame = true;
		}

		g.getMouseClick();
	}

	static bool init = false;
	if(!init){
		draw::drawImagePixels(point2D(0,0),startImage, g);

		startButton.drawObject(g,draw_image);
		startButton.drawText(g);
		startText.drawText(g);

		init = true;
	}
	startButton.drawText(g);

	return startGame;
}

int level = 1;



int main(int argc, char ** argv)
{

	uint64_t t = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	uint64_t dt = 0;
    SDL_Plotter g(800,840);
    point p;
    color c;
    int size;
	Image button(transparentTest);

	Vector2D force(3, directions.up);

	ball b(point2D(100.0,100.0), force);
	b.setBoundry(polygon::makeCircle(b.getPosition() + point2D(42,42), 42,30));
	b.show();

	Object box;
	box.setLocation(point2D(0,0));
	box.setBoundry(polygon::makeCornerSquare(box.getPosition(),800,10));
	box.show();

	Object wallLeft;
	wallLeft.setLocation(point2D(0.0,800));
	wallLeft.setBoundry(polygon::makeCornerSquare(wallLeft.getPosition(),10,800));
	wallLeft.show();

	Object wallRight;
	wallRight.setLocation(point2D(800,800));
	wallRight.setBoundry(polygon::makeCornerSquare(wallRight.getPosition(),10,800));
	wallRight.show();

	vector<Object> walls;

	Image bgI = Image(backgroundIMG);
	Object background(bgI);
	background.setLocation(point2D(40,0));
	background.show();

	walls.push_back(wallLeft);
	walls.push_back(wallRight);
	walls.push_back(box);

	Image arrowImg(arrow);
	Object directionArrow(arrowImg);
	directionArrow.setLocation(point2D(g.getCol()/2.0, 20));
	directionArrow.show();

	uint64_t time = 0;

	double a = 0;
	uint64_t s5 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


	vector<obstacle> obst; 
	vector<obstacle> obst2;
	bool startGame = false;
	generateLevel(1, obst);
	bool up,down = false;

    while (!g.getQuit())
    {
 
		if(g.kbhit()){
			switch(toupper(g.getKey())){
				case 'C': g.clear();
				          break;
			}
		}

		if(!startGame){
			startGame = drawStartScreen(g);
		}

	
		if(startGame){

			a = 0.001;

			for (size_t i = 0; i < obst.size(); i++) {

				if (obst.at(i).isDead()) {
					// Remove the dead obstacle from the vecto
					obst.at(i).hide();
            } 

			}

			g.clear();
			dt = dt + 1;

			b.update(dt);

			//Draw direction arrow
			int mouseX, mouseY;
			Vector2D direction;
			point2D arrowPos;

			arrowPos = directionArrow.getPosition();
			arrowPos.y = arrowPos.y + 0.5*arrow.height;

			g.getMouseLocation(mouseX, mouseY);
			direction = Vector2D::createFromCartesian( mouseX - arrowPos.x, mouseY - arrowPos.y);
			
			directionArrow.drawImage(g,direction.getAngle() ,arrowPos);
			draw::drawFillPolygon(polygon::makeCircle(arrowPos,20,20).getPoints(),g, color(255,0,0));
			//draw::drawFillPolygon(polygon::makeCircle(point2D(mouseX - arrowPos.x, mouseY - arrowPos.x),20,20).getPoints(),g, color(255,0,0));

			if(g.mouseClick()){
				g.getMouseClick();
				b.setLocation(arrowPos - point2D(beachBall.width/2, beachBall.height/2));
				b.giveVelocity(direction);
			}

			
			
			static double ang = 0;
			ang += 0.01;
			b.drawImage(g,0, b.getBoundry().calculateCenter());
			b.drawObject(g,draw_polygon_outline,color(255,0,0));

			for(Object wall : walls){
				wall.drawObject(g,draw_polygon_fill,color(0,200,200));
				b.isColiding(wall);
			}

			for (size_t i = 0; i < obst.size(); i++) {
				if(!obst.at(i).isDead()){	
					obst.at(i).isColiding(b);
					if(b.isColiding(obst.at(i))){
						obst.at(i).handleCollision(b);
					}
				}

			}

			if( duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - s5 > 4000){
				level++;
				moveUp(obst);
				generateLevel(level, obst);
				s5 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			}


			for (size_t i = 0; i < obst.size(); i++) {
					point2D center;
					obst.at(i).rotate(a,obst.at(i).getPosition());
					center = obst.at(i).getPosition();
					center = center - point2D(10,20) ;
					obst.at(i).setTextLocation(center);
					obst.at(i).drawObstacle(g);
			}


		}

		g.update();

		}

	return 0;
}


