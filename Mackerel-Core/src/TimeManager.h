#pragma once

#include <map>
#include <functional>

namespace MCK
{
	/**
	 * This class is the time manager responsible for keeping track
	 * of different timers, and general time keeping.
	 */
	class TimeManager
	{
	private:
		//private constructor/destructor because singleton
		TimeManager();
		~TimeManager();

		// singleton bookkeeping

		static TimeManager* instance;
		static TimeManager* Instance()
		{
			if (!instance)
				instance = new TimeManager;
			return instance;
		}

		// private member variables

		double timescale;
		double lastFrame;
		// list of key-value pairs that holds the timers
		std::list<std::pair<double, std::function<void()>>> timers;

		// private implemenetations of Singleton functions

		/**
		 * Private implementation of the TimeManager::GetUpTime() function.
		 * Gets the total up time of the application aka the value of the GLFW timer, which
		 *	has elapsed since GLFW was initialized, or set using glfwSetTime.
		 *
		 * \return the value of the GLFW timer as a double
		 */
		double privGetUpTime();

		/**
		 * Private implemenetation of the TimeManager::setTimer() function.
		 * Attempts to create a timer, by correctly adding it to the list of timers
		 *	based on its end time.
		 *
		 * \param timer: key-value pair, where the first value is the end time and the
		 *		second is the callback function
		 * \return true if timer successfully set or false if it fails
		 */
		void privSetTimer(std::pair<double, std::function<void()>> timer);

		/**
		 * Private implementation of the TimeManager::setTimescale() function.
		 * Attempts to change the timescale to the new scale provided by the user.
		 *
		 * \param scale: value the user wants to change the timescale to.
		 * \return true if timescale successfully changed, false if it fails
		 */
		void privSetTimescale(double scale);

		/**
		 * Private implementation of the TimeManager::getFrameTime() function.
		 * Function to get the time taken between frames scaled by the timescale.
		 *
		 * \return scaled time since the last frame
		 */
		double privGetFrameTime();

		/**
		 * Private implementation of the TimeManager::getUnscaledFrameTime() function.
		 * Function to get the time taken between frames without it having been scaled.
		 *
		 * \return time since the last frame, without scaling
		 */
		double privGetUnscaledFrameTime();

		/**
		 * Private implementation of the TimeManager::Update() fuction.
		 * Update should get called every frame.
		 * Removes all finished timers, sends callbacks for each of them.
		 *
		 */
		void privUpdate();


	public:
		/**
		 * Releases all timers in memory, and frees the TimeManager instance.
		 * Should only be called at the end of the applications lifetime!
		 *
		 * \return true if the manager was successfully released, false if it
		 *	wasn't initialised to begin with
		 */
		bool Release();

		/**
		 * Gets the total up time of the application.
		 *
		 * \return up time as a double
		 */
		double GetUpTime()
		{
			return Instance()->privGetUpTime();
		}

		/**
		 * Attempts to create a new timer by passing the information to the time manager.
		 *
		 * \param timer: key-value pair of end time and callback function
		 * \return true if timer has been successfully created, false if it fails
		 */
		void setTimer(std::pair<double, std::function<void()>> timer)
		{
			Instance()->privSetTimer(timer);
		}

		/**
		 * Attempts to change the timescale to the new scale provided by the user.
		 *
		 * \param scale: value the user wants to change the timescale to.
		 * \return true if timescale successfully changed, false if it fails.
		 */
		void setTimescale(double scale)
		{
			Instance()->privSetTimescale(scale);
		}

		/**
		 * Function to get the time taken between frames scaled by the timescale.
		 *
		 * \return scaled time since the last frame
		 */
		double getFrameTime()
		{
			return Instance()->privGetFrameTime();
		}

		/**
		 * Function to get the time taken between frames without it having been scaled.
		 *
		 * \return time since the last frame, without scaling
		 */
		double getUnscaledFrameTime()
		{
			return Instance()->privGetUnscaledFrameTime();
		}

		/**
		 * Update should get called every frame.
		 * Removes all finished timers, sends callbacks for each of them.
		 *
		*/
		void Update()
		{
			Instance()->privUpdate();
		}

	};
}

