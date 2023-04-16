#pragma once

#include <functional>
#include "GLinclude.h"
#include <list>
#include <map>

#include "ButtonEvents.h"
#include "ButtonHandler.h"
#include "Keys.h"
#include "InputSubReceipt.h"
#include "MouseState.h"

namespace MCK::Input
{
	class InputManager
	{
		using callbackMap = std::map<int32_t, ButtonHandler*>;

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
			GLFWgamepadstate gamepadState{};
			MouseState mouseState{};

			// private member functions
			/**
			 * Instatiates the associated key handler if it doesn't exist.
			 * 
			 * \param key: the associated key.
			 *
			 * \return the associated key handler.
			 */
			ButtonHandler* CheckMake(int32_t key);
			/**
			 * Deletes the associated key handler if it is empty.
			 *
			 * \param key: the associated key.
			 */
			void CheckDemake(int32_t key);
			/**
			 * Deletes the associated key handler if it is empty, but doesn't check if
			 *     it exists first.
			 *
			 * \param key: the associated key.
			 */
			void CheckDemakeUnsafe(int32_t key);

			// private implementations of static functions
			bool privSubscribe(int32_t key, ButtonEvents event, callbackFunc& callback, InputSubReceipt* receipt = nullptr);
			void privUnsubscribe(InputSubReceipt* receipt);

			void privUpdate(GLFWwindow* window);

			const GLFWgamepadstate& privGetGamepadState() const;
			const MouseState& privGetMouseState() const;

		public:
			/**
			 * Adds the given function as a callback for when the given event occurs
			 *     with the given key.
			 * 
			 * \param key: the key that should trigger the callback
			 * \param event: the key event that should trigger the callback
			 * \param callback: the callback function
			 * \param receipt: the receipt this callback will be appended to, used for deregistration
			 */
			static bool Subscribe(int32_t key, ButtonEvents event, callbackFunc& callback, InputSubReceipt* receipt = nullptr)
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
			static void Update(GLFWwindow* window)
			{
				Instance()->privUpdate(window);
			}

			/**
			 * Returns the state of the gamepad. If there is no gamepad connected every button will return released.
			 * 
			 * \return a reference to the current gamepad state.
			 */
			static const GLFWgamepadstate& GetGamepadState()
			{
				return Instance()->privGetGamepadState();
			}

			/**
			 * Returns the positional state of the mouse, use a button callback to handle mouse button input.
			 *
			 * \return a reference to the current gamepad state.
			 */
			static const MouseState& GetMouseState()
			{
				return Instance()->privGetMouseState();
			}
	};
}
