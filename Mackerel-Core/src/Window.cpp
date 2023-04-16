#include "_Window.h"

// Asset Headers
#include "MeshLibrary.h"
#include "ShaderLibrary.h"

// Logging Headers
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
#include "JsonHelpers.h"
#include "Material.h"

#include "TestComponent.h"
#include "TransformComponent.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

MCK::EntitySystem::TransformComponent testTransform;
MCK::EntitySystem::TransformComponent par;

MCK::EntitySystem::CameraComponent* camera;

void FramebufferResizeCallback(GLFWwindow* window, int screenWidth, int screenHeight)
{
    MCK::Rendering::Renderer::ResizeRenderer((GLuint)screenWidth, (GLuint)screenHeight);
    camera->AspectRatio() = (float)screenWidth / (float)screenHeight;

    glViewport(0, 0, screenWidth, screenHeight);
}

bool moveCamUp = false;
bool moveCamDown = false;
bool moveCamIn = false;
bool moveCamOut = false;
bool moveCamLeft = false;
bool moveCamRight = false;

bool moveModUp = false;
bool moveModDown = false;
bool moveModIn = false;
bool moveModOut = false;
bool moveModLeft = false;
bool moveModRight = false;

void InputCallbackTest(int32_t key, MCK::ButtonEvents ButtonEvents)
{
    std::string message = "Key [" + std::to_string(static_cast<int>(key)) + "] did action [" + std::to_string(static_cast<int>(ButtonEvents)) + "].";
    MCK::Logger::log(message, MCK::Logger::LogLevel::Debug, std::source_location::current());

    if (key == MCK::Key::E && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveCamUp = true;
    else if (key == MCK::Key::E && ButtonEvents == MCK::ButtonEvents::Released)
        moveCamUp = false;

    if (key == MCK::Key::Q && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveCamDown = true;
    else if (key == MCK::Key::Q && ButtonEvents == MCK::ButtonEvents::Released)
        moveCamDown = false;

    if (key == MCK::Key::W && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveCamIn = true;
    else if (key == MCK::Key::W && ButtonEvents == MCK::ButtonEvents::Released)
        moveCamIn = false;

    if (key == MCK::Key::S && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveCamOut = true;
    else if (key == MCK::Key::S && ButtonEvents == MCK::ButtonEvents::Released)
        moveCamOut = false;

    if (key == MCK::Key::A && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveCamLeft = true;
    else if (key == MCK::Key::A && ButtonEvents == MCK::ButtonEvents::Released)
        moveCamLeft = false;

    if (key == MCK::Key::D && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveCamRight = true;
    else if (key == MCK::Key::D && ButtonEvents == MCK::ButtonEvents::Released)
        moveCamRight = false;

    if (key == MCK::Key::LEFT_SHIFT && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveModUp = true;
    else if (key == MCK::Key::LEFT_SHIFT && ButtonEvents == MCK::ButtonEvents::Released)
        moveModUp = false;

    if (key == MCK::Key::LEFT_CONTROL && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveModDown = true;
    else if (key == MCK::Key::LEFT_CONTROL && ButtonEvents == MCK::ButtonEvents::Released)
        moveModDown = false;

    if (key == MCK::Key::UP && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveModIn = true;
    else if (key == MCK::Key::UP && ButtonEvents == MCK::ButtonEvents::Released)
        moveModIn = false;

    if (key == MCK::Key::DOWN && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveModOut = true;
    else if (key == MCK::Key::DOWN && ButtonEvents == MCK::ButtonEvents::Released)
        moveModOut = false;

    if (key == MCK::Key::LEFT && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveModLeft = true;
    else if (key == MCK::Key::LEFT && ButtonEvents == MCK::ButtonEvents::Released)
        moveModLeft = false;

    if (key == MCK::Key::RIGHT && ButtonEvents == MCK::ButtonEvents::Pressed)
        moveModRight = true;
    else if (key == MCK::Key::RIGHT && ButtonEvents == MCK::ButtonEvents::Released)
        moveModRight = false;
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

    MCK::EntitySystem::Entity* e1 = new MCK::EntitySystem::Entity();
    MCK::EntitySystem::Entity* e2 = new MCK::EntitySystem::Entity();

    e1->AddComponent(&testTransform);
    e2->AddComponent(&par);

    e1->parent = e2;

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
    MCK::Input::Subscribe(MCK::Key::Q, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::E, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::W, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::A, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::S, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::D, MCK::ButtonEvents::All, exampleCallback, receipt);

    MCK::Input::Subscribe(MCK::Key::LEFT_SHIFT, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::LEFT_CONTROL, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::LEFT, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::RIGHT, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::UP, MCK::ButtonEvents::All, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::Key::DOWN, MCK::ButtonEvents::All, exampleCallback, receipt);

        // This key will print a message only when pressed
    //MCK::Input::Subscribe(MCK::Key::E, MCK::ButtonEvents::Pressed, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::GamepadButton::CROSS, MCK::ButtonEvents::Pressed, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::MouseButton::MOUSE_LEFT, MCK::ButtonEvents::Pressed, exampleCallback, receipt);

        // This key will print a message only when released
    //MCK::Input::Subscribe(MCK::Key::Q, MCK::ButtonEvents::Released, exampleCallback, receipt);

        // This key will print a message only when held
    MCK::Input::Subscribe(MCK::Key::R, MCK::ButtonEvents::Held, exampleCallback, receipt);
    MCK::Input::Subscribe(MCK::MouseButton::MOUSE_RIGHT, MCK::ButtonEvents::Held, printMouseInfo, receipt);
    MCK::Input::Subscribe(MCK::GamepadButton::CIRCLE, MCK::ButtonEvents::Held, printGamepadInfo, receipt);

    // End input demo

    // Start timer demo

    // set callback functions
    std::function<void()> timerCallback = TimerWentOff;
    std::function<void()> scaledTimerCallback = ScaledTimerWentOff;
    std::function<void(GLFWwindow*, int, int)> framebufferResizeCallback = FramebufferResizeCallback;

    glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

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

    // Start json helper demo
    //json test = MCK::Helpers::getJsonObject("../../test.txt");
    //json sample = MCK::Helpers::getJsonObject("../../sample1.json");
    //std::string messageJson = "Json output: " + sample.dump();
    //MCK::Logger::log(messageJson, MCK::Logger::LogLevel::Debug, std::source_location::current());
    //std::string messageJson1 = "Json output: " + test.dump();
    //MCK::Logger::log(messageJson1, MCK::Logger::LogLevel::Debug, std::source_location::current());
    // End json helper demo

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

    MCK::AssetType::Mesh* testMesh = new MCK::AssetType::Mesh("Test Mesh");
    testMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Suzanne.obj");
    MCK::AssetType::Material* testMaterial = new MCK::AssetType::Material();
    testMaterial->addUInt16Uniform("lightShaderID", 0);
    testMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    MCK::AssetType::Shader* unlitShader;
    MCK::AssetType::Shader* monocolourShader;

    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, unlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, monocolourShader);

    MCK::Rendering::Renderer::AddUnlitShader(unlitShader);

    camera = new MCK::EntitySystem::PerspectiveCamera(1280.0f / 720.0f);
    //camera = new MCK::EntitySystem::OrthographicCamera(1280.0f / 720.0f);
    camera->Position() = Eigen::Vector3f(0.0f, 0.0f, -3.0f);
    //camera->FrontDirection() = Eigen::Vector3f(0.0f, -1.0f, 0.0f);
    //camera->UpDirection() = Eigen::Vector3f(0.0f, 0.0f, 1.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Input System
        MCK::Input::Update(window);

        // Time Manager
        MCK::TimeManager::Update();

        if (moveCamIn)
            camera->Position().z() += 0.001f;
        if (moveCamOut)
            camera->Position().z() -= 0.001f;
        if (moveCamUp)
            camera->Position().y() += 0.001f;
        if (moveCamDown)
            camera->Position().y() -= 0.001f;
        if (moveCamRight)
            camera->Position().x() += 0.001f;
        if (moveCamLeft)
            camera->Position().x() -= 0.001f;

        if (moveModIn)
            camera->FrontDirection().z() += 0.001f;
        if (moveModOut)
            camera->FrontDirection().z() -= 0.001f;
        if (moveModUp)
            camera->FrontDirection().y() += 0.001f;
        if (moveModDown)
            camera->FrontDirection().y() -= 0.001f;
        if (moveModRight)
            camera->FrontDirection().x() += 0.001f;
        if (moveModLeft)
            camera->FrontDirection().x() -= 0.001f;

        //testTransform.Rotation() = Eigen::Quaternionf(Eigen::AngleAxisf((float)MCK::TimeManager::GetUpTime(), Eigen::Vector3f(0.0f, 1.0f, 1.0f).normalized()));

        //ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Set Renderer Data
        MCK::Rendering::Renderer::QueueMeshInstance(testTransform, testMesh, monocolourShader, testMaterial, false);
        MCK::Rendering::Renderer::UseCamera(*camera);

        /* Render here */
        MCK::Rendering::Renderer::RenderFrame();

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

void SayHello2()
{
    MCK::EntitySystem::Scene scene;

    MCK::EntitySystem::Entity* entity = scene.CreateEntity();

    MCK::EntitySystem::TransformComponent transformComp;
    MCK::EntitySystem::TestComponent testComp;

    entity->AddComponent(&transformComp);
    entity->AddComponent(&testComp);

    // Initialise GLFW
    if (!glfwInit())
    {
        return;
    }

    MCK::Logger::initialize();

    // Make Window Current & Load GLAD
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    MCK::Rendering::Renderer::InitialiseRenderer(1280, 720);

    MCK::AssetType::Mesh* testMesh = new MCK::AssetType::Mesh("Test Mesh");
    testMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Suzanne.obj");
    MCK::AssetType::Material* testMaterial = new MCK::AssetType::Material();
    testMaterial->addUInt16Uniform("lightShaderID", 0);
    testMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    MCK::AssetType::Shader* unlitShader;
    MCK::AssetType::Shader* monocolourShader;

    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, unlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, monocolourShader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        scene.UpdateScene();

        // Set Renderer Data
        MCK::Rendering::Renderer::QueueMeshInstance(transformComp, testMesh, monocolourShader, testMaterial, false);

        /* Render here */
        MCK::Rendering::Renderer::RenderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}

Eigen::Vector2f cubeInput;
void MovingCubeCallback(int32_t key, MCK::ButtonEvents ButtonEvents)
{
    Eigen::Vector2f inp{};
    if (key == MCK::Key::W)
    {
        inp.x() = 0.f;
        inp.y() = 1.f;
    }
    else if (key == MCK::Key::S)
    {
        inp.x() = 0.f;
        inp.y() = -1.f;
    }
    else if (key == MCK::Key::A)
    {
        inp.x() = -1.f;
        inp.y() = 0.f;
    }
    else if (key == MCK::Key::D)
    {
        inp.x() = 1.f;
        inp.y() = 0.f;
    }
    cubeInput = inp;
    std::cout << "k" << key << " b" << (int)ButtonEvents << std::endl;
}

void SayHello3()
{

#pragma region Entity Creation

    MCK::EntitySystem::Scene scene;

    MCK::EntitySystem::Entity* entity = scene.CreateEntity();
    MCK::EntitySystem::Entity* cameraEntity = scene.CreateEntity();
    MCK::EntitySystem::Entity* floorEntity = scene.CreateEntity();

    MCK::EntitySystem::TransformComponent transformComp;
    MCK::EntitySystem::TestComponent testComp;
    MCK::Physics::RigidbodyComponent rigidComp;

    MCK::EntitySystem::TransformComponent floorTransform;
    MCK::Physics::CollisionComponent collisionComp;

    transformComp.Position() = Eigen::Vector3f({ 0,15, 20 });
    floorTransform.Position() = Eigen::Vector3f({ 0,-3, 20 });
    floorTransform.Scale() = Eigen::Vector3f({ 10,1, 10 });

#pragma endregion

#pragma region Input Init
    MCK::InputCallback cubeInputCallback = MovingCubeCallback;
    MCK::Input::InputSubReceipt receipt;

    MCK::Input::Subscribe(MCK::Key::W, MCK::ButtonEvents::Held, cubeInputCallback, &receipt);
    MCK::Input::Subscribe(MCK::Key::A, MCK::ButtonEvents::Held, cubeInputCallback, &receipt);
    MCK::Input::Subscribe(MCK::Key::D, MCK::ButtonEvents::Held, cubeInputCallback, &receipt);
    MCK::Input::Subscribe(MCK::Key::S, MCK::ButtonEvents::Held, cubeInputCallback, &receipt);
#pragma endregion


#pragma region Render Init

    // Initialise GLFW
    if (!glfwInit())
    {
        return;
    }

    MCK::Logger::initialize();

    // Make Window Current & Load GLAD
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Test", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    MCK::Rendering::Renderer::InitialiseRenderer(1280, 720);

    MCK::AssetType::Mesh* testMesh = new MCK::AssetType::Mesh("Test Mesh");
    testMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");
    MCK::AssetType::Material* testMaterial = new MCK::AssetType::Material();
    testMaterial->addUInt16Uniform("lightShaderID", 0);
    testMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(1.0f, 1.0f, 1.0f));

    MCK::AssetType::Shader* testShader = nullptr;
    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, testShader);

    MCK::AssetType::Shader* lightShader = nullptr;
    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, lightShader);

    MCK::EntitySystem::MeshRendererComponent meshRenderer(testMesh, testShader, testMaterial);
    MCK::EntitySystem::MeshRendererComponent floorRenderer(testMesh, testShader, testMaterial);

    MCK::Rendering::Renderer::AddUnlitShader(lightShader);

    MCK::EntitySystem::ProjectionCamera cameraComponent(1280.f / 720.f);

#pragma endregion

#pragma region Add Components

    entity->AddComponent(&transformComp);
    //entity->AddComponent(&testComp);
    entity->AddComponent(&meshRenderer);
    entity->AddComponent(&rigidComp);

    floorEntity->AddComponent(&floorTransform);
    floorEntity->AddComponent(&collisionComp);
    floorEntity->AddComponent(&floorRenderer);

    cameraEntity->AddComponent(&cameraComponent);

#pragma endregion

    scene.InitialiseScene();

    while (!glfwWindowShouldClose(window))
    {
        cubeInput = { 0,0 };

        MCK::Input::InputManager::Update(window);
        MCK::TimeManager::Update();

        auto& joyIn = MCK::Input::GamepadState();
        
        //testComp.input.x() = joyIn.axes[MCK::GamepadAxes::LEFT_X];
        //testComp.input.y() = -joyIn.axes[MCK::GamepadAxes::LEFT_Y];

        scene.UpdateScene();

        btVector3 rayStart(0, -3, 0);
        btVector3 rayEnd(0, -3, 30);
        MCK::Physics::RaycastHit hit = scene.physicsWorld.Raycast(rayStart, rayEnd);
        

        MCK::Rendering::Renderer::RenderFrame();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}
