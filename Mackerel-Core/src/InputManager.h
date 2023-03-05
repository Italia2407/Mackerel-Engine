#pragma once

#include <functional>
#include <GLFW/glfw3.h>
#include <list>
#include <map>

#include "KeyEvents.h"
#include "KeyHandler.h"
#include "Keys.h"

namespace MCK::Input
{
	class InputManager
	{
		using callbackMap = std::map<Key, KeyHandler*>;

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
			callbackMap callbacks;

			// private member functions
			/**
			 * Instatiates the associated key handler if it doesn't exist.
			 * 
			 * \param key: the associated key.
			 *
			 * \return the associated key handler.
			 */
			KeyHandler* CheckMake(Key key);
			/**
			 * Deletes the associated key handler if it is empty.
			 *
			 * \param key: the associated key.
			 */
			void CheckDemake(Key key);
			/**
			 * Deletes the associated key handler if it is empty, but doesn't check if
			 *     it exists first.
			 *
			 * \param key: the associated key.
			 */
			void CheckDemakeUnsafe(Key key);

			// private implementations of static functions
			void privSubscribe(Key key, KeyEvent event, callbackFunc& callback);
			void privUnsubscribe(Key key, KeyEvent event, callbackFunc& callback);

			void privCheckKeys(GLFWwindow* window);

		public:
			/**
			 * Adds the given function as a callback for when the given event occurs
			 *     with the given key.
			 * 
			 * \param key: the key that should trigger the callback
			 * \param event: the key event that should trigger the callback
			 * \param callback: the callback function
			 */
			static void Subscribe(Key key, KeyEvent event, callbackFunc& callback)
			{
				Instance()->privSubscribe(key, event, callback);
			}

			/**
			 * Removes the given callback function for the associated key and key event.
			 * 
			 * \param key: the relevant key
			 * \param event: the relevant key event
			 * \param callback: the callback function to be deregistered
			 */
			static void Unsubscribe(Key key, KeyEvent event, callbackFunc& callback)
			{
				Instance()->privUnsubscribe(key, event, callback);
			}

			/**
			 * Checks the states of tracked keys and envokes relevant callbacks.
			 * 
			 * \param window: the glfw window to query the key states from.
			 */
			static void CheckKeys(GLFWwindow* window)
			{
				Instance()->privCheckKeys(window);
			}
	};
}
