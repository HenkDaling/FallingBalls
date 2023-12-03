/*
 * SDL_Plotter.h
 *
 * Henk Daling - Version 3.2
 * Modify point struct
 * Add alpha chanels for drawing pixels
 * 11/21/23
 * 
 * Version 3.1
 * Add: color and point constructors
 * 12/14/2022
 *
 * Version 3.0
 * 5/31/2022
 *
 * Version 2.4
 * 4/4/2022
 *
 * Version 2.3
 *  6/28/2021
 *
 * Version 2.2
 *  4/26/2019
 *
 *  Dr. Booth
 */

#ifndef SDL_PLOTTER_H_
#define SDL_PLOTTER_H_

//OSX Library
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_thread.h>

//Windows Library
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <string.h>
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <queue>
#include <cmath>

#include "image.h"
using namespace std;

const char UP_ARROW    = 1;
const char DOWN_ARROW  = 2;
const char LEFT_ARROW  = 3;
const char RIGHT_ARROW = 4;
const int RED_SHIFT    = 65536;
const int GREEN_SHIFT  = 256;
const int BLUE_SHIFT   = 1;
const int ALPHA_SHIFT  = 16777216;
const int WHITE        = 255;
const int MAX_THREAD   = 100;


//m_point
struct m_point{
	int x,y;
	m_point(){
		x = y = 0;
	}

	m_point(int x, int y){
		this->x = x;
		this->y = y;
	}
};

//Color
struct color{
	unsigned int R,G,B,A;
	color(){
		R = G = B = A = 0;
	}


	color(int r, int g, int b){
		R = r;
		G = g;
		B = b;
	}

    color(int r, int g, int b, int a){
		R = r;
		G = g;
		B = b;
        A = a;
	}
};

//Threaded Sound Function
struct param{
	bool play;
	bool running;
	bool pause;
	SDL_Thread*  threadID;
	SDL_cond *cond;
	SDL_mutex *mut;
	string name;

	param(){
		play = false;
		running = false;
		pause = false;
	    cond = nullptr;
	    mut  = nullptr;
	    threadID = nullptr;
	    name="";
	}
};


class SDL_Plotter{
private:
    SDL_Texture  *mix;
	SDL_Texture  *texture;
    SDL_Texture *fontLayer;
    SDL_Texture *imageLayer;
	SDL_Renderer *renderer;
	SDL_Window   *window;
    TTF_Font *font;

    Uint32       *pixels;
    const Uint8  *currentKeyStates;
    SDL_Event    event;
    int          row, col;
    bool         quit;

    //Keyboard Stuff
    queue<char> key_queue;

    //Mouse Stuff
    queue<m_point> click_queue;

    //Sound Stuff
    bool SOUND;
    int soundCount;
    map<string, param> soundMap;

    char getKeyPress(SDL_Event & event);

public:
    SDL_Plotter(int r=480, int c=640, bool WITH_SOUND = true);
    ~SDL_Plotter();
    void update();

    bool getQuit();
    void setQuit(bool flag);

    bool kbhit();
    bool mouseClick();
    char getKey();
    m_point getMouseClick();

    void plotPixel(int x, int y, int r, int g, int b);
    void plotPixel(int x, int y, int r, int g, int b, int a);
    void plotPixel(m_point p, int r, int g, int b);
    void plotPixel(int x, int y, color=color{});
    void plotPixel(m_point p, color=color{});

    void plotText(int x, int y, string text);
    void plotImage(int x, int y,Image &I);

    void clear();
    int getRow();
    int getCol();

    void initSound(string sound);
    void playSound(string sound);
    void quitSound(string sound);

    void Sleep(int ms);

    bool getMouseDown(int& x, int& y);
    bool getMouseUp(int& x, int& y);
    bool getMouseMotion(int& x, int& y);
    void getMouseLocation(int& x, int& y);

    Uint32 getColor(int x, int y);

};

#endif // SDL_PLOTTER_H_
