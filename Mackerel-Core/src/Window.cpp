#include "Window.h"

#include <iostream>
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

void SayHello()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	GLFWwindow* window = glfwCreateWindow(480, 480, "Test", nullptr, nullptr);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

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

	std::cout << "Hello World!\n";
}