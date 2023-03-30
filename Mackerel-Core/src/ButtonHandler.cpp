#include "ButtonHandler.h"

#include "InputManager.h"

MCK::Input::ButtonHandler::ButtonHandler(int32_t key)
	: key(key), lastState(ButtonEvents::Null)
{
	if (key >= MKC_KEY_OFFSET && key < MKC_KEY_OFFSET + MCK_KEY_SPAN)
	{
		inputCheck = std::bind(&ButtonHandler::CheckKey, this, std::placeholders::_1, std::placeholders::_2);
	}
	if (key >= MCK_GP_OFFSET && key < MCK_GP_OFFSET + MCK_GP_SPAN)
	{
		inputCheck = std::bind(&ButtonHandler::CheckGamepadButton, this, std::placeholders::_1, std::placeholders::_2);
	}
	if (key >= MCK_MOUSE_OFFSET && key < MCK_MOUSE_OFFSET + MCK_MOUSE_SPAN)
	{
		inputCheck = std::bind(&ButtonHandler::CheckMouseButton, this, std::placeholders::_1, std::placeholders::_2);
	}
}

MCK::Input::ButtonHandler::~ButtonHandler()
{
	// Not strictly necessary but w/e
	Clear();
}

void MCK::Input::ButtonHandler::CheckState(GLFWwindow* window)
{
	// glfw will return either pressed or released
	ButtonEvents newState = inputCheck(window, key);

	if (newState == ButtonEvents::Pressed)
	{
		if (lastState == ButtonEvents::Released)
		{
			onPressed();
		}
		else
		{
			onHeld();
		}
	}
	else
	{
		if (lastState == ButtonEvents::Pressed)
		{
			onReleased();
		}
	}

	lastState = newState;
}

void MCK::Input::ButtonHandler::onPressed()
{
	for (auto func : pressedList)
		func(key, ButtonEvents::Pressed);
}

void MCK::Input::ButtonHandler::onReleased()
{
	for (auto func : releasedList)
		func(key, ButtonEvents::Released);
}

void MCK::Input::ButtonHandler::onHeld()
{
	for (auto func : heldList)
		func(key, ButtonEvents::Held);
}

bool MCK::Input::ButtonHandler::Register(ButtonEvents event, callbackFunc callback, InputSubReceipt* receipt)
{
	bool ret = false;

	if (receipt)
	{
		ret = true;

		if (receipt->ContainsKey(key) == false)
			receipt->data.emplace(key, InputSubReceipt::IttTriplet());

		switch (event)
		{
			case ButtonEvents::Pressed:
				receipt->GetItt(key).pressed_iterators.push_back(pressedList.insert(pressedList.end(), callback));
				break;

			case ButtonEvents::Released:
				receipt->GetItt(key).released_iterators.push_back(releasedList.insert(releasedList.end(), callback));
				break;

			case ButtonEvents::Held:
				receipt->GetItt(key).held_iterators.push_back(heldList.insert(heldList.end(), callback));
				break;

			case ButtonEvents::All:
				receipt->GetItt(key).pressed_iterators.push_back(pressedList.insert(pressedList.end(), callback));
				receipt->GetItt(key).released_iterators.push_back(releasedList.insert(releasedList.end(), callback));
				receipt->GetItt(key).held_iterators.push_back(heldList.insert(heldList.end(), callback));
				break;

			default:
				break;
		}
	}

	return ret;
}

void MCK::Input::ButtonHandler::Deregister(InputSubReceipt* receipt)
{
	if (receipt->ContainsKey(key))
	{
		for (auto itt : receipt->GetItt(key).pressed_iterators)
			pressedList.erase(itt);

		for (auto itt : receipt->GetItt(key).released_iterators)
			releasedList.erase(itt);

		for (auto itt : receipt->GetItt(key).held_iterators)
			heldList.erase(itt);

		receipt->GetItt(key).pressed_iterators.clear();
		receipt->GetItt(key).released_iterators.clear();
		receipt->GetItt(key).held_iterators.clear();
	}
}

bool MCK::Input::ButtonHandler::Empty()
{
	return pressedList.size() == releasedList.size() == heldList.size() == 0;
}

void MCK::Input::ButtonHandler::Clear()
{
	pressedList.clear();
	releasedList.clear();
	heldList.clear();
}

MCK::ButtonEvents MCK::Input::ButtonHandler::CheckKey(GLFWwindow* window, int32_t key)
{
	return static_cast<ButtonEvents>(glfwGetKey(window, static_cast<int>(key - MKC_KEY_OFFSET)));
}

MCK::ButtonEvents MCK::Input::ButtonHandler::CheckGamepadButton(GLFWwindow* window, int32_t key)
{
	return static_cast<ButtonEvents>(InputManager::GetGamepadState().buttons[key - MCK_GP_OFFSET]);
}

MCK::ButtonEvents MCK::Input::ButtonHandler::CheckMouseButton(GLFWwindow* window, int32_t key)
{
	return static_cast<ButtonEvents>(glfwGetMouseButton(window, key - MCK_MOUSE_OFFSET));
}

MCK::ButtonEvents MCK::Input::ButtonHandler::CheckError(GLFWwindow* window, int32_t key)
{
	return MCK::ButtonEvents::Null;
}
