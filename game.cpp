/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#include "game.h"
#include <chrono>
#include <iostream>

#include <algorithm>
#include <cstdint>
#include <iomanip>


using namespace std::chrono;
using namespace std;

game::game(SDL_Plotter &g)
{
			arrowImg = Image(arrow);
	        directionArrow =(arrowImg);
			directionArrow.setLocation(point2D(g.getCol()/2.0, 20));
			directionArrow.show();

			b = ball( directionArrow.getPosition() - point2D(beachBall.width/2, beachBall.height/2), Vector2D::createFromCartesian(0,0));

			b.setLocation(point2D(g.getCol()/2.0, 20)  - point2D(beachBall.width/2, beachBall.height/2));
			//b.applyForce(Vector2D(10, directions.up));

    		b.setBoundry(polygon::makeCircle((b.getPosition() + point2D(42,42)), 42,30));
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

float perlin(float x) {
    // Hash function (replace with a better hash function if needed)
    auto hash = [](int x) {
        return (x << 13) ^ x;
    };

    // Fade function
    auto fade = [](float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    };

    // Lerp function
    auto lerp = [](float t, float a, float b) {
        return a + t * (b - a);
    };

    // Gradient function
    auto grad = [](int hash, float x) {
        int h = hash & 15;
        float grad = 1.0 + (h & 7); // Gradient value 1-8
        if (h & 8) grad = -grad;    // Randomly invert half of them
        return (grad * x);         // This is the key part where we multiply the gradient with the distance
    };

    // Calculate grid coordinates
    int X = static_cast<int>(floor(x)) & 255;
    x -= floor(x);
    float u = fade(x);

    // Calculate the Perlin noise value
    return lerp(u, grad(hash(X), x), grad(hash(X + 1), x - 1)) * 2;
}

gameState game::playingGame(SDL_Plotter &g){
	gameState nextState;
	bool pause = false;
	bool gameOver = false;

	double a = 0.001;

	dt = dt + 1;

	g.clear();

	//Background test:
	//    for (int y = 0; y < 800; y++) {
    //     for (int x = 0; x < 840; x++) {
    //         // Calculate color based on position
    //         double gradientFactor = static_cast<double>(y) / 800; // Gradient based on vertical position

	// 		gradientFactor = (x^2 + y^2 ) / 255;

    //         // Retro-style colors
    //         int red = static_cast<int>(255 * gradientFactor);
    //         int green = static_cast<int>(165 + 90 * gradientFactor);
    //         int blue = static_cast<int>(255 - 255 * gradientFactor);

    //         // Draw pixel
    //         g.plotPixel(x, y, red, green, blue);
    //     }
	//    }


    for (size_t i = 0; i < obst.size(); i++) {
        if (obst.at(i).isDead()) {
            obst.at(i).hide();
    	}
		if (obst.at(i).isTop()){
			gameOver = true;
		}
    }

	if(b.getPosition().y > 800){
		level++;
        moveUp(obst);
        generateLevel(level, obst);

		b.setLocation(arrowPos  - point2D(beachBall.width/2, beachBall.height/2));
		b.giveVelocity(Vector2D::createFromCartesian(0,0));
		b.setForce(Vector2D::createFromCartesian(0,0));

	}

    //Draw direction arrow
    int mouseX, mouseY;
    Vector2D direction;

    arrowPos = directionArrow.getPosition();
    arrowPos.y = arrowPos.y + 0.5*arrow.height;

    g.getMouseLocation(mouseX, mouseY);
    direction = Vector2D::createFromCartesian( mouseX - arrowPos.x, mouseY - arrowPos.y);

    directionArrow.drawImage(g,direction.getAngle() ,arrowPos, color(255,255,255));
    draw::drawFillPolygon(polygon::makeCircle(arrowPos,20,20).getPoints(),g, color(255,0,0));

	//Draw buttons
	Image bPause = Image(pauseButton);
	Object buttonPause(bPause);

	Image bQuit = Image(quitButton);
	Object buttonQuit(bQuit);

	buttonPause.setLocation(point2D(750,10));
	buttonPause.setBoundry(polygon::makeCornerSquare(buttonPause.getPosition(), 75, -75)) ;
	buttonPause.drawObject(g, draw_polygon_outline);
	buttonPause.show();

	buttonQuit.setLocation(point2D(750,95));
	buttonQuit.setBoundry(polygon::makeCornerSquare(buttonQuit.getPosition(), 75, -75)) ;
	buttonQuit.drawObject(g, draw_polygon_outline);
	buttonQuit.show();

    if(g.mouseClick()){
		int x,y;
		g.getMouseLocation(x, y);

		b.applyForce(Vector2D(6, directions.up));

		if(buttonPause.isInside(point2D(x,y))){
			pause = true;
		}
		else if(buttonQuit.isInside(point2D(x,y))){
			g.setQuit(true);
		}
		else{
			g.getMouseClick();
        	b.setLocation(arrowPos - point2D(beachBall.width/2, beachBall.height/2));
        	b.giveVelocity(direction);
		}
    }

    static double ang = 0;
    ang += 0.01;


	b.update(dt);
	b.drawImage(g,0, b.getBoundry().calculateCenter(), color(255,255,255));
    //b.drawObject(g,draw_polygon_outline,color(255,0,0));

    for(Object &wall : walls){
        wall.drawObject(g,draw_polygon_fill,color(0,200,200));
        b.isColiding(wall);
    }

	for (size_t i = 0; i < obst.size(); i++) {
		point2D center;
		obst.at(i).rotate(a,obst.at(i).getPosition());
		center = obst.at(i).getPosition();
		center = center - point2D(10,20) ;
		obst.at(i).setTextLocation(center);
		obst.at(i).drawObstacle(g);
    }


    for (size_t i = 0; i < obst.size(); i++) {
        if(!obst.at(i).isDead()){
            obst.at(i).isColiding(b);
            if(b.isColiding(obst.at(i))){
				cout << "checking\n";
                obst.at(i).handleCollision(b);
            }
        }

    }

    // static uint64_t s5;
    // if( duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - s5 > 4000){
    //     level++;
    //     moveUp(obst);
    //     generateLevel(level, obst);
    //     s5 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    // }

	buttonPause.drawImage(g);
	buttonQuit.drawImage(g);

    if(pause){
        nextState = game_paused;
    }
    else if(gameOver){
        nextState = game_gameOver;
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
	Image banner(bannerImg);
	Object startButton(sButtonImage);
	point2D mouseClick;

	point2D buttonLoc(330, 500);
	point2D textLoc = buttonLoc + point2D(60,30);
	startButton.setLocation(buttonLoc);
	startButton.setBoundry(polygon::makeCornerSquare(buttonLoc,200,-80));

	if(g.mouseClick()){

		point mouse = g.getMouseClick();
		mouseClick.x = mouse.x;
		mouseClick.y = mouse.y;

		if(startButton.isInside(mouseClick)){
			nextState = game_run;
		}

		g.getMouseClick();
	}


	draw::drawImagePixels(point2D(80,200), banner, g, 0, point2D(), color(255,0,255));
	static double dt = 0;
	static bool init = false;
	dt += 1;
	static Uint32 *screen = new Uint32[800*840];

	if(!init){
	for (int y = 0; y < 800; y++) {
		for (int x = 0; x < 840; x++) {
			screen[y * 800 + x]	= g.getColor(x,y);
		}
	}
	init = true;
	}


	for (int y = 0; y < 800; y++) {
		for (int x = 0; x < 840; x++) {
			// Calculate color based on position
			double gradientFactor = static_cast<double>(y) / 800; // Gradient based on vertical position
			Uint32 bg = screen[y * 800 + x];

			//gradientFactor = cos((y/(rand()%8 + 3) - dt) * -1 );

			gradientFactor *=  (cos(((x-420) / 420.0) * (3.14/2.0))) * 0.5;
			gradientFactor *=  (cos(((y-400) / 400.0) * (3.14/2.0))) * 0.5;

			//bg *= gradientFactor;

			//red = static_cast<int>(0);
			//green = static_cast<int>( 90 * gradientFactor);
			//blue = static_cast<int>(0);

			int red = (bg >> 16) & 0xFF;
			int green = (bg >> 8) & 0xFF;
			int blue = (bg & 0xFF);

			red = ((bg >> 16) & 0xFF) * gradientFactor * 2 ;
			green = ((bg >> 8) & 0xFF) * gradientFactor * 2 ;
			blue = ((bg & 0xFF)) * gradientFactor * 2 ;

			// Draw pixel
			g.plotPixel(x, y, red, green, blue);
		}
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

		static double dt = 0;
	static bool init = false;
	dt += 1;
	static Uint32 *screen = new Uint32[800*840];

	if(!init){
		obst.clear();

		for (int y = 0; y < 800; y++) {
			for (int x = 0; x < 840; x++) {
				screen[y * 800 + x]	= g.getColor(x,y);
			}
		}
		init = true;
	}


	for (int y = 0; y < 800; y++) {
	for (int x = 0; x < 840; x++) {
		// Calculate color based on position
		double gradientFactor = static_cast<double>(y) / 800; // Gradient based on vertical position
		Uint32 bg = screen[y * 800 + x];

		gradientFactor = cos((y/(rand()%8 + 3) - dt) * -1 );

		gradientFactor *=  (cos(((x-420) / 420.0) * (3.14/2.0))) * 0.5;
		gradientFactor *=  (cos(((y-400) / 400.0) * (3.14/2.0))) * 0.5;

		//bg *= gradientFactor;

		//red = static_cast<int>(0);
		//green = static_cast<int>( 90 * gradientFactor);
		//blue = static_cast<int>(0);

		int red = (bg >> 16) & 0xFF;
		int green = (bg >> 8) & 0xFF;
		int blue = (bg & 0xFF);

		red = ((bg >> 16) & 0xFF) * gradientFactor * 0.1;
		green = ((bg >> 8) & 0xFF) * gradientFactor * 0.1;
		blue = ((bg & 0xFF)) * gradientFactor * 0.1;

		// Draw pixel
		g.plotPixel(x, y, red, green, blue);
	}
	}


	startButton.drawObject(g,draw_image);

	return nextState;
}

gameState game::startScreen(SDL_Plotter &g){


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
		init = true;
	}

	static double dt = 0;
	static bool inits = false;
	dt += 1;
	//static Uint32 *screen = new Uint32[800*840];

	// if(!inits){
	// for (int y = 0; y < 800; y++) {
	// 	for (int x = 0; x < 840; x++) {
	// 		screen[y * 800 + x]	= g.getColor(x,y);
	// 	}
	// }
	// 	inits = true;
	// }


	// for (int y = 0; y < 800; y++) {
	// for (int x = 0; x < 840; x++) {
	// 	// Calculate color based on position
	// 	double gradientFactor = 1;
	// 	double fadeFactor = 1;
	// 	Uint32 bg = backgroundIMG.pixel_data[y * 800 + x];

	// 	gradientFactor = cos((y/(rand()%8 + 3) - dt) * -1 );

	// 	fadeFactor *=  (cos(((x-420) / 420.0) * (3.14/2.0)));
	// 	fadeFactor *=  (cos(((y-400) / 400.0) * (3.14/2.0)));

	// 	//bg *= gradientFactor;

	// 	//red = static_cast<int>(0);
	// 	//green = static_cast<int>( 90 * gradientFactor);
	// 	//blue = static_cast<int>(0);

	// 	//gradientFactor = std::clamp(gradientFactor, 0.0, 1.0);

	// 	int red = (bg >> 16) & 0xFF ;
	// 	int green = (bg >> 8) & 0xFF;
	// 	int blue = (bg & 0xFF);

	// 	// red = red * fadeFactor  ;
	// 	// green = green * fadeFactor ;
	// 	// blue = blue * fadeFactor ;

	// 	// red =  (gradientFactor * 100) + red;
	// 	// green = (gradientFactor * 100) + green ;
	// 	// blue =  (gradientFactor * 100) + red ;

	// 	double drawY = y ;//+ gradientFactor ;

	// 	//if(x >= 0 && drawY >= 0 && x < 840 && drawY < 800){
	// 		// Draw pixel
	// 		g.plotPixel(x, drawY, red, green, blue);
	// 	//}
	// }
	// }

	startButton.drawObject(g,draw_image);

	return nextState;
}
