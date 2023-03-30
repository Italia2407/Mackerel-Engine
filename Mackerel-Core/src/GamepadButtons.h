#pragma once

#include <cstdint>

#include "InputSetRanges.h"

namespace MCK
{
	/**
	 * This enum class contains an enum for detectible gamepad buttons.
	 * It is based on the GLFW gamepad button constants.
	 */
	namespace GamepadButton
	{
		const int32_t CROSS = MCK_GP_OFFSET + 0;
		const int32_t CIRCLE = MCK_GP_OFFSET + 1;
		const int32_t SQUARE = MCK_GP_OFFSET + 2;
		const int32_t TRIANGLE = MCK_GP_OFFSET + 3;
		const int32_t LEFT_BUMPER = MCK_GP_OFFSET + 4;
		const int32_t RIGHT_BUMPER = MCK_GP_OFFSET + 5;
		const int32_t BACK = MCK_GP_OFFSET + 6;
		const int32_t START = MCK_GP_OFFSET + 7;
		const int32_t GUIDE = MCK_GP_OFFSET + 8;
		const int32_t LEFT_THUMB = MCK_GP_OFFSET + 9;
		const int32_t RIGHT_THUMB = MCK_GP_OFFSET + 10;
		const int32_t DPAD_UP = MCK_GP_OFFSET + 11;
		const int32_t DPAD_RIGHT = MCK_GP_OFFSET + 12;
		const int32_t DPAD_DOWN = MCK_GP_OFFSET + 13;
		const int32_t DPAD_LEFT = MCK_GP_OFFSET + 14;
	};
}
