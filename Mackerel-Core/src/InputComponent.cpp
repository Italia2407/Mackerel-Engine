#include "LoggingSystem.h"

#include "InputComponent.h"

#include "Entity.h"

#include "Input.h"

namespace MCK::EntitySystem
{
	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void InputComponent::OnCreate()
	{
		stickDriftThreshold = 0.1;

		keyboardUp = 0;
		keyboardDown = 0;
		keyboardLeft = 0;
		keyboardRight = 0;

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

		MCK::Input::Subscribe(MCK::GamepadButton::CROSS, MCK::ButtonEvents::Pressed, buttonInputCallback, &receipt); 
		MCK::Input::Subscribe(MCK::GamepadButton::CROSS, MCK::ButtonEvents::Held, buttonInputCallback, &receipt); 
		MCK::Input::Subscribe(MCK::GamepadButton::CROSS, MCK::ButtonEvents::Released, buttonInputCallback, &receipt); 
	}

	/**
	 * Callback to update variables
	 *
	 */
	void InputComponent::MovingPlayerCallback(int32_t key, MCK::ButtonEvents ButtonEvents)
	{
		if (ButtonEvents == MCK::ButtonEvents::Held)
		{
			if (key == MCK::Key::W)
			{
				keyboardUp = true;
			}
			else if (key == MCK::Key::S)
			{
				keyboardDown = true;
			}
			else if (key == MCK::Key::A)
			{
				keyboardLeft = true;
			}
			else if (key == MCK::Key::D)
			{
				keyboardRight = true;
			}
			else if (key == MCK::Key::SPACE)
			{
				keyboardJumpHeld = true;
				keyboardJumpPressed = false;
			}
			else if (key == MCK::GamepadButton::CROSS)
			{
				gamepadJumpHeld = true;
				gamepadJumpPressed = false;
			}
		}
		else if (ButtonEvents == MCK::ButtonEvents::Released)
		{
			if (key == MCK::Key::W)
			{
				keyboardUp = false;
			}
			else if (key == MCK::Key::S)
			{
				keyboardDown = false;
			}
			else if (key == MCK::Key::A)
			{
				keyboardLeft = false;
			}
			else if (key == MCK::Key::D)
			{
				keyboardRight = false;
			}
			else if (key == MCK::Key::SPACE)
			{
				keyboardJumpHeld = false;
				keyboardJumpPressed = false;
			}
			else if (key == MCK::GamepadButton::CROSS)
			{
				gamepadJumpHeld = false;
				gamepadJumpPressed = false;
			}
		}
		else if (ButtonEvents == MCK::ButtonEvents::Pressed)
		{
			if (key == MCK::Key::SPACE)
			{
				keyboardJumpPressed = true;
			}
			else if (key == MCK::GamepadButton::CROSS)
			{
				gamepadJumpPressed = true;
			}
		}

		//std::ostringstream output;
		//output << "Direction: (" << direction.x() << ", " << direction.y() << ") " << " JumpPressed: " << jumpPressed << " JumpHeld: " << jumpHeld;
		//std::string outputStr = output.str();

		//MCK::Logger::log(outputStr, MCK::Logger::LogLevel::Debug, std::source_location::current());
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void InputComponent::OnUpdate()
	{
		// Query gamepad state
		auto gamepadState = Input::GamepadState();

		// Update direction based on gamepad axes, checking for unintended stick drift
		if (abs(gamepadState.axes[MCK::GamepadAxes::LEFT_X]) > stickDriftThreshold)
		{
			gamepadDirection.x() = gamepadState.axes[MCK::GamepadAxes::LEFT_X];
		}
		else
		{
			gamepadDirection.x() = 0;
		}
		if (abs(gamepadState.axes[MCK::GamepadAxes::LEFT_Y]) > stickDriftThreshold)
		{
			gamepadDirection.y() = -gamepadState.axes[MCK::GamepadAxes::LEFT_Y];
		}
		else
		{
			gamepadDirection.y() = 0;
		}

		// Update direction based on keyboard
		keyboardDirection.x() = static_cast<float>(keyboardRight - keyboardLeft);
		keyboardDirection.y() = static_cast<float>(keyboardDown - keyboardUp);

		jumpPressed = keyboardJumpPressed || gamepadJumpPressed;
		jumpHeld = keyboardJumpHeld || gamepadJumpHeld;
		direction = keyboardDirection + gamepadDirection;

		// Normalize the direction vector
		if (direction.norm() > 0)
		{
			direction.normalize();
		}

		//std::ostringstream output;
		//output << "Direction: (" << direction.x() << ", " << direction.y() << ") " << " JumpPressed: " << jumpPressed << " JumpHeld: " << jumpHeld;
		//std::string outputStr = output.str();
		
		//MCK::Logger::log(outputStr, MCK::Logger::LogLevel::Debug, std::source_location::current());
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