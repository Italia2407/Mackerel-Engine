#include "EvaluationApp.h"

EvaluationApp::EvaluationApp()
{}
EvaluationApp::~EvaluationApp()
{}

void EvaluationApp::Start()
{
    RegisterComponents();

#pragma region Scene Init
    scene.InitialiseScene();
    
#pragma endregion

#pragma region Rendering Init

    cubeMesh = new MCK::AssetType::Mesh("Cube Mesh");
    cubeMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");

    sphereMesh = new MCK::AssetType::Mesh("Spehere Mesh");
    sphereMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Suzanne.obj");

    greyMaterial = new MCK::AssetType::Material();
    greyMaterial->addUInt16Uniform("lightShaderID", 0);
    greyMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.8f, 0.8f, 0.8f));

    blueMaterial = new MCK::AssetType::Material();
    blueMaterial->addUInt16Uniform("lightShaderID", 0);
    blueMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.6f, 0.6f, 1.f));

    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT_SHADOWS, m_UnlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);

    MCK::Rendering::Renderer::AddDirectionLightShader(m_UnlitShader);
    light = new MCK::Rendering::DirectionLight(Eigen::Vector3f(-0.3f, -1.0f, -0.2f), Eigen::Vector4f(1.0f, 1.0f, 1.0f, 1.0f), Eigen::Vector4f::Zero(), Eigen::Vector4f(0.1f, 0.1f, 0.1f, 1.0f));
#pragma endregion

#pragma region Floor Init
    
    floorTransform.Position() = Eigen::Vector3f(0, -2, -0);
    floorTransform.Scale() = Eigen::Vector3f(10, 0.5, 10);


    Physics::CreateCollisionShapeInfo floorShape{};
    floorShape.colliderType = Physics::ColliderTypes::Box;
    floorShape.width = 20;
    floorShape.height = 0.5;
    floorShape.depth = 20;

    Physics::CollisionComponent* floorCollider = new Physics::CollisionComponent();


    EntitySystem::Entity* floorEntity = scene.CreateEntity();
    floorEntity->AddComponent(&floorTransform);
    floorCollider->SetCollisionShape(floorShape);
    floorEntity->AddComponent(floorCollider);
    
#pragma endregion

#pragma region Camera Init
    cameraComponent = new EntitySystem::PerspectiveCamera(1280.0f / 720.0f);
    cameraComponent->Position() = Eigen::Vector3f(0, 13, 22);
    cameraComponent->FrontDirection() = Eigen::Vector3f(0, -.7f, -1).normalized();
    cameraComponent->UpDirection() = Eigen::Vector3f(0, 1, 0).normalized();

    EntitySystem::Entity* cameraEntity = scene.CreateEntity();
    cameraEntity->AddComponent(cameraComponent);

#pragma endregion

#pragma region Player Init

    for (int i = 0; i < rigidbodyCount; i++)
    {
    EntitySystem::TransformComponent* playerTransform = new EntitySystem::TransformComponent();
    playerTransform->Position() = Eigen::Vector3f(0, 20, 0);
    playerTransform->Scale() = Eigen::Vector3f(0.3f, 0.3f, 0.3f);

    // Physics
    Physics::CreateCollisionShapeInfo playerShape{};
    playerShape.colliderType = Physics::ColliderTypes::Box;
    playerShape.width = 0.3f;
    playerShape.height = 0.3f;
    playerShape.depth = 0.3f;

    Physics::RigidbodyComponent* playerBody = new Physics::RigidbodyComponent();
    playerBody->SetCollisionShape(playerShape);
    //playerBody->DisableRotation();

    EntitySystem::MeshRendererComponent* playerRenderer
        = new EntitySystem::MeshRendererComponent(cubeMesh, m_MonoColourShader, blueMaterial);

    
        EntitySystem::Entity* playerEntity = scene.CreateEntity();
        playerEntity->AddComponent(playerTransform);
        playerEntity->AddComponent(playerBody);
        playerEntity->AddComponent(playerRenderer);
    }

#pragma endregion

#pragma region Object Init
    

    for (float i = 0; i < objectCount; i++)
    {
        EntitySystem::TransformComponent* objectTransform = new EntitySystem::TransformComponent();
        objectTransform->Position() = Eigen::Vector3f(-10+i*0.1f, 2,  float((int)i %10));
        objectTransform->Scale() = Eigen::Vector3f(0.3f, 0.3f, 0.3f);

        // Physics
        Physics::CreateCollisionShapeInfo objectShape{};
        objectShape.colliderType = Physics::ColliderTypes::Box;
        objectShape.width = 0.3f;
        objectShape.height = 0.3f;
        objectShape.depth = 0.3f;


        EntitySystem::MeshRendererComponent* objectRenderer
            = new EntitySystem::MeshRendererComponent(sphereMesh, m_MonoColourShader, blueMaterial);


        EntitySystem::Entity* objectEntity = scene.CreateEntity();
        objectEntity->AddComponent(objectTransform);
        objectEntity->AddComponent(objectRenderer);
    }
    

#pragma endregion

#pragma region Calculation

    for (int i = 0; i < entitiesCount; i++)
    {

    }

#pragma endregion

#pragma region UI Init

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

    EntitySystem::UIComponent* uiComponent = new EntitySystem::UIComponent();

    // Create FPS counter in bottom left
    uiComponent->CreateText(true, ImVec2(10, windowSize.y - 40), 1.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, "", [this]() { return this->GetCurrentFPS(); });

    // Dislay object count in bottom left, under FPS
    uiComponent->CreateText(true, ImVec2(10, windowSize.y - 20), 1.0f, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, "", [this]() { return this->GetCount(); });



    EntitySystem::Entity* uiEntity = scene.CreateEntity();
    uiEntity->AddComponent(uiComponent);

#pragma endregion

}

std::string EvaluationApp::GetCount() {

    triangleCount = objectCount * 12;

    // Format string.
    char buffer[100];
    sprintf(buffer, "Rigid bodies: %i, Triangles: %i, Lights: %i, Animated meshes: %i", rigidbodyCount, triangleCount, lightCount, animatedCount);
    return std::string(buffer);
}

std::string EvaluationApp::GetCurrentFPS() {
    // Get the current frames per second.
    double fps = MCK::TimeManager::getFPS();

    if (fps > maxFPS)
        maxFPS = fps;
    if (fps < minFPS)
        minFPS = fps;

    if (max != 0)
        before = now;

    now = MCK::TimeManager::GetUpTime();

    double new_max = now - before;
    if (new_max > max)
        max = new_max;

    // Format string.
    char buffer[100];
    sprintf(buffer, "FPS: %.0f,  Max FPS: %.0f,   Min FPS: %.0f, Max Time taken: %.2f", fps, maxFPS, minFPS, max);
    return std::string(buffer);
}

void EvaluationApp::Update()
{
    TimeManager::Update();

    Rendering::Renderer::QueueDirectionLight(light);

    scene.UpdateScene();
}

void EvaluationApp::End()
{
    scene.UnloadScene();
}

