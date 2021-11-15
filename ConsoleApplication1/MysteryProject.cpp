#include <iostream>

#include <ctype.h>

#include <SDL.h>
#include <SDL_image.h>

#define TARGET_SECONDS ((double)0.0166667)

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

static uint64 global_performance_frequency;

struct vec2D
{
	float x, y;
};
struct particle 
{
	vec2D position;
	SDL_Color color;
	vec2D velocity;
	uint32 size;
};

particle CreateParticle(float x, float y, uint8 r, uint8 g, uint8 b, uint32 s)
{
	particle result;

	result.position = { x, y };
	result.color = { r, g, b };
	result.velocity = { 0, 0 };
	result.size = s;

	return result;
}

int RandInt(int minNum, int maxNum)
{
	return minNum + rand() % ((maxNum + 1) - minNum);
}

particle CreateRandParticle()
{
	return CreateParticle((float)RandInt(0,1280), (float)RandInt(0, 720), RandInt(0,255), RandInt(0, 255), RandInt(0, 255), RandInt(0, 50));
}

void DrawParticle(particle *p, SDL_Renderer *renderer, SDL_Texture *img) 
{
	vec2D offset = {p->position.x - (float)p->size/2.0 , p->position.y - (float)p->size/2.0 };

	SDL_Rect dest = SDL_Rect{ (int)offset.x, (int)offset.y, (int)p->size, (int)p->size };
	//SDL_SetRenderDrawColor(renderer, p->color.r, p->color.g, p->color.b, 255);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderCopy(renderer, img, NULL, &dest);
}

uint64 GetTickCounter()
{
	uint64 result = 0;
	result = SDL_GetPerformanceCounter();
	return result;
}

double GetSecondsElapsed(uint64 start)
{
	double result = 0.0;
	result = (double)(GetTickCounter() - start) / (double)global_performance_frequency;
	return result;
}

double GetSecondsElapsed(uint64 start, uint64 end)
{
	double result = 0.0;
	result = (double)(end - start) / (double)global_performance_frequency;
	return result;
}

SDL_Texture* LoadTexture(std::string path, SDL_Renderer* renderer)
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
	
	global_performance_frequency = SDL_GetPerformanceFrequency();
	uint64 preInitCounter = GetTickCounter();
	particle dots[100];
	for (int i = 0; i < 100; i++) 
	{
		dots[i] = CreateRandParticle();
	}

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

	SDL_Texture* dot = LoadTexture("C:/Users/freez/source/repos/ConsoleApplication1/particle.png", renderer);
	
	if (!dot) {
		SDL_Quit();
		return -1;
	}

	double millisToInit = GetSecondsElapsed(preInitCounter) * 1000.0f;
	printf("\ninit time: %f\n\n", millisToInit);

	// --------------------------------------------------------------------- Main Loop
	bool running = true;	
	while(running) {
		uint64 workStart = GetTickCounter();

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

		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		//Clear screen
		SDL_RenderClear(renderer);
		
		//Render texture to screen
		for (int i = 0; i < 100; i++)
		{
			DrawParticle(&dots[i], renderer, dot);
		}

		//Update screen
		SDL_RenderPresent(renderer);

		// --------------------------------------------------  END OF FRAME - Calculate and Enforce Timing		
		double workTime = GetSecondsElapsed(workStart);		
		//printf("\nmilliseconds to update and render: %.6f\n", workTime * 1000.0);
			
		if (workTime >= TARGET_SECONDS) {	// we missed a frame		
			printf("\nmissed frame by %.4f seconds.\n", workTime - TARGET_SECONDS);
		}
		else {	// wait until its time for next frame
		
			// convert the wait interval to milliseconds for 'SDL_Delay's sake    		
			/* 10ms is the granularity of SDL_Delay, so to prevent it waiting too long, we subtract the possible overshoot
				and wait the rest of the time with a more precise method. NOTE : we don't use the precise method for the whole
			   duration because it hogs the OSs attention where SDL_Delay does not */
			int waitInterval = ((int32)((TARGET_SECONDS - workTime) * 1000.0)) - 10;
			SDL_Delay(waitInterval > 0 ? waitInterval : 0);

			double totalTime = GetSecondsElapsed(workStart);
			while (totalTime < TARGET_SECONDS)
				totalTime = GetSecondsElapsed(workStart);
		
			//printf("total milliseconds this frame: %.4f\n", totalTime * 1000.0);
		}
	}

	// --------------------------------------------------------------------- Shut Down
	
	//Destroy window
	SDL_DestroyWindow(window);

	SDL_DestroyTexture(dot);
	//Quit SDL subsystems
	SDL_Quit();

    return 0;
}
