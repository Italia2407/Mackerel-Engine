#pragma once

#include <functional>
#include <GLFW/glfw3.h>
#include <list>
#include <map>

#include "Keys.h"

namespace MCK::Input
{
	using callbackList = std::list<std::function<void>>;

	class InputManager
	{
		private:
			InputManager();
			~InputManager();

			// we can have one input manager, as a treat
			InputManager(const InputManager&) = delete;
			InputManager& operator=(const InputManager&) = delete;

			// singleton bookkeeping
			static InputManager* instance;
			static InputManager* Instance()
			{
				if (!instance)
					instance = new InputManager;
				return instance;
			}

			// private member variables
			std::map<Key, callbackList> callbacks;

			// private implementations of static functions
			void privSubscribe(Key keyVal, std::function<void> callback);

			void privCheckKeys();

		public:
			/**
			 * Adds the given function as a callback for when the given key is pressed.
			 * 
			 * \param keyVal: the key that should trigger the callback
			 * \param callback: the callback function
			 */
			static void Subscribe(Key keyVal, std::function<void> callback)
			{
				Instance()->privSubscribe(keyVal, callback);
			}
	};
}
