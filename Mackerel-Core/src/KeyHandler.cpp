#include "KeyHandler.h"

MCK::Input::KeyHandler::KeyHandler(Key key)
	: key(key), lastState(KeyEvent::Null)
{}

MCK::Input::KeyHandler::~KeyHandler()
{
	// Not strictly necessary but w/e
	Clear();
}

void MCK::Input::KeyHandler::CheckState(GLFWwindow* window)
{
	// glfw will return either pressed or released
	KeyEvent newState = static_cast<KeyEvent>(glfwGetKey(window, static_cast<int>(key)));

	if (newState == KeyEvent::Pressed)
	{
		if (lastState == KeyEvent::Released)
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
		if (lastState == KeyEvent::Pressed)
		{
			onReleased();
		}
	}

	lastState = newState;
}

void MCK::Input::KeyHandler::onPressed()
{
	for (auto func : pressedList)
		func();
}

void MCK::Input::KeyHandler::onReleased()
{
	for (auto func : releasedList)
		func();
}

void MCK::Input::KeyHandler::onHeld()
{
	for (auto func : heldList)
		func();
}

void MCK::Input::KeyHandler::Register(KeyEvent event, callbackFunc callback)
{
	switch (event)
	{
		case KeyEvent::Pressed:
			if (std::find(pressedList.begin(), pressedList.end(), callback) == pressedList.end())
				pressedList.push_back(callback);
			break;

		case KeyEvent::Released:
			if (std::find(releasedList.begin(), releasedList.end(), callback) == releasedList.end())
				releasedList.push_back(callback);
			break;

		case KeyEvent::Held:
			if (std::find(heldList.begin(), heldList.end(), callback) == heldList.end())
				heldList.push_back(callback);
			break;

		default:
			break;
	}
}

void MCK::Input::KeyHandler::Deregister(KeyEvent event, callbackFunc callback)
{
	callbackList::iterator itt;

	switch (event)
	{
	case KeyEvent::Pressed:
		itt = std::find(pressedList.begin(), pressedList.end(), callback);
		if (itt != pressedList.end())
			pressedList.erase(itt);
		break;

	case KeyEvent::Released:
		itt = std::find(releasedList.begin(), releasedList.end(), callback);
		if (itt != releasedList.end())
			releasedList.erase(itt);
		break;

	case KeyEvent::Held:
		itt = std::find(heldList.begin(), heldList.end(), callback);
		if (itt != heldList.end())
			heldList.erase(itt);
		break;

	default:
		break;
	}
}

bool MCK::Input::KeyHandler::Empty()
{
	return pressedList.size() == releasedList.size() == heldList.size() == 0;
}

void MCK::Input::KeyHandler::Clear()
{
	pressedList.clear();
	releasedList.clear();
	heldList.clear();
}
