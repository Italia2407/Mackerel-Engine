#include "TimeManager.h"
#include <GLFW/glfw3.h>

using namespace MCK;

TimeManager* TimeManager::instance = nullptr;

TimeManager::TimeManager()
{
	timescale = 0.0;
	lastFrame = glfwGetTime();
}

TimeManager::~TimeManager() {}


// private Singleton functions
bool MCK::TimeManager::Release()
{
	bool success = false;

	if (instance)
	{
		timers.clear();
		delete instance;
		success = true;
	}

	return success;
}


double MCK::TimeManager::privGetUpTime()
{
	// returns the value of the GLFW timer
	return glfwGetTime();
}


void MCK::TimeManager::privSetTimer(std::pair<double, std::function<void()>> timer)
{
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


void MCK::TimeManager::privSetTimescale(double scale)
{
	timescale = scale;
}


double MCK::TimeManager::privGetFrameTime()
{
	return (glfwGetTime() - lastFrame) * timescale;
}


double MCK::TimeManager::privGetUnscaledFrameTime()
{
	return glfwGetTime() - lastFrame;
}


void MCK::TimeManager::privUpdate()
{

}
