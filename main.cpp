#include <iostream>
#include <cmath>
#include "SDL_Plotter.h"
#include "../images/dots.c"

using namespace std;

void drawCircle(point loc, int size, color c, SDL_Plotter& g);
void drawBmp(point loc,const img &im, SDL_Plotter& g);

int main(int argc, char ** argv)
{

    SDL_Plotter g(1000,1000);
    point p;
    color c;
    int size;
    Uint32 RGB;

    while (!g.getQuit())
    {

		if(g.kbhit()){
			switch(toupper(g.getKey())){
				case 'C': g.clear();
				          break;
			}
		}

		if(g.mouseClick()){
			p = g.getMouseClick();
			size = 10;
			c.R  = rand()%256;
			c.G  = rand()%256;
			c.B  = rand()%256;
			//drawCircle(p, size, c, g);
			drawBmp(p, gimp_image, g);
		}
		g.update();

    }
}

void drawBmp(point loc,const img &im, SDL_Plotter& g){
	
	for(unsigned int i = 0; i < im.width; i++){
		for(unsigned int j = 0; j < im.height; j = j + 3){
			g.plotPixel((loc.x + double(i /10)) , (loc.y + double(j /10)) ,
			 (color(int(im.pixel_data[i+j]), int(im.pixel_data[i + j + 1]), int(im.pixel_data[i + j + 2]))));

		}
	}
}

void drawCircle(point loc, int size, color c, SDL_Plotter& g){
	for(double i = -size; i <= size;i+=0.1){
		for(double j = -size; j <= size; j+=0.1){
			if(i*i + j*j <= size*size){
				g.plotPixel(round(loc.x+i),round(loc.y+j),c);
			}
		}
	}

}
