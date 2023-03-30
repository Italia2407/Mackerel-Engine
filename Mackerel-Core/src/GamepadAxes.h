#pragma once

#include <cstdint>

namespace MCK
{
	/**
	 * This enum class contains an enum for detectible gamepad axes (joysticks and triggers).
	 * It is based on the GLFW gamepad axes constants.
	 */
	namespace GamepadAxes
	{
		const int32_t LEFT_X = 0;
		const int32_t LEFT_Y = 1;
		const int32_t RIGHT_X = 2;
		const int32_t RIGHT_Y = 3;
		const int32_t LEFT_TRIGGER = 4;
		const int32_t RIGHT_TRIGGER = 5;
	};
}
