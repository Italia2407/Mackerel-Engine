#include "TimeManager.h"
#include "GLinclude.h"
#include <chrono>
#include <thread>
using namespace MCK;

TimeManager* TimeManager::instance = nullptr;

TimeManager::TimeManager()
{
	timescale = 0.0;
	lastFrame = 0.0;
	lastScaledFrame = 0.0;
	upTime = glfwGetTime();
	scaledUpTime = 0.0f;
	fps = 0.0;
}

TimeManager::~TimeManager() {}


// private Singleton functions
bool MCK::TimeManager::Release()
{
	bool success = false;

	if (instance)
	{
		instance->timers.clear();
		instance->scaledTimers.clear();
		delete instance;
		success = true;
	}

	return success;
}


double MCK::TimeManager::privGetUpTime()
{
	// returns the value of the GLFW timer
	return upTime;
}

double MCK::TimeManager::privGetScaledUpTime()
{
	return scaledUpTime;
}

void MCK::TimeManager::privWaitForEnforcedFrameTime(double minFrameTime)
{
	double curTime = glfwGetTime();
	double waitTime = minFrameTime - (curTime - upTime);

	std::this_thread::sleep_for(std::chrono::duration<double>(waitTime));
}


void MCK::TimeManager::privSetTimer(std::pair<double, std::function<void()>> timer)
{
	timer.first = timer.first + privGetUpTime();

	// iterator for the list
	std::list<std::pair<double, std::function<void()>>>::iterator it;
	it = timers.begin();

	// loop through all timers and insert it correctly
	while (it != timers.end())
	{
		if (it->first >= timer.first)
		{
			// timer gets inserted before the timer it was compared to
			timers.insert(it, timer);
			return;
		}
		it++;
	}
	// add to the end if all other timers end earlier
	timers.push_back(timer);
}

void MCK::TimeManager::privSetScaledTimer(std::pair<double, std::function<void()>> timer)
{
	timer.first = timer.first + privGetScaledUpTime();

	// iterator for the list
	std::list<std::pair<double, std::function<void()>>>::iterator it;
	it = scaledTimers.begin();

	// loop through all timers and insert it correctly
	while (it != scaledTimers.end())
	{
		if (it->first >= timer.first)
		{
			// timer gets inserted before the timer it was compared to
			scaledTimers.insert(it, timer);
			return;
		}
		it++;
	}
	// add to the end if all other timers end earlier
	scaledTimers.push_back(timer);
}


void MCK::TimeManager::privSetTimescale(double scale)
{
	timescale = scale;
}

float MCK::TimeManager::privGetTimescale()
{
	return timescale;
}


double MCK::TimeManager::privGetFrameTime()
{
	return scaledUpTime - lastScaledFrame;
}


double MCK::TimeManager::privGetUnscaledFrameTime()
{
	return upTime - lastFrame;
}

double MCK::TimeManager::privGetFPS()
{
	return fps;
}


void MCK::TimeManager::privUpdate()
{
	// Enforce wait
	privWaitForEnforcedFrameTime(0.013);

	// update unscaled time
	lastFrame = upTime;
	upTime = glfwGetTime();

	// update scaled time
	lastScaledFrame = scaledUpTime;
	scaledUpTime = scaledUpTime + (upTime - lastFrame) * timescale;

	// check unscaled timers
	// iterator for the list
	std::list<std::pair<double, std::function<void()>>>::iterator it;

	it = timers.begin();

	while (it != timers.end())
	{
		if (upTime >= it->first)
		{
			// send callback for the timer
			it->second();
			// pop it off the list
			timers.pop_front();
		}
		else
			break;	// no point checking further, as those timers don't need stopped

		it = timers.begin();
	}	

	// check scaled timers
	it = scaledTimers.begin();

	while (it != scaledTimers.end())
	{
		if (scaledUpTime >= it->first)
		{
			// send callback for the timer
			it->second();
			// pop it off the list
			scaledTimers.pop_front();
		}
		else
			return;	// no point checking further, as those timers don't need stopped

		it = scaledTimers.begin();
	}

	double frameTime = privGetUnscaledFrameTime();

	if (frameTime > 0.0)
		fps = 1.0 / frameTime;
	else
		fps = 0.0;

}
