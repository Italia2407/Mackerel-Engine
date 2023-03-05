#include "Window.h"

#include "LoggingSystem.h"

#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include <Eigen/Dense.h>
#include <vector>
#include <list>
#include "Entity.h"

void SayHello()
{
    MCK::EntitySystem::Entity entity;
    entity.Deserialise(entity.TestEntityJson());

	// Initialise GLFW
    if (!glfwInit())
    {
        return;
    }

    // intialize logging system
    MCK::Logger::initialize();

    //Initial system start message
    MCK::Logger::log("System Starting", MCK::Logger::LogLevel::Basic, std::source_location::current());

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);

    // Make Window Current & Load GLAD
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    // Setup Dear ImGui style
    ImGui::StyleColorsClassic();
    ImGui::GetStyle().WindowRounding = 8.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::ShowDemoWindow();
        ImGui::Begin("Test");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    Eigen::Vector3<float> vecA(12.0f, 13.0f, 4.0f);
    Eigen::Vector3<float> vecB(2.0f, 42.0f, 10.0f);

    std::stringstream ss;
    ss << "Hello World! " << "Dot Result = " << vecA.dot(vecB);
    MCK::Logger::log(ss.str(), MCK::Logger::LogLevel::Basic, std::source_location::current());
}