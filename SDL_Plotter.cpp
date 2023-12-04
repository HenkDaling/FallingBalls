/*
 * SDL_Plotter.h
 *
 * Version 3.0
 * 5/20/2022
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

#include "SDL_Plotter.h"
#include <SDL_ttf.h>
#include <algorithm>

//Threaded Sound Function

static int Sound(void *data){
	param *p = (param*)data;
	p->running = true;
	Mix_Chunk *gScratch = NULL;
	gScratch = Mix_LoadWAV( p->name.c_str() );


	while(p->running){
		SDL_mutexP( p->mut );
		  SDL_CondWait(p->cond, p->mut);
		  Mix_PlayChannel( -1, gScratch, 0 );
		  p->play = false;
		SDL_mutexV(p->mut);
	}

	Mix_FreeChunk( gScratch );
	p->running = false;
	return 0;
}


// SDL Plotter Function Definitions

SDL_Plotter::SDL_Plotter(int r, int c, bool WITH_SOUND){
	row = r;
	col = c;
	//leftMouseButtonDown = false;
	quit = false;
	SOUND = WITH_SOUND;
	currentKeyStates = NULL;

	SDL_Init(SDL_INIT_AUDIO);

    window   = SDL_CreateWindow("SDL2 Pixel Drawing",
    		                     SDL_WINDOWPOS_UNDEFINED,
    		                     SDL_WINDOWPOS_UNDEFINED, col, row, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    texture  = SDL_CreateTexture(renderer,
    		                     SDL_PIXELFORMAT_ARGB8888,
    		                     SDL_TEXTUREACCESS_TARGET, col, row);

	fontLayer  = SDL_CreateTexture(renderer,
    		                     SDL_PIXELFORMAT_ARGB8888,
    		                     SDL_TEXTUREACCESS_TARGET, col, row);

	imageLayer  = SDL_CreateTexture(renderer,
    		                     SDL_PIXELFORMAT_RGBA8888,
    		                     SDL_TEXTUREACCESS_TARGET, col, row);

	mix  = SDL_CreateTexture(renderer,
    		                     SDL_PIXELFORMAT_ARGB8888,
    		                     SDL_TEXTUREACCESS_TARGET, col, row);

    pixels   = new Uint32[col * row];

    memset(pixels, 0, col * row * sizeof(Uint32));

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
    }

    font = TTF_OpenFont("VT323.ttf", 40); // Replace with the path to your TrueType font file
    if (!font) {
        std::cerr << "Font loading failed: " << TTF_GetError() << std::endl;
    }

    currentKeyStates = SDL_GetKeyboardState( NULL );

    //SOUND Thread Pool
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    soundCount = 0;
    update();
  }


SDL_Plotter::~SDL_Plotter(){
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

void SDL_Plotter::update(){

	SDL_SetRenderTarget(renderer, texture);
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_MUL);
	//SDL_RenderClear(renderer);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_UpdateTexture(texture, NULL, pixels, col * sizeof(Uint32));
	
	SDL_SetRenderTarget(renderer,NULL);
	SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, mix);
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	//SDL_SetTextureBlendMode(mix, SDL_BLENDMODE_MUL);
	SDL_RenderClear(renderer);
	

    // Copy imageLayer with blend mode to mix
    //SDL_SetTextureBlendMode(mix, SDL_BLENDMODE_BLEND);
    //SDL_RenderCopy(renderer, imageLayer, NULL, NULL);

    // Copy texture with blend mode to mix
    SDL_SetTextureBlendMode(mix, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Copy fontLayer with blend mode to mix
    SDL_SetTextureBlendMode(mix, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, fontLayer, NULL, NULL);

	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, mix, NULL, NULL);

	SDL_SetRenderTarget(renderer, fontLayer);
	SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

}

Uint32 SDL_Plotter::getColor(int x, int y){
    return pixels[y * col + x];
}


bool SDL_Plotter::getQuit(){
	//Handle events on queue
	while( SDL_PollEvent( &event ) != 0 )
	{
		if(event.type == SDL_TEXTINPUT){
			key_queue.push(getKeyPress(event));
		}
		else if(event.type == SDL_KEYDOWN){
			//Make the arrow keys work
			if(currentKeyStates[SDL_SCANCODE_DOWN])  key_queue.push(DOWN_ARROW);
			if(currentKeyStates[SDL_SCANCODE_UP])    key_queue.push(UP_ARROW);
			if(currentKeyStates[SDL_SCANCODE_LEFT])  key_queue.push(LEFT_ARROW);
			if(currentKeyStates[SDL_SCANCODE_RIGHT]) key_queue.push(RIGHT_ARROW);
		}
		else if(event.type == SDL_MOUSEBUTTONUP){
			m_point p;
			SDL_GetMouseState( &p.x, &p.y );
			click_queue.push(p);
		}
		else if(event.type == SDL_MOUSEBUTTONDOWN){
			//SDL_GetMouseState( &mouse_X, &mouse_Y );
			//mouseClick = true;
		}
		else if(event.type == SDL_MOUSEMOTION){
			//SDL_PushEvent(&event);
		}

		if(event.type == SDL_QUIT || currentKeyStates[SDL_SCANCODE_ESCAPE]){
			quit = true;
		}
	}
	return quit;
}

bool SDL_Plotter::kbhit(){
	return key_queue.size() > 0;
}

bool SDL_Plotter::mouseClick(){
	return click_queue.size() > 0;
}


char SDL_Plotter::getKeyPress(SDL_Event & event){
	return *event.text.text;
}

char SDL_Plotter::getKey(){
    char key = '\0';
    if(key_queue.size() > 0){
    	key = key_queue.front();
    	key_queue.pop();
    }

    return key;
}

m_point SDL_Plotter::getMouseClick(){
	m_point p;
    if(click_queue.size() > 0){
    	p = click_queue.front();
    	click_queue.pop();
    }

	return p;
}


void SDL_Plotter::plotPixel(m_point p, int r, int g, int b){
	plotPixel(p.x,  p.y,  r,  g,  b);
}

void SDL_Plotter::plotPixel(int x, int y, color c){
	plotPixel(x,  y,  c.R,  c.G,  c.B);
}

void SDL_Plotter::plotPixel(m_point p, color c){
	plotPixel(p.x,  p.y,  c.R,  c.G,  c.B);
}


void SDL_Plotter::plotPixel(int x, int y, int r, int g, int b){
	int a = 50;
	if(x >= 0 && y >= 0 && x < col && y < row){
    	pixels[y * col + x] = RED_SHIFT*r + GREEN_SHIFT*g + BLUE_SHIFT*b + ALPHA_SHIFT*a;
	}
}

void SDL_Plotter::plotPixel(int x, int y, int r, int g, int b, int a){
	Uint8 dstR, dstG, dstB, dstA;
    SDL_PixelFormat* format;

    

    if (x >= 0 && y >= 0 && x < col && y < row) {
		format = SDL_AllocFormat(SDL_GetWindowPixelFormat(window));

		SDL_GetRGBA(pixels[y * col + x], format, &dstR, &dstG, &dstB, &dstA);

		// Alpha blending calculation
		dstR = (r * a + dstR * (255 - a)) / 255;
		dstG = (g * a + dstG * (255 - a)) / 255;
		dstB = (b * a + dstB * (255 - a)) / 255;
		dstA = (a * a + dstA * (255 - a)) / 255;
        pixels[y * col + x] = RED_SHIFT * dstR + GREEN_SHIFT * dstG + BLUE_SHIFT * dstB + ALPHA_SHIFT * dstA;

		SDL_FreeFormat(format);
    }

     
}

void SDL_Plotter::plotText(int x, int y, string text)
{
	SDL_Color textColor = {100, 100, 100, 255};

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor); 
    if (textSurface) {
        SDL_Texture *fontTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (fontTexture) {
            
            SDL_Rect destinationRect = {x, y, textSurface->w, textSurface->h};
			
			SDL_SetRenderTarget(renderer, fontLayer);
			
			SDL_SetTextureBlendMode(fontLayer, SDL_BLENDMODE_BLEND);

            SDL_RenderCopy(renderer, fontTexture, NULL, &destinationRect);

			SDL_SetRenderTarget(renderer,NULL);
            SDL_DestroyTexture(fontTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

void SDL_Plotter::plotImage(int X, int Y, Image &I)
{

	int index = 0;
    int drawX,drawY;
    int rd;
    int gn;
    int bl;
	int a;

    for (double y = Y; y < Y + I.height; ++y) {
        drawY = y;
        for (double x = X; x < X + I.width; ++x) {
            drawX = x;
            if (x >= 0 && x < col && y >= 0 && y < row) {
				
                rd = I.pixelData[index++];
                gn = I.pixelData[index++];
                bl = I.pixelData[index++];
				a = I.pixelData[index++];

				pixels[drawY * col + drawX] = RED_SHIFT * rd + GREEN_SHIFT * gn + BLUE_SHIFT * bl + ALPHA_SHIFT * a;
			}
        }
	}
	
}


void SDL_Plotter::clear(){
	     memset(pixels, 0, col * row * sizeof(Uint32));
}

int SDL_Plotter::getRow(){
    return row;
}

int SDL_Plotter::getCol(){
    return col;
}

void SDL_Plotter::initSound(string sound){
	if(!soundMap[sound].running){
			param* p = &soundMap[sound];
			p->name = sound;
			p->cond = SDL_CreateCond();
			p->mut = SDL_CreateMutex();

			p->threadID = SDL_CreateThread( Sound, sound.c_str(), (void*)p );
	}
}

void SDL_Plotter::setQuit(bool flag){
	this->quit = flag;
}

void SDL_Plotter::playSound(string sound){
	if(soundMap[sound].running){
		SDL_CondSignal(soundMap[sound].cond);
	}
}

void SDL_Plotter::quitSound(string sound){
	soundMap[sound].running = false;
	SDL_CondSignal(soundMap[sound].cond);
}

void SDL_Plotter::Sleep(int ms){
	SDL_Delay(ms);
}


bool SDL_Plotter::getMouseDown(int& x, int& y){
		bool flag = false;
		x = y = 0;
		if(SDL_PollEvent(&event)){
			if(event.type == SDL_MOUSEBUTTONDOWN){
				//Get mouse position
				flag = true;
				SDL_GetMouseState( &x, &y );
				cout << "FROM SDL" << x << " " << y << endl; 
			}
			else{
				SDL_PushEvent(&event);
			}
		}
		return flag;
}

bool SDL_Plotter::getMouseUp(int& x, int& y){
		bool flag = false;
		x = y = 0;
		if(SDL_PollEvent(&event)){
			if(event.type == SDL_MOUSEBUTTONUP){
				//Get mouse position
				flag = true;
				SDL_GetMouseState( &x, &y );
				cout << "FROM SDL" << x << " " << y << endl; 
			}
			else{
				SDL_PushEvent(&event);
			}
		}
		return flag;
}

bool SDL_Plotter::getMouseMotion(int& x, int& y){
		bool flag = false;
		x = y = 0;
		if(SDL_PollEvent(&event)){
			if(event.type == SDL_MOUSEMOTION){
				//Get mouse position
				flag = true;
				SDL_GetMouseState( &x, &y );
			}
			else{
				SDL_PushEvent(&event);
			}
		}
		return flag;
}

void SDL_Plotter::getMouseLocation(int& x, int& y){
    SDL_GetMouseState( &x, &y );
    cout << x << " " << y << endl;
}

