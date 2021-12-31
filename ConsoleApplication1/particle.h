#pragma once

#include "globals.h"

int RandInt(int minNum, int maxNum) {
	return minNum + rand() % ((maxNum + 1) - minNum);
	int x = 5;
	int y = 6;
	int a = x + y;	
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

	//TEMPORARY
	if (p->position.y > 720 - p->size / 2) {
		p->position.y = 720 - p->size / 2;
		p->velocity.y = -0.5 * p->velocity.y;
	}
	if (p->position.x < 0) {
		p->position.x = 0;
		p->velocity.x = -0.5 * p->velocity.x;
	}
	if (p->position.x > 1280) {
		p->position.x = 1280;
		p->velocity.x = -0.5 * p->velocity.x;
	}
}

void DrawParticle(particle* p, SDL_Renderer* renderer, SDL_Texture* img) {
	vec2D offset = { p->position.x - (float)p->size / 2.0 , p->position.y - (float)p->size / 2.0 };

	SDL_Rect dest = SDL_Rect{ (int)offset.x, (int)offset.y, (int)p->size, (int)p->size };
	SDL_SetTextureColorMod(img, p->color.r, p->color.g, p->color.b);
	SDL_RenderCopy(renderer, img, NULL, &dest);
}

 
class ParticleEmitter {
public:
	vec2D position;
	int particleCount;
	particle *particles;
	bool gravity;

public: 
	ParticleEmitter(vec2D pos, int partCount, bool isGravity = false)
	{
		particleCount = partCount;
		gravity = isGravity;
		int sizeOfArray = sizeof(particle) * particleCount;
		particles = (particle *)malloc(sizeOfArray);
		memset(particles, 0, sizeOfArray);

		position = pos;
	}

	ParticleEmitter() {}

	void UpdateEmitter(float dt) {
		for (int i = 0; i < particleCount; i++)
		{
			// TODO: add gravity if flag is true
			UpdateParticle(&particles[i], Vec2D(0, 0), dt);
		}
	}

	void DrawEmitter(SDL_Renderer *renderer, SDL_Texture *img) {
		for (int i = 0; i < particleCount; i++)
		{	
			DrawParticle(&particles[i], renderer, img);
		}
	}
};

class Explosion : public ParticleEmitter {		
public:
	Explosion(vec2D pos, float explosionPower, int partCount, bool isGravity = false) : ParticleEmitter(pos, partCount, isGravity) {
		for (int i = 0; i < partCount; i++)
		{			
			particles[i] = CreateParticle(position.x, position.y, RandInt(-explosionPower, explosionPower), RandInt(-explosionPower, explosionPower),  255, 255, 255, 15);
		}
	}

	Explosion() {}
};