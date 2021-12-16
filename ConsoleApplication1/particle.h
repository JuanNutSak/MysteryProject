#pragma once

#include "globals.h"

int RandInt(int minNum, int maxNum) {
	return minNum + rand() % ((maxNum + 1) - minNum);
}

struct particle {
	vec2D position;
	SDL_Color color;
	vec2D velocity;
	uint32 size;
};

particle CreateParticle(float x, float y, uint8 r, uint8 g, uint8 b, uint32 s) {
	particle result;

	result.position = { x, y };
	result.color = { r, g, b };
	result.velocity = { 0, 0 };
	result.size = s;

	return result;
}

particle CreateParticle(float x, float y, float vx, float vy, uint8 r, uint8 g, uint8 b, uint32 s) {
	particle result;

	result.position = { x, y };
	result.color = { r, g, b };
	result.velocity = { vx, vy };
	result.size = s;

	return result;
}



particle CreateRandParticle() {
	return CreateParticle((float)RandInt(0, 1280), (float)RandInt(0, 720), RandInt(0, 255), RandInt(0, 255), RandInt(0, 255), RandInt(0, 50));
}

particle CreateRandMovingParticle() {
	return CreateParticle((float)RandInt(0, 1280), (float)RandInt(0, 720), (float)RandInt(-300, 300), (float)RandInt(-800, 0), RandInt(0, 255), RandInt(0, 255), RandInt(0, 255), RandInt(0, 50));
}

void UpdateParticle(particle* p, vec2D acc, double dt) 
{
	p->position += p->velocity * dt + (0.5 * acc * dt * dt);	
	p->velocity += acc * dt;
}

void DrawParticle(particle* p, SDL_Renderer* renderer, SDL_Texture* img) {
	vec2D offset = { p->position.x - (float)p->size / 2.0 , p->position.y - (float)p->size / 2.0 };

	SDL_Rect dest = SDL_Rect{ (int)offset.x, (int)offset.y, (int)p->size, (int)p->size };
	SDL_SetTextureColorMod(img, p->color.r, p->color.g, p->color.b);
	SDL_RenderCopy(renderer, img, NULL, &dest);
}
