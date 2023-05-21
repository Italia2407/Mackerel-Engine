#include "PlatformerDemo.h"
#include "CameraFollowComponent.h"

PlatformerApp::PlatformerApp() 
{}
PlatformerApp::~PlatformerApp()
{}

void PlatformerApp::Start()
{
    RegisterComponents();

#pragma region Scene Init
    scene.InitialiseScene();
    scene.LoadScene("../scenes/lvl2/scene.scn");
    scene.LoadSceneAdditive("../scenes/lvl2/DynamicScene/scene.scn");
    //MCK::Logger::initialize();
#pragma endregion

#pragma region Rendering Init

    bridgeMesh = new MCK::AssetType::Mesh("Bridge Mesh");
    //bridgeMesh->LoadFromFile("../scenes/lvl1/IslandGeo.obj");
   // bridgeMesh->LoadFromFile("../scenes/lvl1/Bridge.obj");
    bridgeMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");

    cubeMesh = new MCK::AssetType::Mesh("Cube Mesh");
    cubeMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj"); 

    greyMaterial = new MCK::AssetType::Material();
    greyMaterial->addUInt16Uniform("lightShaderID", 0);
    greyMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.8f, 0.8f, 0.8f));

    blueMaterial = new MCK::AssetType::Material();
    blueMaterial->addUInt16Uniform("lightShaderID", 0);
    blueMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.6f, 0.6f, 1.f));

    //MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, m_UnlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT_SHADOWS, m_UnlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);

    //MCK::Rendering::Renderer::AddUnlitShader(m_UnlitShader);
    MCK::Rendering::Renderer::AddDirectionLightShader(m_UnlitShader);

    light = new Rendering::DirectionLight(Eigen::Vector3f(-0.3f, -1.0f, -0.2f).normalized(), Eigen::Vector4f::Ones(), Eigen::Vector4f::Zero(), Eigen::Vector4f::Ones() / 2.f);
#pragma endregion

#pragma region Floor Init
    floorTransform.Position() = Eigen::Vector3f(0,-2, -0);
    floorTransform.Scale() = Eigen::Vector3f(1, 1, 1);

    //floorMesh = new EntitySystem::MeshRendererComponent(bridgeMesh, m_MonoColourShader, greyMaterial);
    floorMesh = new EntitySystem::MeshRendererComponent(bridgeMesh, m_MonoColourShader, greyMaterial);

    Physics::CreateCollisionShapeInfo floorShape{};
    floorShape.colliderType = Physics::ColliderTypes::Mesh;
    floorShape.width = 1;
    floorShape.height = 1;
    floorShape.depth = 1;
    floorShape.mesh = bridgeMesh;

    Physics::CollisionComponent* floorCollider = new Physics::CollisionComponent();
    

    EntitySystem::Entity* floorEntity = scene.CreateEntity();
    floorEntity->AddComponent(&floorTransform);
    floorEntity->AddComponent(floorMesh);
    floorCollider->SetCollisionShape(floorShape);
    floorEntity->AddComponent(floorCollider);
#pragma endregion

#pragma region Camera Init
    cameraComponent = new EntitySystem::PerspectiveCamera(1280.0f / 720.0f);
    cameraComponent->Position() = Eigen::Vector3f(0, 4, -10);
    cameraComponent->FrontDirection() = Eigen::Vector3f(0, -.7f, -1).normalized();
    cameraComponent->UpDirection() = Eigen::Vector3f(0, 1, 0).normalized();
    EntitySystem::CameraFollowComponent* cameraFollowComponent = new EntitySystem::CameraFollowComponent();

    EntitySystem::Entity* cameraEnttiy = scene.CreateEntity();
    cameraEnttiy->AddComponent(cameraComponent);
    cameraEnttiy->AddComponent(cameraFollowComponent);
    
#pragma endregion

#pragma region Player Init
    EntitySystem::TransformComponent* playerTransform = new EntitySystem::TransformComponent();
    playerTransform->Position() = Eigen::Vector3f(0, 10, 0);
    playerTransform->Scale() = Eigen::Vector3f(0.3f, 0.3f, 0.3f);

    EntitySystem::AudioListener* listener = new EntitySystem::AudioListener();

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

    EntitySystem::InputComponent* playerInput = new EntitySystem::InputComponent();
    ExamplePlayer::ExamplePlayerController* playerController
        = new ExamplePlayer::ExamplePlayerController();

    //EntitySystem::AudioListener* listener;

    EntitySystem::Entity* playerEntity = scene.CreateEntity();
    playerEntity->AddTag("Player");
    playerEntity->AddComponent(playerTransform);
    playerEntity->AddComponent(playerBody);
    playerEntity->AddComponent(playerRenderer);
    playerEntity->AddComponent(playerInput);
    playerEntity->AddComponent(playerController);
    playerEntity->AddComponent(listener);

#pragma endregion

#pragma region Audio Init
   //EntitySystem::TransformComponent* audioTransform = new EntitySystem::TransformComponent();
   //audioTransform->Position() = Eigen::Vector3f(30.f, 0, 0);
   //
   // EntitySystem::AudioEmitter* audioComponent = new EntitySystem::AudioEmitter();
   // audioComponent->SetSoundFileName("../Mackerel-Core/res/Sounds/Voyager.mp3");
   //
   // EntitySystem::Entity* audioEntity = scene.CreateEntity();
   // audioEntity->AddComponent(audioTransform);
   // audioEntity->AddComponent(audioComponent);

#pragma endregion

#pragma region UI Init
    EntitySystem::UIComponent* uiComponent = new EntitySystem::UIComponent();  
    //MCK::AssetType::Texture* image = uiComponent->LoadUIImage("../Mackerel-Core/res/Textures/TestImage.png");
    uiComponent->CreateStandardMenu();
    uiComponent->CreateStandardHUD(false);
    uiComponent->CreateButton(true, ImVec2(580, 280), 1.0f, ImVec4(0.2f, 0.6f, 1.0f, 1.0f), 1.0f, "Test Button", ImVec2(120, 40), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), [&]() {ButtonCallbackTestFunction(); }, true);
    
    EntitySystem::Entity* uiEntity = scene.CreateEntity();
    uiEntity->AddComponent(uiComponent);

#pragma endregion

}

void PlatformerApp::ButtonCallbackTestFunction()
{
    std::cout << "Button Pressed" << std::endl;
}

void PlatformerApp::Update()
{
    TimeManager::Update();
    //floorTransform.Rotation() = floorTransform.Rotation() * Eigen::AngleAxisf(0.001f, Eigen::Vector3f(0.0f, 1.0f, 0.0f));

    Rendering::Renderer::QueueDirectionLight(light);
    scene.UpdateScene();
}

void PlatformerApp::End()
{
    scene.UnloadScene();
}

