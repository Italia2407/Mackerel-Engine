#pragma once


#include <functional>
#include <GLFW/glfw3.h>
#include <list>
#include <vector>

#include "Keys.h"
#include "KeyEvents.h"
#include "InputSubReceipt.h"

namespace MCK::Input
{
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
			 * \param receipt (optional): if not null, the given receipt is appended to instead of returning a new one.
			 */
			InputSubReceipt* Register(KeyEvent event, callbackFunc callback, InputSubReceipt* receipt = nullptr);

			/**
			 * Deregisters the givencallback.
			 * 
			 * \param event: the associated event to deregister the callback from.
			 * \param callback: the callback to deregister.
			 */
			void Deregister(InputSubReceipt* receipt);

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