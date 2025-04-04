#pragma once

class Time {
private:
	static double deltaTime;
	static double timeSinceStart;

	static double lastFrameTime;
public:
	static double DeltaTimeDouble();
	static float DeltaTime();
	static float TimeSinceStart();

	static void UpdateTime(double currentTime);
};