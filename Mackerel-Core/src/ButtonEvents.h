#pragma once

namespace MCK
{
	/**
	 * This enum class contains an enum for each button event (Pressed, Released, Held), it is
	 *     based on the GLFW key constants. Serves a dual purpose as a key state and
	 *     key event enum.
	 */
	enum class ButtonEvents
	{
		Released = 0,
		Pressed = 1,
		Held = 2, // Take note that this used differently than glfw uses 'Repeated' (See KeyHandler class and glfw docs)
		Null = 3, // Not used by glfw, but used for default key state in Key Handlers
		All = 4 // Used to subscribe/unsubscribe a callback function to all events for a given key
	};
}
