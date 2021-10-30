#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char* args[])	{
	// --------------------------------------------------------------------- Initialization
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	//Initialize PNG loading	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		SDL_Quit();
		return -1;
	}

	//Create window
	window = SDL_CreateWindow("SDL Tutorial", 0, 0, 1280, 720, SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == NULL)	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	//Create renderer for window
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	SDL_SetRenderDrawColor(renderer, 85, 25, 145, 255);

	// --------------------------------------------------------------------- Main Loop

	//Clear screen
	SDL_RenderClear(renderer);

	// all drawing should happen between these two SDL_Render functions

	//Update screen
	SDL_RenderPresent(renderer);

	//Wait two seconds
	SDL_Delay(2000);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

    return 0;
}
