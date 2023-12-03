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

		newO.updatePosition(point(randomX, randomY));

		int shape = rand() % 3;

		switch(shape){
			case 0: 
				 newO.setBoundry(polygon::makeCenterSquare(newO.getPosition(), 50, 50)); // Adjust size as needed
				 break;
			case 1: 
				newO.setBoundry(polygon::makeCircle(newO.getPosition(), 25,30)); // Adjust size as needed
				 break;
			case 2: 
				newO.setBoundry(polygon::makeEquilateralTriangle(newO.getPosition(), 70)); // Adjust size as needed
				 break;

		}

		newO.show();
		newO.setCount(level + rand() % level / 2);

		newO.setColor(color(R,G,B, 255));

		o.push_back(newO);
	}
		
}

void moveUp(vector<obstacle> &o){
	point pos;

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
	point mouseClick;

	startButton.setText("START");
	startText.setText("Hello worls");
	point buttonLoc(330, 340);
	point textLoc = buttonLoc + point(60,30);
	startButton.setLocation(buttonLoc);
	startButton.setTextLocation(textLoc);
	startButton.setBoundry(polygon::makeCornerSquare(point(330,350),200,-80));

	if(g.mouseClick()){
			//p = g.getMouseClick();
			cout<< "mouseKclik" << endl;
		}

	if(g.mouseClick()){
		m_point mouse = g.getMouseClick();
		mouseClick.x = mouse.x;		
		mouseClick.y = mouse.y;	

		if(startButton.isInside(mouseClick)){
			startGame = true;
		}
	}

	static bool init = false;
	if(!init){
		g.plotImage(0,0, startImage);

		startButton.drawObject(g,draw_image);
		startButton.drawObject(g,draw_polygon_outline,color(255,0,255,255));
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
    m_point p;
    color c;
    int size;
	Image button(transparentTest);

	ball b(point(100.0,100.0), 1);
	b.setBoundry(polygon::makeCircle(b.getPosition() + point(25,25), 30,30));
	b.show();

	Object box;
	box.setLocation(point(0,790));
	box.setBoundry(polygon::makeCornerSquare(box.getPosition(),800,40));
	box.show();

	Object wallLeft;
	wallLeft.setLocation(point(0.0,800));
	wallLeft.setBoundry(polygon::makeCornerSquare(wallLeft.getPosition(),40,800));
	wallLeft.show();

	Object wallRight;
	wallRight.setLocation(point(800,800));
	wallRight.setBoundry(polygon::makeCornerSquare(wallRight.getPosition(),40,800));
	wallRight.show();

	vector<Object> walls;

	Image bgI = Image(backgroundIMG);
	Object background(bgI);
	background.setLocation(point(40,0));
	background.show();

	walls.push_back(wallLeft);
	walls.push_back(wallRight);
	walls.push_back(box);

	uint64_t time = 0;

	double a = 0;
	uint64_t s5 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();


	vector<obstacle> obst; 
	vector<obstacle> obst2;
	bool startGame = false;
	generateLevel(1, obst);

    while (!g.getQuit())
    {

		for (size_t i = 0; i < obst.size(); i++) {

			if (obst.at(i).isDead()) {
                // Remove the dead obstacle from the vecto
                obst.at(i).hide();
            } 

		}
 
		if(g.kbhit()){
			switch(toupper(g.getKey())){
				case 'C': g.clear();
				          break;
			}
		}

		if(!startGame){
			startGame =drawStartScreen(g);
		}
		if(startGame){

			g.plotImage(40,0, bgI);
			dt = dt + 5;


			for(Object wall : walls){
				wall.drawObject(g,draw_polygon_fill,color(0,200,200,255));
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

			a = 0.01;

			if( duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - s5 > 4000){
				level++;
				moveUp(obst);
				generateLevel(level, obst);
				s5 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			}


			for (size_t i = 0; i < obst.size(); i++) {
					point center;
					obst.at(i).rotate(a,obst.at(i).getPosition());
					center = obst.at(i).getPosition();
					center = center - point(10,20) ;
					obst.at(i).setTextLocation(center);
					obst.at(i).drawObstacle(g);
			}

			
			b.update(dt);
			b.drawObject(g,draw_image);
			b.drawImage(g,0.0, b.getPosition());

		}

		g.update();

		}

	return 0;
}


