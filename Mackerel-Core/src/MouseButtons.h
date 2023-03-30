#pragma once

#include <cstdint>

#include "InputSetRanges.h"

namespace MCK
{
	/**
	 * This enum class contains an enum for detectible gamepad buttons; it is
	 *     based on the GLFW gamepad button constants.
	 */
	namespace MouseButton
	{
		const int32_t MOUSE_BUTTON_1 = MCK_MOUSE_OFFSET + 0;
		const int32_t MOUSE_BUTTON_2 = MCK_MOUSE_OFFSET + 1;
		const int32_t MOUSE_BUTTON_3 = MCK_MOUSE_OFFSET + 2;
		const int32_t MOUSE_BUTTON_4 = MCK_MOUSE_OFFSET + 3;
		const int32_t MOUSE_BUTTON_5 = MCK_MOUSE_OFFSET + 4;
		const int32_t MOUSE_BUTTON_6 = MCK_MOUSE_OFFSET + 5;
		const int32_t MOUSE_BUTTON_7 = MCK_MOUSE_OFFSET + 6;
		const int32_t MOUSE_BUTTON_8 = MCK_MOUSE_OFFSET + 7;
		const int32_t MOUSE_LEFT = MOUSE_BUTTON_1;
		const int32_t MOUSE_RIGHT = MOUSE_BUTTON_2;
		const int32_t MOUSE_MIDDLE = MOUSE_BUTTON_3;
	};
}
