#pragma once

#include "globals.h"

int RandInt(int minNum, int maxNum) {
	return minNum + rand() % ((maxNum + 1) - minNum);
}

float randf() {
	return ((float)RandInt(0, 10)) * 0.1f;
}

float Randfloat(float minNum, float maxNum) {
	return minNum + randf() * (maxNum - minNum);
}

float RandLifeTime(float minLifeTime, float maxLifeTime) {
	return Randfloat(minLifeTime, maxLifeTime) * 60.0f;
}

struct particle {
	vec2D position;
	SDL_Color color;
	vec2D velocity;
	uint32 size;
	uint32 lifeTime;
};

particle CreateParticle(float x, float y, SDL_Color color, uint32 s, float minLifeTime,float maxLifeTime) {
	particle result;

	result.position = { x, y };
	result.color = color;
	result.velocity = { 0, 0 };
	result.size = s;
	result.lifeTime = RandLifeTime(minLifeTime, maxLifeTime);

	return result;
}

particle CreateParticle(float x, float y, float vx, float vy, SDL_Color color, uint32 s, float minLifeTime, float maxLifeTime) {
	particle result;

	result.position = { x, y };
	result.color = color;
	result.velocity = { vx, vy };
	result.size = s;
	result.lifeTime = RandLifeTime(minLifeTime, maxLifeTime);

	return result;
}

particle CreateRandParticle(SDL_Color color) {
	return CreateParticle((float)RandInt(0, 1280), (float)RandInt(0, 720), color, RandInt(0, 50), 1, 5);
}

particle CreateRandMovingParticle(SDL_Color color) {
	return CreateParticle((float)RandInt(0, 1280), (float)RandInt(0, 720), (float)RandInt(-300, 300), (float)RandInt(-800, 0), color, RandInt(0, 50), 1, 5);
}

void UpdateParticle(particle* p, vec2D acc, double dt)
{
	if (p->lifeTime > 0) {
		p->position += p->velocity * dt + (0.5 * acc * dt * dt);
		p->velocity += acc * dt;
		p->lifeTime--;
	}
	
	////TEMPORARY
	//if (p->position.y > 720 - p->size / 2) {
	//	p->position.y = 720 - p->size / 2;
	//	p->velocity.y = -0.5 * p->velocity.y;
	//}
	//if (p->position.x < 0) {
	//	p->position.x = 0;
	//	p->velocity.x = -0.5 * p->velocity.x;
	//}
	//if (p->position.x > 1280) {
	//	p->position.x = 1280;
	//	p->velocity.x = -0.5 * p->velocity.x;
	//}
}

void DrawParticle(particle* p, SDL_Renderer* renderer, SDL_Texture* img) {
	if (p->lifeTime > 0) {
		vec2D offset = { p->position.x - (float)p->size / 2.0 , p->position.y - (float)p->size / 2.0 };

		SDL_Rect dest = SDL_Rect{ (int)offset.x, (int)offset.y, (int)p->size, (int)p->size };
		SDL_SetTextureColorMod(img, p->color.r, p->color.g, p->color.b);
		SDL_RenderCopy(renderer, img, NULL, &dest);
	}
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
			vec2D fg = gravity ? Vec2D(0,980):Vec2D(0,0);
			UpdateParticle(&particles[i], fg, dt);
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
	Explosion(vec2D pos, float explosionPower, int partCount, int radius, SDL_Color color, bool isGravity = false)
		: ParticleEmitter(pos, partCount, isGravity) {
		for (int i = 0; i < partCount; i++)
		{			
			particles[i] = CreateParticle(position.x + RandInt(-radius,radius), position.y + RandInt(-radius,radius), RandInt(-explosionPower, explosionPower), RandInt(-explosionPower, explosionPower), color, 15, 1, 4);
		}
	}

	Explosion() {}
};