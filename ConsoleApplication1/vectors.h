#pragma once

#include "globals.h"

struct vec2D
{
	float x, y;
};

vec2D Vec2D(float x, float y)
{
	vec2D result = {0};
	result.x = x;
	result.y = y;

	return result;	
}

double DistanceBetween(vec2D a,vec2D b) 
{
	double result = sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
	return result;
}

double Deg2Rads(double a) {
	return a * PI / 180.0;
}

vec2D DirectionFromAngle(float angle) {
	vec2D result = { 0 };
	result.x = cos(Deg2Rads(angle));
	result.y = sin(Deg2Rads(angle));
	return result;
}

inline vec2D
operator*(float a, vec2D b)
{
	vec2D result;

	result.x = a * b.x;
	result.y = a * b.y;

	return result;
}

inline vec2D
operator*(vec2D a, float b)
{
	vec2D result = b * a;
	return result;
}

inline vec2D
operator+(vec2D a, vec2D b)
{
	vec2D result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;

	return result;
}

inline vec2D
& operator*=(vec2D& a, float b)
{
	a = b * a;
	return(a);
}

inline vec2D
& operator+=(vec2D& a, vec2D b)
{
	a = a + b;
	return(a);
}

inline vec2D
operator-(vec2D a, vec2D b)
{
	vec2D result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;

	return result;
}

inline vec2D
operator-(vec2D a)
{
	vec2D result;

	result.x = -a.x;
	result.y = -a.y;

	return result;
}
