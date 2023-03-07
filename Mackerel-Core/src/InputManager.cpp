#include "InputManager.h"

namespace MCK::Input
{
	InputManager* InputManager::instance = nullptr;

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

	InputSubReceipt* InputManager::privSubscribe(Key key, KeyEvent event, callbackFunc& callback, InputSubReceipt* receipt)
	{
		KeyHandler* curHandler = CheckMake(key);
		return curHandler->Register(event, callback, receipt);
	}

	void InputManager::privUnsubscribe(InputSubReceipt* receipt)
	{
		for (auto handler : callbacks)
		{
			handler.second->Deregister(receipt);
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