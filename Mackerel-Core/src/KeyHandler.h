#pragma once


#include <functional>
#include <GLFW/glfw3.h>
#include <list>

#include "Keys.h"
#include "KeyEvents.h"

namespace MCK::Input
{
	using callbackFunc = std::function<void()>;
	using callbackList = std::list<callbackFunc>;

	class KeyHandler
	{
		public:
			KeyHandler(Key key);
			~KeyHandler();

			// no default constructor, needs to be associated with a key.
			KeyHandler() = delete;

			// no copying
			KeyHandler(const KeyHandler&) = delete;
			KeyHandler& operator=(const KeyHandler&) = delete;

			// public member functions
			/**
			 * Checks the relevant key state for the current window and calls relevant
			 *     callback functions.
			 * 
			 * \param window: the glfw window to query the key state from.
			 */
			void CheckState(GLFWwindow* window);

			/**
			 * Registers the given callback function for the given key event with this KeyHandler.
			 * 
			 * \param event: the key event that should envoke the callback.
			 * \param callback: the callback function to be envoked.
			 */
			void Register(KeyEvent event, callbackFunc callback);

			/**
			 * Deregisters the givencallback.
			 * 
			 * \param event: the associated event to deregister the callback from.
			 * \param callback: the callback to deregister.
			 */
			void Deregister(KeyEvent event, callbackFunc callback);

			/**
			 * Checks if the KeyHandler has any associated callbacks.
			 * 
			 * \return true if the KeyHandler has no associated callbacks.
			 */
			bool Empty();

			/**
			 * Clears all callbacks associated with the KeyHandler.
			 * 
			 */
			void Clear();

		private:
			Key key;
			KeyEvent lastState;
			callbackList pressedList;
			callbackList releasedList;
			callbackList heldList;

			// private member functions
			void onPressed();
			void onReleased();
			void onHeld();
	};
}