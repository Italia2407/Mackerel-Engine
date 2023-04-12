#pragma once

struct GLFWmonitor;
struct GLFWwindow;

namespace MCK
{
	class Context
	{
		public:
			Context(int width = 1280, int height = 720, const char* title = "Test", GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
			~Context();

			Context(const Context&) = delete;
			Context& operator=(const Context&) = delete;

			/**
			 * Returns the GLFWwindow pointer associated with this context.
			 * 
			 * \return Associated GLFWwindow pointer
			 */
			inline GLFWwindow* operator*() const
			{
				return _window;
			}

			inline int Width()
			{
				return _width;
			}

			inline int Height()
			{
				return _height;
			}

		private:
			GLFWwindow* _window = nullptr;
			int _width = 0, _height = 0;

			/* glfw callbacks */
			static void resizeWindow(GLFWwindow* window, int screenWidth, int screenHeight);
	};
}
