#ifndef GAME_H
#define GAME_H

#include <cstddef>
#include <chrono>

#include "image.h"
#include "images/dots.c"
#include "images/2703.c"
#include "images/button.c"
#include "images/pauseButton.c"
#include "images/background.c"
#include "images/backgroundStart.c"
#include "images/arrow.c"

#include "obstacle.h"
#include "Object.h"

#include "ball.h"

#include "draw.h"

using namespace std;

enum gameState{
	game_startScreen,
	game_run,
	game_paused,
	game_gameOver
};

class game
{
public:
    game(SDL_Plotter &g);
    ~game();

    void generateLevel (int level,vector<obstacle> &o );
    void moveUp(vector<obstacle> &o);

    gameState startScreen(SDL_Plotter &g);
    gameState pauseGame(SDL_Plotter &g);
    gameState gameOver(SDL_Plotter &g);
    gameState playingGame(SDL_Plotter &g);

private:

    int level;
	//Time for physics

	uint64_t t;
	uint64_t dt;
	static uint64_t s5;
    point p;
    color c;
    int size;

	// Image button;

	Vector2D force;
	point2D arrowPos;

	ball b;
	Object box;
	Object wallLeft;
	Object wallRight;

	Image bgI;
	Object background;
	Image arrowImg;
	Object directionArrow;

	vector<Object> walls;
	vector<obstacle> obst; 

};
#endif