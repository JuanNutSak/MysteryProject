#pragma once

#include "globals.h"

#define TARGET_MS ((double)16.6667)
static uint64 global_performance_frequency;

static uint64 timers[3];

enum timed_block {
	INIT,
	WORK, 
	TOTAL
};

void InitTimers() {
	memset(timers, 0, sizeof(uint64) * 3);
	global_performance_frequency = SDL_GetPerformanceFrequency();
}

uint64 GetTickCounter() {
	uint64 result = 0;
	result = SDL_GetPerformanceCounter();
	return result;
}

double GetSecondsElapsed(uint64 start) {
	double result = 0.0;
	result = (double)(GetTickCounter() - start) / (double)global_performance_frequency;
	return result;
}

double GetSecondsElapsed(uint64 start, uint64 end) {
	double result = 0.0;
	result = (double)(end - start) / (double)global_performance_frequency;
	return result;
}

void StartTimer(timed_block type) {
	timers[type] = GetTickCounter();
}

// returns the number of milliseconds since that timer started
double StopTimer(timed_block type) {
	return GetSecondsElapsed(timers[type]) * 1000.0f;
}

void FrameDelay(double workTime) {
	//printf("\nmilliseconds to update and render: %.6f\n", workTime * 1000.0);

	if (workTime >= TARGET_MS) {	// we missed a frame		
		printf("\nmissed frame by %.4f seconds.\n", workTime - TARGET_MS);
	}
	// wait until its time for next frame

	// convert the wait interval to milliseconds for 'SDL_Delay's sake    		
	/* 10ms is the granularity of SDL_Delay, so to prevent it waiting too long, we subtract the possible overshoot
		and wait the rest of the time with a more precise method. NOTE : we don't use the precise method for the whole
		duration because it hogs the OSs attention where SDL_Delay does not */
	int waitInterval = ((int32)((TARGET_MS - workTime) * 1000.0)) - 10;
	SDL_Delay(waitInterval > 0 ? waitInterval : 0);

	double totalTime = StopTimer(TOTAL);
	while (totalTime < TARGET_MS)
		totalTime = StopTimer(TOTAL);

	printf("total milliseconds this frame: %.4f\n", totalTime);
}