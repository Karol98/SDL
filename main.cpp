#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "map.h"
#include "LTexture.h"
#include <sstream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

using namespace std::chrono;
using namespace std;

bool init();

bool loadMedia();

void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* gFont = NULL;

LTexture gRocketTexture;
LTexture gPowerTexture;
LTexture gBackgroundTexture;
LTexture gTextTexture;
LTexture gGameOver;

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	bool success = true;

	if (!gRocketTexture.loadFromFile("graphic/model3.png", gRenderer))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	if (!gPowerTexture.loadFromFile("graphic/power.png", gRenderer))
	{
		printf("Failed to load Foo' texture image!\n");
		success = false;
	}

	if (!gBackgroundTexture.loadFromFile("graphic/background2.png", gRenderer))
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}

	if (!gGameOver.loadFromFile("graphic/gameOver.png", gRenderer))
	{
		printf("Failed to load background texture image!\n");
		success = false;
	}
	else
	{
		gGameOver.setBlendMode(SDL_BLENDMODE_BLEND);
	}

	gFont = TTF_OpenFont("fonts/lazy.ttf", 28);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}

void close()
{
	gRocketTexture.free();
	gBackgroundTexture.free();
	gPowerTexture.free();
	gTextTexture.free();
	gFont = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}



int main(int argc, char* args[])
{
	double rocketHeight = -100;
	double velocity = 0;
	double gravity = 0.15;
	string str;
	double difficultity = 350;
	SDL_Color textColor = { 255, 255, 255 };
	double rocketX = 50;
	double rocketXV = 0;

	double scrollingSpeed = 3;

	int yPoint = 100;
	int xPoint = 0;
	int totalWidth;
	int score = 0;

	int scrollingOffsetBackground = 0;
	int scrollingOffsetMap = 0;

	bool power;
	bool collision = false;
	bool gameStarted = false;

	map downMap;

	//25FPS
	milliseconds dt(30);

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			bool quit = false;

			SDL_Event e;

			while (!quit)
			{
				power = false;
				while (SDL_PollEvent(&e) != 0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					if (e.key.keysym.sym == SDLK_SPACE)
					{	
						gameStarted = true;
						power = true;
						velocity -= 0.5;
						rocketXV += 1;
					}
				}

				if (gameStarted) {

					steady_clock::time_point current_time = steady_clock::now();
					
					std::vector<vector<int>> downMapValues = downMap.getMap();

					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					gBackgroundTexture.render(scrollingOffsetBackground, 0, gRenderer);
					gBackgroundTexture.render(scrollingOffsetBackground + gBackgroundTexture.getWidth(), 0, gRenderer);

					difficultity -= 0.01;
					scrollingSpeed += 0.002;
					scrollingOffsetMap = scrollingOffsetMap - scrollingSpeed;
					--scrollingOffsetBackground + scrollingSpeed;
					if (scrollingOffsetBackground < -gBackgroundTexture.getWidth())
					{
						scrollingOffsetBackground = 0;
					}

					gRocketTexture.render(rocketX, -rocketHeight, gRenderer);
					if (power) gPowerTexture.render(rocketX - 10, -rocketHeight + 14, gRenderer);

					for (int i = 1; i < downMapValues.size(); i++) {
						SDL_RenderDrawLine(gRenderer, downMapValues[i - 1][0] + scrollingOffsetMap, downMapValues[i - 1][1], downMapValues[i][0] + scrollingOffsetMap, downMapValues[i][1]);
						SDL_RenderDrawLine(gRenderer, downMapValues[i - 1][0] + scrollingOffsetMap, downMapValues[i - 1][1] - difficultity, downMapValues[i][0] + scrollingOffsetMap, downMapValues[i][1] - difficultity);
						if (downMapValues[i - 1][0] + scrollingOffsetMap <= rocketX + (gRocketTexture.getWidth() / 2) && downMapValues[i][0] + scrollingOffsetMap > rocketX + (gRocketTexture.getWidth() / 2))
							collision = downMap.checkColisions(rocketX, rocketHeight, i, scrollingOffsetMap, gRocketTexture.getWidth(), difficultity);
					}

					if (downMap.getTotalLength() + scrollingOffsetMap < 900) {
						downMap.calculateMap(scrollingOffsetMap, difficultity);
					}

					rocketHeight = rocketHeight - velocity;
					velocity = velocity + gravity;

					if (collision) {
						gGameOver.render(0, 0, gRenderer);
						gTextTexture.loadFromRenderedText("Game Over Score: " + str, { 0,0,0 }, gRenderer, gFont);
						gTextTexture.render(SCREEN_WIDTH / 2 - gTextTexture.getWidth() / 2, SCREEN_HEIGHT / 2 - gTextTexture.getHeight(), gRenderer);
						SDL_RenderPresent(gRenderer);
						while (true) {}
					}

					score = score + (350 - difficultity) + scrollingSpeed;
					str = to_string(score);
					gTextTexture.loadFromRenderedText("Score: " + str, textColor, gRenderer, gFont);
					gTextTexture.render(SCREEN_WIDTH - 220, (0 + gTextTexture.getHeight()), gRenderer);
					
					SDL_RenderPresent(gRenderer);
					
					this_thread::sleep_until(current_time = current_time + dt);
				}
				else
				{
					gBackgroundTexture.render(scrollingOffsetBackground, 0, gRenderer);
					gRocketTexture.render(rocketX, -rocketHeight, gRenderer);
					gGameOver.setAlpha(180);
					gGameOver.render(0, 0, gRenderer);
					gTextTexture.loadFromRenderedText("Hold Space to start the engines", { 0,0,0 }, gRenderer, gFont);
					gTextTexture.render(SCREEN_WIDTH / 2 - gTextTexture.getWidth() / 2, SCREEN_HEIGHT / 2 - gTextTexture.getHeight(), gRenderer);
					SDL_RenderPresent(gRenderer);
				}
			}
		}
	}

	close();

	return 0;
}