#include "Time.h"

double Time::deltaTime = 0;
double Time::timeSinceStart = 0;

double Time::lastFrameTime = 0;

float Time::DeltaTime() {
	return static_cast<float>(deltaTime);
}
double Time::DeltaTimeDouble() {
	return deltaTime;
}
float Time::TimeSinceStart() {
	return static_cast<float>(timeSinceStart);
}

void Time::UpdateTime(double currentTime) {
	lastFrameTime = timeSinceStart;
	timeSinceStart = currentTime;

	deltaTime = timeSinceStart - lastFrameTime;
}