#include <iostream>

#include "globals.h"
#include "timing.h"
#include "particle.h"

enum image_id {
	PARTICLE
};

struct game_state {
	SDL_Window* window;
	SDL_Renderer* renderer;

	double dt;	
	vec2D forces[32];
	particle dots[100];	
};

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
		//printf("Unable to load image %s! SDL_image Error: %s\n", assetPath, IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			//printf("Unable to create texture from %s! SDL Error: %s\n", assetPath, SDL_GetError());
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

	state.forces[0] = Vec2D(0.0, 980);

	char textures[1][64];
	memset(textures, 0, 64);

	strncat_s(textures[PARTICLE], "particle.png", 12);
	const int particleCount = 90;	
	for (int i = 0; i < particleCount; i++)
	{
		state.dots[i] = CreateRandMovingParticle();
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

	// ------------------------------------------------------  Pre - Loop Testing

	vec2D a = Vec2D(1.0, 1.0);
	vec2D b = Vec2D(10.0, 10.0);

	vec2D c = a + b;

	printf("x: %f, y: %f\n", c.x, c.y);

	// -------------------------------------------------------------------------

	// --------------------------------------------------------------------- Main Loop
	bool running = true;	
	while(running) {
		StartTimer(WORK);
		StartTimer(TOTAL);
		// --------------------------------------------------------------------- SDL Event Handling
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
		
		// --------------------------------------------------------------------- Update
		vec2D totalForce = { 0 };
		for (int i = 0; i < 32; i++)		{
			
			totalForce += state.forces[i];
		}

		for (int i = 0; i < particleCount; i++)
		{
			UpdateParticle(&state.dots[i], totalForce, state.dt);

		}

		// --------------------------------------------------------------------- Render
		SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
		//Clear screen
		SDL_RenderClear(state.renderer);
		
		//Render texture to screen
		for (int i = 0; i < particleCount; i++)
		{
			DrawParticle(&state.dots[i], state.renderer, dot);

		}

		//Update screen
		SDL_RenderPresent(state.renderer);

		// --------------------------------------------------------------------- END OF FRAME - Calculate and Enforce Timing		
		double workTime = StopTimer(WORK);

		state.dt = FrameDelay(workTime);
	}

	// --------------------------------------------------------------------- Shut Down
	
	//Destroy window
	SDL_DestroyWindow(state.window);

	SDL_DestroyTexture(dot);
	//Quit SDL subsystems
	SDL_Quit();

    return 0;
}
