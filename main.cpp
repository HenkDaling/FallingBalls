#include <iostream>
#include <cmath>

#include "SDL_Plotter.h"
#include "game.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char ** argv)
{
	// Start SDL Plotter
	SDL_Plotter g(800,840);

	game fallingBalls(g);
	gameState state;

	state = game_startScreen;

    while (!g.getQuit())
    {
		if(g.kbhit()){
			switch(toupper(g.getKey())){
				case 'C': g.clear();
				          break;
			}
		}

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


