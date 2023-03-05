#include "InputManager.h"

namespace MCK::Input
{
	InputManager::InputManager()
	{
	}

	InputManager::~InputManager()
	{
	}

	KeyHandler* InputManager::CheckMake(Key key)
	{
		if (callbacks.contains(key) == false)
			callbacks.emplace(key, new KeyHandler(key));

		return callbacks[key];
	}

	void InputManager::CheckDemake(Key key)
	{
		if (callbacks.contains(key) && callbacks[key]->Empty())
		{
			delete callbacks[key];
			callbacks.erase(key);
		}
	}

	void InputManager::CheckDemakeUnsafe(Key key)
	{
		if (callbacks[key]->Empty())
		{
			delete callbacks[key];
			callbacks.erase(key);
		}
	}

	void InputManager::privSubscribe(Key key, KeyEvent event, callbackFunc& callback)
	{
		KeyHandler* curHandler = CheckMake(key);
		curHandler->Register(event, callback);
	}

	void InputManager::privUnsubscribe(Key key, KeyEvent event, callbackFunc& callback)
	{
		if (callbacks.contains(key))
		{
			KeyHandler* curHandler = callbacks[key];
			curHandler->Deregister(event, callback);
			CheckDemakeUnsafe(key);
		}
	}

	void InputManager::privCheckKeys(GLFWwindow* window)
	{
		for (auto handler : callbacks)
		{
			handler.second->CheckState(window);
		}
	}
}