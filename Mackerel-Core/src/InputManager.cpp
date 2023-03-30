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

	ButtonHandler* InputManager::CheckMake(int32_t key)
	{
		if (callbacks.contains(key) == false)
			callbacks.emplace(key, new ButtonHandler(key));

		return callbacks[key];
	}

	void InputManager::CheckDemake(int32_t key)
	{
		if (callbacks.contains(key) && callbacks[key]->Empty())
		{
			delete callbacks[key];
			callbacks.erase(key);
		}
	}

	void InputManager::CheckDemakeUnsafe(int32_t key)
	{
		if (callbacks[key]->Empty())
		{
			delete callbacks[key];
			callbacks.erase(key);
		}
	}

	bool InputManager::privSubscribe(int32_t key, ButtonEvents event, callbackFunc& callback, InputSubReceipt* receipt)
	{
		if (receipt == nullptr)
		{
			return false;
		}
		else
		{
			ButtonHandler* curHandler = CheckMake(key);
			return curHandler->Register(event, callback, receipt);
		}
	}

	void InputManager::privUnsubscribe(InputSubReceipt* receipt)
	{
		for (auto handler : callbacks)
		{
			handler.second->Deregister(receipt);
		}
	}

	void InputManager::privUpdate(GLFWwindow* window)
	{
		// gamepad update
		if (glfwJoystickPresent(GLFW_JOYSTICK_1) != 0)
		{
			glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepadState);
		}
		else
		{
			memset(&gamepadState, 0, sizeof(gamepadState));
		}
		
		// mouse update
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		mouseState.deltaX = static_cast<float>(mouseX) - mouseState.posX;
		mouseState.deltaY = static_cast<float>(mouseY) - mouseState.posY;
		mouseState.posX = static_cast<float>(mouseX);
		mouseState.posY = static_cast<float>(mouseY);
		
		// check button handlers
		for (auto handler : callbacks)
		{
			handler.second->CheckState(window);
		}
	}

	const GLFWgamepadstate& InputManager::privGetGamepadState() const
	{
		return gamepadState;
	}
	const MouseState& InputManager::privGetMouseState() const
	{
		return mouseState;
	}
}