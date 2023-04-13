#include "InputComponent.h"

#include "Entity.h"

#include "Input.h"

namespace MCK::EntitySystem
{
	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef InputComponent::GetType()
	{
		return typeid(InputComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void InputComponent::OnCreate()
	{
		// Create button input callback
		buttonInputCallback = std::bind(&InputComponent::MovingPlayerCallback, this, std::placeholders::_1, std::placeholders::_2); /* <- syntax for member functions */

		// Create input subscriptions for keyboard
		MCK::Input::Subscribe(MCK::Key::W, MCK::ButtonEvents::Held, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::A, MCK::ButtonEvents::Held, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::D, MCK::ButtonEvents::Held, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::S, MCK::ButtonEvents::Held, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::W, MCK::ButtonEvents::Released, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::A, MCK::ButtonEvents::Released, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::D, MCK::ButtonEvents::Released, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::S, MCK::ButtonEvents::Released, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::SPACE, MCK::ButtonEvents::Pressed, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::SPACE, MCK::ButtonEvents::Held, buttonInputCallback, &receipt);
		MCK::Input::Subscribe(MCK::Key::SPACE, MCK::ButtonEvents::Released, buttonInputCallback, &receipt);
	}

	/**
	 * Callback to update variables
	 *
	 */
	void InputComponent::MovingPlayerCallback(int32_t key, MCK::ButtonEvents ButtonEvents)
	{
		float moveSpeed = 1.0f; // You can adjust this value to change the movement speed

		if (ButtonEvents == MCK::ButtonEvents::Held)
		{
			if (key == MCK::Key::W)
			{
				direction.y() -= moveSpeed;
			}
			else if (key == MCK::Key::S)
			{
				direction.y() += moveSpeed;
			}
			else if (key == MCK::Key::A)
			{
				direction.x() -= moveSpeed;
			}
			else if (key == MCK::Key::D)
			{
				direction.x() += moveSpeed;
			}
			else if (key == MCK::Key::SPACE)
			{
				jumpHeld = true;
			}
		}
		else if (ButtonEvents == MCK::ButtonEvents::Released)
		{
			if (key == MCK::Key::W)
			{
				direction.y() += moveSpeed;
			}
			else if (key == MCK::Key::S)
			{
				direction.y() -= moveSpeed;
			}
			else if (key == MCK::Key::A)
			{
				direction.x() += moveSpeed;
			}
			else if (key == MCK::Key::D)
			{
				direction.x() -= moveSpeed;
			}
			else if (key == MCK::Key::SPACE)
			{
				jumpHeld = false;
			}
		}
		else if (ButtonEvents == MCK::ButtonEvents::Pressed)
		{
			if (key == MCK::Key::SPACE)
			{
				jumpPressed = true;
			}
		}

		// Normalize the direction vector
		if (direction.norm() > 0)
		{
			direction.normalize();
		}

		std::cout << "k" << key << " b" << (int)ButtonEvents << " x" << direction.x() << " y" << direction.y() << std::endl;
	}


	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void InputComponent::OnUpdate()
	{

		// query gamepad state
		auto gamepadState = Input::GamepadState(); 

		// Update direction based on gamepad axes
		direction.x() = gamepadState.axes[MCK::GamepadAxes::LEFT_X];
		direction.y() = gamepadState.axes[MCK::GamepadAxes::LEFT_Y];

		// Normalize the direction vector
		if (direction.norm() > 0)
		{
			direction.normalize();
		}

		// Update jump held and jump pressed based on gamepad buttons
		auto event = static_cast<MCK::ButtonEvents>(gamepadState.buttons[MCK::GamepadButton::CROSS]);

		// The button is pressed and was not held before
		bool crossPressed = (event == MCK::ButtonEvents::Pressed) && !jumpHeld;

		// The button is held or pressed
		bool crossHeld = (event == MCK::ButtonEvents::Held) || (event == MCK::ButtonEvents::Pressed);

		// Update jumpPressed and jumpHeld only if the state has changed
		if (crossPressed != jumpPressed || crossHeld != jumpHeld)
		{
			jumpPressed = crossPressed;
			jumpHeld = crossHeld;
		}
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void InputComponent::OnDestroy() 
	{
		MCK::Input::Unsubscribe(&receipt);
	}

	bool InputComponent::Deserialise(json data)
	{
		// Get Entity's Input Component Data.
		data = data["data"];

		// Get Input Component's Direction as a Vector2f
		Direction().x() = data["directionX"];
		Direction().y() = data["directionY"];

		// Get Input Component's jump pressed variable
		JumpPressed() = data["jumpPressed"];

		// Get Input Component's jump held variable
		JumpHeld() = data["jumpHeld"];

		return true;
	}
}