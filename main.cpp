/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "map.h"
#include "LTexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
using namespace std::chrono;
using namespace std;
//Texture wrapper class


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gRocketTexture;
LTexture gPowerTexture;
LTexture gBackgroundTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Rocket texture
	if (!gRocketTexture.loadFromFile("graphic/model3.png", gRenderer))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load Power' texture
	if (!gPowerTexture.loadFromFile("graphic/power.png", gRenderer))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	//Load background texture
	if (!gBackgroundTexture.loadFromFile("graphic/background2.png", gRenderer))
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded images
	gRocketTexture.free();
	gBackgroundTexture.free();
	gPowerTexture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}



int main(int argc, char* args[])
{
	double rocketHeight = -100;
	double velocity = 0;
	double gravity = 0.15;

	double difficultity = 350;

	double rocketX = 50;
	double rocketXV = 0;

	double scrollingSpeed = 3;

	int yPoint = 100;
	int xPoint = 0;
	int totalWidth;

	int scrollingOffsetBackground = 0;
	int scrollingOffsetMap = 0;

	bool power;
	bool collision = false;

	map downMap;

	//25FPS
	milliseconds dt(30);

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;


			steady_clock::time_point current_time = steady_clock::now();
			//While application is running
			while (!quit)
			{
				power = false;
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					if (e.key.keysym.sym == SDLK_SPACE)
					{
						power = true;
						velocity -= 0.5;
						rocketXV += 1;
					}
				}

				std::vector<vector<int>> downMapValues = downMap.getMap();
				
				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				
				//Render background texture to screen
				gBackgroundTexture.render(scrollingOffsetBackground, 0, gRenderer);
				gBackgroundTexture.render(scrollingOffsetBackground + gBackgroundTexture.getWidth(), 0, gRenderer);
				
				scrollingSpeed += 0.002;
				scrollingOffsetMap = scrollingOffsetMap - scrollingSpeed;
				--scrollingOffsetBackground + scrollingSpeed;
				if (scrollingOffsetBackground < -gBackgroundTexture.getWidth())
				{
					scrollingOffsetBackground = 0;
				}


				//Render Foo' to the screen
				gRocketTexture.render(	rocketX, -rocketHeight, gRenderer);
				if (power) gPowerTexture.render(rocketX-10, -rocketHeight+14, gRenderer);


				difficultity -= 0.01;

				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
				for (int i = 1; i < downMapValues.size(); i++) {
					SDL_RenderDrawLine(gRenderer, downMapValues[i-1][0]+scrollingOffsetMap, downMapValues[i-1][1], downMapValues[i][0]+scrollingOffsetMap, downMapValues[i][1]);
					SDL_RenderDrawLine(gRenderer, downMapValues[i-1][0]+scrollingOffsetMap, downMapValues[i-1][1]-difficultity, downMapValues[i][0]+scrollingOffsetMap, downMapValues[i][1]-difficultity);
						
					if(downMapValues[i-1][0] + scrollingOffsetMap <= rocketX+(gRocketTexture.getWidth()/2) && downMapValues[i][0] + scrollingOffsetMap > rocketX+(gRocketTexture.getWidth() / 2))
					collision = downMap.checkColisions(rocketX, rocketHeight, i, scrollingOffsetMap, gRocketTexture.getWidth(), difficultity);
				}
				

				//Update screen
				SDL_RenderPresent(gRenderer);
				if (downMap.getTotalLength() + scrollingOffsetMap < 900) {
					downMap.calculateMap(scrollingOffsetMap, difficultity);
				}

				rocketHeight = rocketHeight - velocity;
				velocity = velocity + gravity;

				if (collision) {
					while(true){}
				}

				this_thread::sleep_until(current_time = current_time + dt);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}