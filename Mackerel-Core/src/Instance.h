#pragma once

#include <cstdio>
#include <exception>

/* The order of these includes is a bit finicky, change at your own risk */
#include "LoggingSystem.h"
#include "GLinclude.h"
#include "AppAttorney.h"
#include "Context.h"
#include "Input.h"
#include "Renderer.h"
#include "TimeManager.h"

class App;

namespace MCK
{
	class Instance
	{
		public:
			static inline bool Start(App* application) try
			{
				/* Initilise logger */
				MCK::Logger::initialize();

				/* Initialise glfw/glad and create context */
				if (glfwInit() == false)
				{
					return false;
				}

				Context* context = new Context();
				glfwMakeContextCurrent(**context);
				AppAttorney::App_Instance::SetContext(application, context);
				gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

				/* Initialise other systems */
				if (MCK::Rendering::Renderer::InitialiseRenderer(context->Width(), context->Height()) == false)
				{
					return false;
				}

				/* Start the app */
				AppAttorney::App_Instance::BeforeLoop(application);
				while (glfwWindowShouldClose(**context) == false)
				{
					/* Poll for events */
					glfwPollEvents();

					/* Update systems */
					MCK::Input::Update(**context);
					MCK::TimeManager::Update();

					/* Update/Draw the app */
					AppAttorney::App_Instance::DuringLoop(application);

					/* Render the frame */
					MCK::Rendering::Renderer::RenderFrame();

					/* Swap front and back buffers */
					glfwSwapBuffers(**context);
				}
				/* Shutdown the app */
				AppAttorney::App_Instance::AfterLoop(application);
			
				/* clean up glfw instance and context */
				delete context;
				glfwTerminate();

				return true;
			}
			catch (std::exception const& exception)
			{
				/* Error... */
				MCK::Logger::log("\nError: " + std::string(exception.what()) + "\n",
					MCK::Logger::LogLevel::Fatal,
					std::source_location::current(),
					"MCK");

				(void*)&exception;

				return false;
			}
	};
}
