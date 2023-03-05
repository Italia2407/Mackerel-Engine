#pragma once

namespace MCK
{
	/**
	 * This enum class contains an enum for each key event (Pressed, Released, Held), it is
	 *     based on the GLFW key constants. Serves a dual purpose as a key state and
	 *     key event enum.
	 */
	enum class KeyEvent
	{
		Released = 0,
		Pressed = 1,
		Held = 2, // Take note that this used differently than glfw uses 'Repeated' (See KeyHandler class and glfw docs)
		Null = 3 // Not used by glfw, but used for default key state in Key Handlers
	};
}
