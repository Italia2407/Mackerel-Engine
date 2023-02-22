#pragma once

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

		// private implemenetations of Singleton functions

	public:



	};
}
