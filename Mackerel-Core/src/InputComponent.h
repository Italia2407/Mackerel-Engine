#pragma once

#include "Eigen/Eigen.h"
#include "Component.h"
#include "Input.h"

namespace MCK::EntitySystem
{
	class InputComponent : public Component<InputComponent>
	{
	private:
		Eigen::Vector2f keyboardDirection;
		bool keyboardUp;
		bool keyboardDown;
		bool keyboardLeft;
		bool keyboardRight;
		bool keyboardJumpPressed;
		bool keyboardJumpHeld;

		Eigen::Vector2f gamepadDirection;
		bool gamepadJumpPressed;
		bool gamepadJumpHeld;
		double stickDriftThreshold;

		Eigen::Vector2f direction;
		bool jumpPressed;
		bool jumpHeld;

		MCK::InputCallback buttonInputCallback;   
		MCK::Input::InputSubReceipt receipt; 

		void MovingPlayerCallback(int32_t key, MCK::ButtonEvents ButtonEvents);

	public:
		inline Eigen::Vector2f& Direction() { return direction; }
		inline bool& JumpPressed() { return jumpPressed; }
		inline bool& JumpHeld() { return jumpHeld; }

	public:
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		bool Deserialise(json data) override;
	};
}