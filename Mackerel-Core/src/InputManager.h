#pragma once

#include <functional>
#include <GLFW/glfw3.h>
#include <list>
#include <map>

#include "KeyEvents.h"
#include "KeyHandler.h"
#include "Keys.h"
#include "InputSubReceipt.h"

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
			InputSubReceipt* privSubscribe(Key key, KeyEvent event, callbackFunc& callback, InputSubReceipt* receipt = nullptr);
			void privUnsubscribe(InputSubReceipt* receipt);

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
			static InputSubReceipt* Subscribe(Key key, KeyEvent event, callbackFunc& callback, InputSubReceipt* receipt = nullptr)
			{
				return Instance()->privSubscribe(key, event, callback, receipt);
			}

			/**
			 * Removes the given callback function for the associated key and key event.
			 * 
			 * \param key: the relevant key
			 * \param event: the relevant key event
			 * \param callback: the callback function to be deregistered
			 */
			static void Unsubscribe(InputSubReceipt* receipt)
			{
				Instance()->privUnsubscribe(receipt);
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
