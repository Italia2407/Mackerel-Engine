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
			GLFWwindow* operator*() const;
			/**
			 * Returns the width of the window associated with this context.
			 * 
			 * \return The window width
			 */
			int Width();
			/**
			 * Returns the height of the window associated with this context.
			 *
			 * \return The window height
			 */
			int Height();

		private:
			GLFWwindow* _window = nullptr;
			int _width = 0, _height = 0;

			/* glfw callbacks */
			static void resizeWindow(GLFWwindow* window, int screenWidth, int screenHeight);
	};
}
