#include "game.h"
#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std;

game::game(SDL_Plotter &g)
{
			b = ball(point2D(g.getCol()/2.0, 20), Vector2D::createFromCartesian(0,0));
			b.setLocation(point2D(g.getCol()/2.0, 20));
			cout << "Position: " << (g.getCol()/2.0) << endl;
    		b.setBoundry(polygon::makeCircle(b.getPosition() + point2D(42,42), 42,30));
			b.show();
			
			box.setLocation(point2D(0,0));
			box.setBoundry(polygon::makeCornerSquare(box.getPosition(),800,10));
			box.show();

			
			wallLeft.setLocation(point2D(0.0,800));
			wallLeft.setBoundry(polygon::makeCornerSquare(wallLeft.getPosition(),10,800));
			wallLeft.show();

			wallRight.setLocation(point2D(830,800));
			wallRight.setBoundry(polygon::makeCornerSquare(wallRight.getPosition(),10,800));
			wallRight.show();
			
			//background.setLocation(point2D(40,0));
			//background.show();

			walls.push_back(wallLeft);
			walls.push_back(wallRight);
			walls.push_back(box);

			arrowImg = Image(arrow);
	        directionArrow =(arrowImg);
			directionArrow.setLocation(point2D(g.getCol()/2.0, 20));
			directionArrow.show();

            //bgI = Image(backgroundIMG);

			level = 1;
	       
			generateLevel(1, obst);

}

game::~game()
{

}

void game::generateLevel (int level,vector<obstacle> &o ){
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

void game::moveUp(vector<obstacle> &o){
    point2D pos;

    for (size_t i = 0; i < o.size(); i++) {
        
        pos = o.at(i).getPosition();
        pos.y -= 150.0;

        o.at(i).updatePosition(pos);
    }

}

gameState game::playingGame(SDL_Plotter &g){
    cout<< "Playing Game " << endl;
	gameState nextState;

	double a = 0.001;
    bool pause = false;

    for (size_t i = 0; i < obst.size(); i++) {
        if (obst.at(i).isDead()) {
            obst.at(i).hide();
    	} 
    }

    g.clear();
    dt = dt + 1;

    b.update(dt);

    //Draw direction arrow
    int mouseX, mouseY;
    Vector2D direction;

    arrowPos = directionArrow.getPosition();
    arrowPos.y = arrowPos.y + 0.5*arrow.height;

    g.getMouseLocation(mouseX, mouseY);
    direction = Vector2D::createFromCartesian( mouseX - arrowPos.x, mouseY - arrowPos.y);
    
    directionArrow.drawImage(g,direction.getAngle() ,arrowPos);
    draw::drawFillPolygon(polygon::makeCircle(arrowPos,20,20).getPoints(),g, color(255,0,0));

	//Draw pause button
	Image bPause = Image(pauseButton);
	Object buttonPause(bPause);

	buttonPause.setLocation(point2D(750,10));
	buttonPause.setBoundry(polygon::makeCornerSquare(buttonPause.getPosition(), 75, -75)) ;
	buttonPause.drawObject(g, draw_polygon_outline);
	buttonPause.show();

	
    if(g.mouseClick()){
		int x,y;
		g.getMouseLocation(x, y);

		if(buttonPause.isInside(point2D(x,y))){
			pause = true;
			cout << "-------------------pause" << endl;
			//nextState = game_paused;
		}
		else{
			g.getMouseClick();
        	b.setLocation(arrowPos - point2D(beachBall.width/2, beachBall.height/2));
        	b.giveVelocity(direction);
		}
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

    static uint64_t s5;
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

	buttonPause.drawImage(g);

    if(pause){
        nextState = game_paused;
    }
    else{
        nextState = game_run;
    }

    return nextState;

}

gameState game::pauseGame(SDL_Plotter &g){
	gameState nextState = game_paused;

	//data abstraction
	Image sButtonImage(ButtonImg);
	Object startButton(sButtonImage);
	point2D mouseClick;

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
			nextState = game_run;
		}

		g.getMouseClick();
	}


	startButton.drawObject(g,draw_image);

	return nextState;
}

gameState game::gameOver(SDL_Plotter &g){
	gameState nextState = game_paused;

	//data abstraction
	Image sButtonImage(ButtonImg);
	Object startButton(sButtonImage);
	point2D mouseClick;

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
			nextState = game_run;
		}

		g.getMouseClick();
	}


	startButton.drawObject(g,draw_image);

	return nextState;
}

gameState game::startScreen(SDL_Plotter &g){

    cout<< "start screen " << endl;

	gameState nextState = game_startScreen;

	//data abstraction
	Image startImage(backgroundStart);
	Object startText;

	Image sButtonImage(ButtonImg);
	Object startButton(sButtonImage);
	point2D mouseClick;

	point2D buttonLoc(330, 340);
	point2D textLoc = buttonLoc + point2D(60,30);
	startButton.setLocation(buttonLoc);
	startButton.setTextLocation(textLoc);
	startButton.setBoundry(polygon::makeCornerSquare(point2D(330,350),200,-80));

	if(g.mouseClick()){
        cout<< "start screen: mouse cklick " << endl;
		
		point mouse = g.getMouseClick();
		mouseClick.x = mouse.x;		
		mouseClick.y = mouse.y;	

		if(startButton.isInside(mouseClick)){
			nextState = game_run;
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

	return nextState;
}
