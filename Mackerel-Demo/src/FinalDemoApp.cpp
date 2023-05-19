#include "FinalDemoApp.h"



FinalDemoApp::FinalDemoApp()
{

}
FinalDemoApp::~FinalDemoApp()
{}

void FinalDemoApp::Start()
{
    RegisterComponents();

        #pragma region Demo Init
            renderingDemo.Init();
            physicsDemo.Init();
            animationDemo.Init();
            audioDemo.Init();
            loadedDemo = false;
        #pragma endregion

        #pragma region Scene Init
            constantScene.InitialiseScene();
            light = new MCK::Rendering::DirectionLight(Eigen::Vector3f(-0.3f, -1.0f, -0.2f), Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Eigen::Vector4f::Zero(), Eigen::Vector4f(0.1f, 0.1f, 0.1f, 1.0f));
            //MCK::Logger::initialize();
        #pragma endregion

        #pragma region UI Init
            
            EntitySystem::UIComponent* uiComponent = new EntitySystem::UIComponent();
            MCK::AssetType::Texture* renderingIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/ButtonRenderingSmall.png");
            MCK::AssetType::Texture* animationIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/ButtonAnimationSmall.png");
            MCK::AssetType::Texture* audioIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/ButtonAudioSmall.png");
            MCK::AssetType::Texture* physicsIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/ButtonPhysicsSmall.png");
            MCK::AssetType::Texture* quitIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/ButtonQuitSmall.png");
            MCK::AssetType::Texture* menuIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/MenuSmall.png");
            MCK::AssetType::Texture* hudIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/Time.png");

            //MCK::AssetType::Texture* menuIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/Textures/survivorMaleB.png");

            // Create a vector of callbacks
            std::vector<std::function<void()>> callbacks = {
                [&]() { LoadRenderingDemo(); },
                [&]() { LoadPhysicsDemo(); },
                [&]() { LoadAnimationDemo(); },
                [&]() { LoadAudioDemo(); },
                [&]() { End(); }
            };

            // Create a vector of button titles
            std::vector<MCK::AssetType::Texture*> buttonImages = {
                renderingIMG,
                physicsIMG,
                animationIMG,
                audioIMG,
                quitIMG
            };

            ImVec2 windowSize;
	        GLFWwindow* window = glfwGetCurrentContext(); // Get current context
	        if (window)
	        {
		        int width, height;
		        glfwGetFramebufferSize(window, &width, &height);

	        windowSize = ImVec2(static_cast<float>(width), static_cast<float>(height));
	        }
	        else
	        {
		        ImVec2 windowSize = ImVec2(1280, 720);
	        }

            ImVec2 menuSize = ImVec2(404, 524);
            ImVec2 menuPosition = ImVec2((windowSize.x - menuSize.x) / 2, (windowSize.y - menuSize.y) / 2);

            // Create a background shape
            uiComponent->CreateShape(true, ImVec2(0, 0), 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 1.0f), 0.5f, MCK::UI::ShapeElement::ShapeType::Rectangle, windowSize, ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 0.0f);
            uiComponent->CreateShape(true, menuPosition, 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, menuSize, ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f, menuIMG);

            // Create HUD time elapsed
            uiComponent->CreateShape(false, ImVec2(windowSize.x - 10 - 276, 10), 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, ImVec2(276, 100), ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f, hudIMG);
            uiComponent->CreateText(false, ImVec2(windowSize.x - 10 - 276 + 82, 65), 2.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, "", [this]() { return this->GetCurrentRuntime(); });

            // Create FPS counter in bottom left
            uiComponent->CreateText(false, ImVec2(10, windowSize.y - 20), 1.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, "", [this]() { return this->GetCurrentFPS(); });

            // Create button elements
            ImVec2 buttonSize = ImVec2(166, 61);
            int buttonOffset = 80; // Offset
            float startY = 15;
            for (int i = 0; i < 5; i++) {
                ImVec2 buttonPosition = ImVec2(menuPosition.x + (menuSize.x - buttonSize.x) / 2, startY + menuPosition.y + (menuSize.y - buttonSize.y) / 5 + i * buttonOffset);
                uiComponent->CreateButton(true, buttonPosition, 1.0f, ImVec4(0.6f, 0.0f, 1.0f, 0.0f), 1.0f, "", buttonSize, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), callbacks[i], true, buttonImages[i]);
            }

            EntitySystem::Entity* uiEntity = constantScene.CreateEntity();
            uiEntity->AddComponent(uiComponent);

            MCK::Logger::log("SETUP COMPLETED", MCK::Logger::LogLevel::Info, std::source_location::current());

        #pragma endregion

        
}

std::string FinalDemoApp::GetCurrentRuntime() {
    // Get the up time in seconds.
    double upTime = MCK::TimeManager::GetUpTime(); 

    // Convert to hours, minutes, and seconds.
    int hours = static_cast<int>(upTime) / 3600;
    upTime -= hours * 3600;
    int minutes = static_cast<int>(upTime) / 60;
    int seconds = static_cast<int>(upTime) % 60;

    // Format string.
    char buffer[100];
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
    return std::string(buffer);
}

std::string FinalDemoApp::GetCurrentFPS() {
    // Get the current frames per second.
    double fps = MCK::TimeManager::getFPS();

    // Format string.
    char buffer[100];
    sprintf(buffer, "FPS: %.0f", fps);
    return std::string(buffer);
}


void FinalDemoApp::LoadRenderingDemo()
{
    MCK::Logger::log("Loading Rendering Demo", MCK::Logger::LogLevel::Info, std::source_location::current());
    
    // Unload current scene
    if (loadedDemo)
        scene.UnloadScene();

    // Load new scene
    scene.InitialiseScene();
    renderingDemo.AddEntities(scene);
    
    loadedDemo = true;
}

void FinalDemoApp::LoadPhysicsDemo()
{
    MCK::Logger::log("Loading Physics Demo", MCK::Logger::LogLevel::Info, std::source_location::current());

    // Unload current scene
    if (loadedDemo)
        scene.UnloadScene();

    // Load new scene
    scene.InitialiseScene();
    physicsDemo.AddEntities(scene);

    loadedDemo = true;
}

void FinalDemoApp::LoadAnimationDemo()
{
    MCK::Logger::log("Loading Animation Demo", MCK::Logger::LogLevel::Info, std::source_location::current());

    // Unload current scene
    if (loadedDemo)
        scene.UnloadScene();

    // Load new scene
    scene.InitialiseScene();
    animationDemo.AddEntities(scene);

    loadedDemo = true;
}

void FinalDemoApp::LoadAudioDemo()
{
    MCK::Logger::log("Loading Audio Demo", MCK::Logger::LogLevel::Info, std::source_location::current());

    // Unload current scene
    if (loadedDemo)
        scene.UnloadScene();

    // Load new scene
    scene.InitialiseScene();
    audioDemo.AddEntities(scene);

    loadedDemo = true;
}

void FinalDemoApp::Update()
{
    TimeManager::Update();
    //floorTransform.Rotation() = floorTransform.Rotation() * Eigen::AngleAxisf(0.001f, Eigen::Vector3f(0.0f, 1.0f, 0.0f));

    if (loadedDemo)
    {
        Rendering::Renderer::QueueDirectionLight(light);
        scene.UpdateScene();
    }

    constantScene.UpdateScene();
}

void FinalDemoApp::End()
{
    MCK::Logger::log("EXITING", MCK::Logger::LogLevel::Info, std::source_location::current());

    if (loadedDemo)
        scene.UnloadScene();
    constantScene.UnloadScene();
}

