#pragma once

// The header to include to utilise the input system.

#include <functional>

#include "Keys.h"
#include "KeyEvents.h"
#include "InputManager.h"
#include "InputSubReceipt.h"

namespace MCK::Input
{
	/**
	 * Adds the given function as a callback for when the given event occurs
	 *     with the given key.
	 *
	 * \param key: the key that should trigger the callback
	 * \param event: the key event that should trigger the callback
	 * \param callback: the callback function
	 * \param receipt(optional): if not null, the given receipt will be appended to instead of returning a new one
	 */
	InputSubReceipt* Subscribe(Key key, KeyEvent event, callbackFunc& callback, InputSubReceipt* receipt = nullptr)
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
		InputManager::CheckKeys(window);
	}
}
