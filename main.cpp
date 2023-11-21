#include <iostream>
#include <cmath>
#include "SDL_Plotter.h"
#include "images/dots.c"
#include "images/beachBall.c"
#include "images/2703.c"


using namespace std;

void drawCircle(point loc, int size, color c, SDL_Plotter& g);
void drawBmp(point loc,const img &im, SDL_Plotter& g);
void drawImage(point loc, const img& image, SDL_Plotter& g);

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
			drawImage(p, beachBall, g);
		}

		g.update();

    }

	return 0;
}


void drawImage(point loc, const img& image, SDL_Plotter& g){
	
    int index = 0;
    for (int y = loc.y; y < image.height + loc.y; ++y) {
        for (int x = loc.x; x < image.width + loc.x; ++x) {
            int rd = image.pixel_data[index++];
            int gn = image.pixel_data[index++];
            int bl = image.pixel_data[index++];
			if(image.bytes_per_pixel == 4){
				int a = image.pixel_data[index++];
				
				g.plotPixel(x,y,rd,gn,bl, a);
			}
			else{
				g.plotPixel(x, y, rd, gn, bl);   
			}
        }
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
