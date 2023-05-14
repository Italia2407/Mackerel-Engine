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
            light = new Rendering::DirectionLight(Eigen::Vector3f(-0.3f, -1.0f, -0.2f).normalized(), Eigen::Vector4f::Zero(), Eigen::Vector4f::Zero(), Eigen::Vector4f::Zero());
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
                animationIMG,
                audioIMG,
                physicsIMG,
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

            // Create a title text element
            //ImVec2 titlePosition = ImVec2(menuPosition.x + (menuSize.x / 2) - 15, menuPosition.y + 40);
            //uiComponent->CreateText(true, titlePosition, 5.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, "MENU");

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

        #pragma endregion

}

void FinalDemoApp::LoadRenderingDemo()
{
    std::cout << "Loading Rendering Demo" << std::endl;
    
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
    std::cout << "Loading Physics Demo" << std::endl;

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
    std::cout << "Loading Animation Demo" << std::endl;

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
    std::cout << "Loading Audio Demo" << std::endl;

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
    if (loadedDemo)
        scene.UnloadScene();
    constantScene.UnloadScene();
}

