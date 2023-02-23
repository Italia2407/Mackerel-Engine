#include "InputManager.h"

namespace MCK::Input
{
	InputManager::InputManager()
	{
	}

	InputManager::~InputManager()
	{
	}

	void InputManager::privSubscribe(Key keyVal, std::function<void> callback)
	{
		// 1. Add a new list if the key hasn't been subscribe to before
		if (callbacks.contains(keyVal) == false)
			callbacks.emplace(keyVal, callbackList());

		// 2. add the callback
		callbacks[keyVal].push_back(callback);

		// 3. ???

		// 4. profit
	}
}