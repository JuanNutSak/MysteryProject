#include <iostream>

#include <ctype.h>

#include <SDL.h>
#include <SDL_image.h>

#define TARGET_MS ((double)16.6667)

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
SDL_Texture* loadTexture(std::string path, SDL_Renderer* renderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int main(int argc, char* args[])	{
	
	uint64 perfFrequency = SDL_GetPerformanceFrequency();

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
	SDL_Texture* dot = loadTexture("C:/Users/freez/source/repos/ConsoleApplication1/particle.png", renderer);

	// --------------------------------------------------------------------- Main Loop
	bool running = true;
	uint64 LastCounter = SDL_GetPerformanceCounter();

	while(running) {
		
		SDL_Event event;
		
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type)
			{
			case SDL_QUIT: {
				running = false;
			} break;

			case SDL_KEYUP: {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: {
					running = false;
				} break;

				default:
					break;
				}
			} break;

			default:
				break;
			}
		}

		//Clear screen
		SDL_RenderClear(renderer);

		// all drawing should happen between these two SDL_Render functions
		//Render texture to screen
		SDL_Rect dest = SDL_Rect{ 440,160,400,400 };
		SDL_RenderCopy(renderer, dot, NULL, &dest);

		//Update screen
		SDL_RenderPresent(renderer);

		uint64 EndCounter = SDL_GetPerformanceCounter();
		uint64 CounterElapsed = EndCounter - LastCounter;

		double MSPerFrame = (((1000.0f * (double)CounterElapsed) / (double)perfFrequency));
		double FPS = (double)perfFrequency / (double)CounterElapsed;
		LastCounter = EndCounter;

		printf("%.02f ms/f, %.02ff/s\n", MSPerFrame, FPS);
	}

	// --------------------------------------------------------------------- Shut Down
	
	//Destroy window
	SDL_DestroyWindow(window);

	SDL_DestroyTexture(dot);
	//Quit SDL subsystems
	SDL_Quit();

    return 0;
}
