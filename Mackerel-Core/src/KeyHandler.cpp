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
		func(key, KeyEvent::Pressed);
}

void MCK::Input::KeyHandler::onReleased()
{
	for (auto func : releasedList)
		func(key, KeyEvent::Released);
}

void MCK::Input::KeyHandler::onHeld()
{
	for (auto func : heldList)
		func(key, KeyEvent::Held);
}

MCK::Input::InputSubReceipt* MCK::Input::KeyHandler::Register(KeyEvent event, callbackFunc callback, InputSubReceipt* receipt)
{
	InputSubReceipt* ret;

	if (receipt)
	{
		ret = receipt;
		if (ret->ContainsKey(key) == false)
			ret->data.emplace(key, InputSubReceipt::IttTriplet());
	}
	else
	{
		ret = new InputSubReceipt;
		ret->data.emplace(key, InputSubReceipt::IttTriplet());
	}

	switch (event)
	{
		case KeyEvent::Pressed:
			ret->GetItt(key).pressed_iterators.push_back(pressedList.insert(pressedList.end(), callback));
			break;

		case KeyEvent::Released:
			ret->GetItt(key).released_iterators.push_back(releasedList.insert(releasedList.end(), callback));
			break;

		case KeyEvent::Held:
			ret->GetItt(key).held_iterators.push_back(heldList.insert(heldList.end(), callback));
			break;

		case KeyEvent::All:
			ret->GetItt(key).pressed_iterators.push_back(pressedList.insert(pressedList.end(), callback));
			ret->GetItt(key).released_iterators.push_back(releasedList.insert(releasedList.end(), callback));
			ret->GetItt(key).held_iterators.push_back(heldList.insert(heldList.end(), callback));
			break;

		default:
			break;
	}

	return ret;
}

void MCK::Input::KeyHandler::Deregister(InputSubReceipt* receipt)
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
