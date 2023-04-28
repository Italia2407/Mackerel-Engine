#include "Context.h"

#include "GLinclude.h" /* class GLFWwindow, class GLFWmonitor */

#include "Renderer.h"

MCK::Context::Context(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	: _window(glfwCreateWindow(width, height, title, monitor, share)), _width(width), _height(height)
{
	glfwMakeContextCurrent(_window);

	glfwSetFramebufferSizeCallback(_window, MCK::Context::resizeWindow);
}

MCK::Context::~Context()
{
	glfwDestroyWindow(_window);
}

GLFWwindow* MCK::Context::operator*() const
{
	return _window;
}

int MCK::Context::Width()
{
	return _width;
}

int MCK::Context::Height()
{
	return _height;
}

void MCK::Context::UpdateDimensions()
{
	glfwGetWindowSize(_window, &_width, &_height);
}

void MCK::Context::resizeWindow(GLFWwindow* window, int screenWidth, int screenHeight)
{
	MCK::Rendering::Renderer::ResizeRenderer((GLuint)screenWidth, (GLuint)screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
}
