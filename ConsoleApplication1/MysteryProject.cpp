#include <iostream>

#include "globals.h"

#include "timing.h"


struct vec2D
{
	float x, y;
};

enum image_id {
	PARTICLE
};

struct particle {
	vec2D position;
	SDL_Color color;
	vec2D velocity;
	uint32 size;
};

struct game_state {
	SDL_Window* window;
	SDL_Renderer* renderer;
};

particle CreateParticle(float x, float y, uint8 r, uint8 g, uint8 b, uint32 s)	{
	particle result;

	result.position = { x, y };
	result.color = { r, g, b };
	result.velocity = { 0, 0 };
	result.size = s;

	return result;
}

int RandInt(int minNum, int maxNum)	{
	return minNum + rand() % ((maxNum + 1) - minNum);
}

particle CreateRandParticle()	{
	return CreateParticle((float)RandInt(0,1280), (float)RandInt(0, 720), RandInt(0,255), RandInt(0, 255), RandInt(0, 255), RandInt(0, 50));
}

void DrawParticle(particle *p, SDL_Renderer *renderer, SDL_Texture *img) {
	vec2D offset = {p->position.x - (float)p->size/2.0 , p->position.y - (float)p->size/2.0 };

	SDL_Rect dest = SDL_Rect{ (int)offset.x, (int)offset.y, (int)p->size, (int)p->size };
	SDL_SetTextureColorMod(img, p->color.r, p->color.g, p->color.b);
	SDL_RenderCopy(renderer, img, NULL, &dest);
}


SDL_Texture* LoadTexture(image_id texture, SDL_Renderer* renderer, char textures[][64]) {	
	char* basePath = SDL_GetBasePath();
	char assetPath[512];
	memset(assetPath, 0, 512);
	strncat_s(assetPath, basePath, strlen(basePath));
	strncat_s(assetPath, "..\\..\\assets\\", 14);
	strncat_s(assetPath, textures[texture], strlen(textures[texture]));	

	SDL_free(basePath);

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(assetPath);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", assetPath, IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", assetPath, SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

int InitializeSDL(game_state *state) {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	//Initialize PNG loading	
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		SDL_Quit();
		return -1;
	}

	//Create window
	state->window = SDL_CreateWindow("SDL Tutorial", 0, 0, 1280, 720, SDL_WINDOW_ALLOW_HIGHDPI);
	if (state->window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	//Create renderer for window
	state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
	if (state->renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
}

int main(int argc, char* args[])	{
	InitTimers();
	StartTimer(INIT);
	
	game_state state = { 0 };

	char textures[1][64];
	memset(textures, 0, 64);

	strncat_s(textures[PARTICLE], "particle.png", 12);

	particle dots[100];
	for (int i = 0; i < 100; i++) 
	{
		dots[i] = CreateRandParticle();
	}

	// --------------------------------------------------------------------- Initialization
	
	InitializeSDL(&state);

	char textureName[64];
	memset(textureName, 0, 64);
	strncat_s(textureName, "particle.png", 12);

	SDL_Texture* dot = LoadTexture(PARTICLE, state.renderer, textures);
	
	if (!dot) {
		SDL_Quit();
		return -1;
	}

	double millisToInit = StopTimer(INIT);
	printf("\ninit time: %fms\n\n", millisToInit);

	// --------------------------------------------------------------------- Main Loop
	bool running = true;	
	while(running) {
		StartTimer(WORK);
		StartTimer(TOTAL);

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

		SDL_SetRenderDrawColor(state.renderer,0,0,0,255);
		//Clear screen
		SDL_RenderClear(state.renderer);
		
		//Render texture to screen
		for (int i = 0; i < 100; i++)
		{
			DrawParticle(&dots[i], state.renderer, dot);
		}

		//Update screen
		SDL_RenderPresent(state.renderer);

		// --------------------------------------------------  END OF FRAME - Calculate and Enforce Timing		
		double workTime = StopTimer(WORK);

		FrameDelay(workTime);
		
	}

	// --------------------------------------------------------------------- Shut Down
	
	//Destroy window
	SDL_DestroyWindow(state.window);

	SDL_DestroyTexture(dot);
	//Quit SDL subsystems
	SDL_Quit();

    return 0;
}
