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
	uint32 initialLifeTime;
	bool fade;
	float alpha;
};

particle CreateParticle(vec2D pos, SDL_Color color, uint32 s, float minLifeTime,float maxLifeTime, bool fadeOut = false) {
	particle result;

	result.position = pos;
	result.color = color;
	result.velocity = { 0, 0 };
	result.size = s;
	result.lifeTime = RandLifeTime(minLifeTime, maxLifeTime);
	result.initialLifeTime = result.lifeTime;
	result.fade = fadeOut;
	result.alpha = 255;

	return result;
}

particle CreateParticle(vec2D pos, vec2D vel, SDL_Color color, uint32 s, float minLifeTime, float maxLifeTime, bool fadeOut = false) {
	particle result;

	result.position = pos;
	result.color = color;
	result.velocity = vel;
	result.size = s;	
	result.lifeTime = RandLifeTime(minLifeTime, maxLifeTime);
	result.initialLifeTime = result.lifeTime;
	result.fade = fadeOut;	

	return result;
}

particle CreateRandParticle(SDL_Color color) {
	return CreateParticle(Vec2D((float)RandInt(0, 1280), (float)RandInt(0, 720)), color, RandInt(10, 50), RandInt(0.3, 3), RandInt(4, 8));
}

particle CreateRandMovingParticle(SDL_Color color) {
	return CreateParticle(Vec2D((float)RandInt(0, 1280), (float)RandInt(0, 720)), Vec2D((float)RandInt(-300, 300), (float)RandInt(-800, 0)), color, RandInt(0, 50), RandInt(0.3, 3), RandInt(4, 8));
}

void UpdateParticle(particle* p, vec2D acc, double dt)
{
	if (p->lifeTime > 0) {
		p->position += p->velocity * dt + (0.5 * acc * dt * dt);
		p->velocity += acc * dt;

		if (p->fade) {
			float pct = (float)p->lifeTime / (float)p->initialLifeTime;
			p->color.a = pct * 255;
		}

		p->lifeTime--;
	}
}

void DrawParticle(particle* p, SDL_Renderer* renderer, SDL_Texture* img) {
	if (p->lifeTime > 0) {
		vec2D offset = { p->position.x - (float)p->size / 2.0 , p->position.y - (float)p->size / 2.0 };

		SDL_Rect dest = SDL_Rect{ (int)offset.x, (int)offset.y, (int)p->size, (int)p->size };
		SDL_SetTextureColorMod(img, p->color.r, p->color.g, p->color.b);
		SDL_SetTextureAlphaMod(img, p->color.a);
		SDL_RenderCopy(renderer, img, NULL, &dest);
	}
}

class ParticleEmitter {
public:
	vec2D position;
	int particleCount;
	particle *particles;
	bool gravity;
 
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
	Explosion(vec2D pos, float explosionPower, int partCount, int radius, SDL_Color colors[], int minSize, int maxSize, bool isGravity = false)
		: ParticleEmitter(pos, partCount, isGravity) {
		for (int i = 0; i < partCount; i++)
		{			
			particles[i] = CreateParticle(Vec2D(position.x + RandInt(-radius,radius), position.y + RandInt(-radius,radius)), 
										  Vec2D(RandInt(-explosionPower, explosionPower), RandInt(-explosionPower, explosionPower)), 
										  colors[RandInt(0, ArrayCount(colors)-1)], RandInt(minSize, maxSize), RandInt(0.3, 3), RandInt(4, 8), true);
		}
	}

	Explosion() {}
};

class JetStream : public ParticleEmitter {
public:
	float angle;
	float width;

	JetStream(vec2D pos, float jetStreamPower, int partCount, float emitterAngle, float emitterWidth, SDL_Color colors[], int minSize, int maxSize, bool isGravity = false)
		:ParticleEmitter(pos, partCount, isGravity) {
		this->angle = emitterAngle;
		this->width = emitterWidth;
		
		//TODO figure out how particle creation will work
	}
	
	void UpdateEmitter(float dt) {
		ParticleEmitter::UpdateEmitter(dt);

	}
	
};