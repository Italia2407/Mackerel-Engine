#pragma once

// The header to include to utilise the input system.

#include <cstdint>
#include <functional>

#include "Keys.h"
#include "ButtonEvents.h"
#include "InputManager.h"
#include "InputSubReceipt.h"
#include "GamepadButtons.h"
#include "MouseButtons.h"

namespace MCK
{
	using InputCallback = std::function<void(int32_t, MCK::ButtonEvents)>;
}

namespace MCK::Input
{
	/**
	 * Adds the given function as a callback for when the given event occurs
	 *     with the given key.
	 *
	 * \param key: the key that should trigger the callback
	 * \param event: the key event that should trigger the callback
	 * \param callback: the callback function
	 * \param receipt: the receipt to append this callback to, used for deregistration
	 */
	bool Subscribe(int32_t key, ButtonEvents event, callbackFunc& callback, InputSubReceipt* receipt)
	{
		return InputManager::Subscribe(key, event, callback, receipt);
	}

	/**
	 * Removes the given callback function for the associated key and key event.
	 *
	 * \param receipt: the receipt of subscriptions to be unsubscribed
	 */
	void Unsubscribe(InputSubReceipt* receipt)
	{
		InputManager::Unsubscribe(receipt);
	}

	/**
	 * Checks the states of tracked keys and envokes relevant callbacks.
	 *
	 * \param window: the glfw window to query the key states from.
	 */
	void Update(GLFWwindow* window)
	{
		InputManager::Update(window);
	}
}
