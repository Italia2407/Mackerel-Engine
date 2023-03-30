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
#include "Scene.h"
#include "Shader.h"

#include "Input.h"
#include "Renderer.h"
#include "Mesh.h"
#include "TimeManager.h"

void InputCallbackTest(int32_t key, MCK::ButtonEvents ButtonEvents)
{
    std::string message = "Key [" + std::to_string(static_cast<int>(key)) + "] did action [" + std::to_string(static_cast<int>(ButtonEvents)) + "].";
    MCK::Logger::log(message, MCK::Logger::LogLevel::Debug, std::source_location::current());
}

void MouseCallbacktest(int32_t key, MCK::ButtonEvents ButtonEvents)
{
    printf("Position: [%.2f, %.2f]\n"
        "Delta: [%.2f, %.2f]\n",
        MCK::Input::MousePosition().posX,
        MCK::Input::MousePosition().posY,
        MCK::Input::MousePosition().deltaX,
        MCK::Input::MousePosition().deltaY);
}

void ControllerCallbacktest(int32_t key, MCK::ButtonEvents ButtonEvents)
{
    printf("Left: [%.2f, %.2f]\n"
        "Right: [%.2f, %.2f]\n"
        "LeftShoulder: [%.2f]\n"
        "RightShoulder: [%.2f]\n",
        MCK::Input::GamepadState().axes[MCK::GamepadAxes::LEFT_X],
        MCK::Input::GamepadState().axes[MCK::GamepadAxes::LEFT_Y],
        MCK::Input::GamepadState().axes[MCK::GamepadAxes::RIGHT_X],
        MCK::Input::GamepadState().axes[MCK::GamepadAxes::RIGHT_Y],
        MCK::Input::GamepadState().axes[MCK::GamepadAxes::LEFT_TRIGGER],
        MCK::Input::GamepadState().axes[MCK::GamepadAxes::RIGHT_TRIGGER]);
}

void TimerWentOff()
{
    double time = MCK::TimeManager::GetUpTime();
    std::string message = "Timer went off at: " + std::to_string(static_cast<double>(time));
    MCK::Logger::log(message, MCK::Logger::LogLevel::Debug, std::source_location::current());
}

void ScaledTimerWentOff()
{
    double time = MCK::TimeManager::GetScaledUpTime();
    std::string message = "Scaled Timer went off at: " + std::to_string(static_cast<double>(time));
    MCK::Logger::log(message, MCK::Logger::LogLevel::Debug, std::source_location::current());
}

#include <windows.h>
#include <string>
#include <iostream>

void SayHello()
{
    MCK::EntitySystem::Scene scene;
    scene.Deserialise(scene.TestSceneJson());

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

    // Begin input demo

        // Setup a key callback
    MCK::InputCallback exampleCallback = InputCallbackTest;
    MCK::InputCallback printMouseInfo = MouseCallbacktest;
    MCK::InputCallback printGamepadInfo = ControllerCallbacktest;

        // Create an input subscription receipt ptr (alternatively a receipt can
        //     generated by a subscribe function where the 'receipt' argument is
        //     excluded. Note it is the user's responsibility delete those too!)
    MCK::Input::InputSubReceipt* receipt = new MCK::Input::InputSubReceipt();

        // These keys will print a message when pressed, released, or held
    MCK::Input::Subscribe(MCK::Key::W, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::A, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::S, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::D, MCK::ButtonEvents::All, exampleCallback, receipt);

        // This key will print a message only when pressed
    MCK::Input::Subscribe(MCK::Key::E, MCK::ButtonEvents::Pressed, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::GamepadButton::CROSS, MCK::ButtonEvents::Pressed, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::MouseButton::MOUSE_LEFT, MCK::ButtonEvents::Pressed, exampleCallback, receipt);

        // This key will print a message only when released
    MCK::Input::Subscribe(MCK::Key::Q, MCK::ButtonEvents::Released, exampleCallback, receipt);

        // This key will print a message only when held
    MCK::Input::Subscribe(MCK::Key::R, MCK::ButtonEvents::Held, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::MouseButton::MOUSE_RIGHT, MCK::ButtonEvents::Held, printMouseInfo, receipt);
    MCK::Input::Subscribe(MCK::GamepadButton::CIRCLE, MCK::ButtonEvents::Held, printGamepadInfo, receipt);

    // End input demo

    // Start timer demo

    // set callback functions
    std::function<void()> timerCallback = TimerWentOff;
    std::function<void()> scaledTimerCallback = ScaledTimerWentOff;

    // set a timer
    double time = 0.1;
    std::pair<double, std::function<void()>> timer = { time, timerCallback };

    MCK::TimeManager::setTimer(timer);
    // set a second timer to check that adding and popping works properly
    MCK::TimeManager::setTimer(timer);

    // set time scale
    MCK::TimeManager::setTimescale(2.0);

    // set a scaled timer
    double time2 = 0.2;
    std::pair<double, std::function<void()>> timer2 = { time2, scaledTimerCallback };

    MCK::TimeManager::setScaledTimer(timer2);


    // wait for timer to finish, then call an update to see if the timer has triggered the callback
    MCK::TimeManager::Update();
    Sleep(100);
    MCK::TimeManager::Update();

    // check that frame times work
    double frameTime = MCK::TimeManager::getFrameTime();
    std::string message0 = "Frame time: " + std::to_string(static_cast<double>(frameTime));
    MCK::Logger::log(message0, MCK::Logger::LogLevel::Debug, std::source_location::current());

    double scaledFrameTime = MCK::TimeManager::getScaledFrameTime();
    std::string message1 = "Scaled Frame time: " + std::to_string(static_cast<double>(scaledFrameTime));
    MCK::Logger::log(message1, MCK::Logger::LogLevel::Debug, std::source_location::current());

    // End timer demo

    // Make Window Current & Load GLAD
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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

    MCK::Rendering::Renderer::InitialiseRenderer(1280, 720);

    MCK::AssetType::Shader* testVertShader = new MCK::AssetType::Shader(); {
        testVertShader->LoadShaderFromSource("../Mackerel-Core/res/Shaders/vert/passthrough.vert", GL_VERTEX_SHADER);
    }
    MCK::AssetType::Shader* testFragShader = new MCK::AssetType::Shader(); {
        testFragShader->LoadShaderFromSource("../Mackerel-Core/res/Shaders/frag/monocolour.glsl", GL_FRAGMENT_SHADER);
    }
    MCK::AssetType::Mesh* testMesh = new MCK::AssetType::Mesh(); {
        testMesh->LoadSmallDisplayMesh();
    }
    MCK::AssetType::Shader* testFragDefShader = new MCK::AssetType::Shader(); {
        testFragDefShader->LoadShaderFromSource("../Mackerel-Core/res/Shaders/light/unlit.glsl", GL_FRAGMENT_SHADER);
    }
    MCK::Rendering::Renderer::AddUnlitShader(testFragDefShader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Input System
        MCK::Input::Update(window);

        //ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Set Renderer Data
        MCK::Rendering::Renderer::QueueMeshInstance(Eigen::Matrix4f::Identity(), testMesh, testFragShader, nullptr, false);

        /* Render here */
        MCK::Rendering::Renderer::RenderFrame(testVertShader);

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

    // All input subscriptions need a matching unsubscribe call
    MCK::Input::Unsubscribe(receipt);

    Eigen::Vector3<float> vecA(12.0f, 13.0f, 4.0f);
    Eigen::Vector3<float> vecB(2.0f, 42.0f, 10.0f);

    std::stringstream ss;
    ss << "Hello World! " << "Dot Result = " << vecA.dot(vecB);
    MCK::Logger::log(ss.str(), MCK::Logger::LogLevel::Basic, std::source_location::current());
}