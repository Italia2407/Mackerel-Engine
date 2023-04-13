#pragma once


#include <functional>
#include "GLinclude.h"
#include <list>
#include <vector>

#include "Keys.h"
#include "ButtonEvents.h"
#include "InputSubReceipt.h"

namespace MCK::Input
{
	class ButtonHandler
	{
		public:
			ButtonHandler(int32_t key);
			~ButtonHandler();

			// no default constructor, needs to be associated with a key.
			ButtonHandler() = delete;

			// no copying
			ButtonHandler(const ButtonHandler&) = delete;
			ButtonHandler& operator=(const ButtonHandler&) = delete;

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
			 * \param receipt: input receipt to append this callback instance too, used for deregistration.
			 */
			bool Register(ButtonEvents event, callbackFunc callback, InputSubReceipt* receipt);

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
			int32_t key;
			ButtonEvents lastState;
			callbackList pressedList;
			callbackList releasedList;
			callbackList heldList;
			
			std::function<ButtonEvents(GLFWwindow*, int32_t)> inputCheck;

			// private member functions
			void onPressed();
			void onReleased();
			void onHeld();

			ButtonEvents CheckKey(GLFWwindow* window, int32_t key);
			ButtonEvents CheckGamepadButton(GLFWwindow* window, int32_t key);
			ButtonEvents CheckMouseButton(GLFWwindow* window, int32_t key);
			ButtonEvents CheckError(GLFWwindow* window, int32_t key);
	};
}