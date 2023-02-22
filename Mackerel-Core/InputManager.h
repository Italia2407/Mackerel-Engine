#pragma once

#include <GLFW/glfw3.h>
#include <map>

namespace MCK::Input
{
	class InputManager
	{
		private:
			InputManager();
			~InputManager();

			// we can have one input manager, as a treat
			InputManager(const InputManager&) = delete;
			InputManager& operator=(const InputManager&) = delete;

			// singleton bookkeeping
			static InputManager* instance;
			static InputManager* Instance()
			{
				if (!instance)
					instance = new InputManager;
				return instance;
			}

		public:
			
	};
}
