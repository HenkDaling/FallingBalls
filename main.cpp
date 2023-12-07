/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23

Data Abstraction: Define the plotter screen and game state
Input: Get key input
Process: Switch through game states
Output: Output Incorrect State or output
    game state to screen
Assumptions: assumes valid user imput

Henk Daling: Made obstacles, polygons, and game (MVP)
Mark Pattillo: Created image and draw
Griffin Roe: Created ball object and did documentation
Joshua Gilpin: Made vector
David Sobernheim: Helped make vector
*/

#include <iostream>
#include <cmath>

#include "SDL_Plotter.h"
#include "game.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char ** argv)
{
    //Data Abstraction
	// Start SDL Plotter
	SDL_Plotter g(800,840);

	game fallingBalls(g);
	gameState state;

	state = game_startScreen;

    while (!g.getQuit())
    {
        //Input
		if(g.kbhit()){
			switch(toupper(g.getKey())){
				case 'C': g.clear();
				          break;
			}
		}
        //Process and Output
		switch(state){
			case(game_startScreen):
			    //cout<< "STATE: START SCREEN " << endl;
				state = fallingBalls.startScreen(g);
				break;
			case(game_run):
				//cout<< "STATE GAME_RUN " << endl;
				state = fallingBalls.playingGame(g);
				break;
			case(game_paused):
				//cout<< "STATE GAME_PAUSED " << endl;
				state = fallingBalls.pauseGame(g);
				break;
			case(game_gameOver):
				//cout<< "STATE GAME_OVER" << endl;
				state = fallingBalls.gameOver(g);
				break;
			default:
				cout<< "INCORRECT STATE: " << state << endl;
		}

		g.update();

		}

	return 0;
}


