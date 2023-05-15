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

		// not to be copied!
		TimeManager(const TimeManager&) = delete;
		TimeManager& operator=(const TimeManager&) = delete;

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
		double lastScaledFrame;
		double upTime;
		double scaledUpTime;

		// list of key-value pairs that holds the timers
		std::list<std::pair<double, std::function<void()>>> timers;
		std::list<std::pair<double, std::function<void()>>> scaledTimers;

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
		 * Private implementation of the TimeManager::GetScaledUpTime() function.
		 * Gets the total scaled up time of the application, which is tracked by scaledUpTime.
		 *
		 * \return the value of scaledUpTime
		 */
		double privGetScaledUpTime();

		/**
		 * Private implemenetation of the TimeManager::setTimer() function.
		 * Creates a timer, by correctly adding it to the list of timers
		 *	based on its end time, which is calculated by adding the given time to
		 *	the current up time.
		 *
		 * \param timer: key-value pair, where the first value is the end time and the
		 *		second is the callback function
		 */
		void privSetTimer(std::pair<double, std::function<void()>> timer);

		/**
		 * Private implemenetation of the TimeManager::setScaledTimer() function.
		 * Creates a scaled timer, by correctly adding it to the list of scaled
		 *	timers based on its end time, which is calculated by adding the given time to
		 *	the current scaled up time.
		 *
		 * \param timer: key-value pair, where the first value is the end time and the
		 *		second is the callback function
		 */
		void privSetScaledTimer(std::pair<double, std::function<void()>> timer);

		/**
		 * Private implementation of the TimeManager::setTimescale() function.
		 * Changes the timescale to the new scale provided by the user.
		 *
		 * \param scale: value the user wants to change the timescale to.
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

		/**
		 * Initiates a sleep to enforce a minimum frame time.
		 */
		void privWaitForEnforcedFrameTime(double minFrameTime);


	public:
		/**
		 * Releases all timers in memory, and frees the TimeManager instance.
		 * Should only be called at the end of the applications lifetime!
		 *
		 * \return true if the manager was successfully released, false if it
		 *	wasn't initialised to begin with
		 */
		static bool Release();

		/**
		 * Gets the total up time of the application.
		 *
		 * \return up time as a double
		 */
		static double GetUpTime()
		{
			return Instance()->privGetUpTime();
		}

		/**
		 * Gets the total scaled up time of the application.
		 * 
		 * \return scaled up time as a double
		 */
		static double GetScaledUpTime()
		{
			return Instance()->privGetScaledUpTime();
		}

		/**
		 * Creates a new timer by passing the information to the time manager.
		 *
		 * \param timer: key-value pair of end time and callback function
		 */
		static void setTimer(std::pair<double, std::function<void()>> timer)
		{
			Instance()->privSetTimer(timer);
		}

		/**
		 * Creates a new scaled timer by passing the information to the time manager.
		 * 
		 * \param timer: key-value pair of end time and callback function
		 */
		static void setScaledTimer(std::pair<double, std::function<void()>> timer)
		{
			Instance()->privSetScaledTimer(timer);
		}

		/**
		 * Changes the timescale to the new scale provided by the user.
		 *
		 * \param scale: value the user wants to change the timescale to.
		 */
		static void setTimescale(double scale)
		{
			Instance()->privSetTimescale(scale);
		}

		/**
		 * Function to get the time taken between frames scaled by the timescale.
		 *
		 * \return scaled time since the last frame
		 */
		static double getScaledFrameTime()
		{
			return Instance()->privGetFrameTime();
		}

		/**
		 * Function to get the time taken between frames without it having been scaled.
		 *
		 * \return time since the last frame, without scaling
		 */
		static double getFrameTime()
		{
			return Instance()->privGetUnscaledFrameTime();
		}

		/**
		 * Update should get called every frame.
		 * Removes all finished timers, sends callbacks for each of them.
		 *
		*/
		static void Update()
		{
			Instance()->privUpdate();
		}

	};
}

