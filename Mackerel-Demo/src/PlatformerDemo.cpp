#include "PlatformerDemo.h"

PlatformerApp::PlatformerApp() 
{}
PlatformerApp::~PlatformerApp()
{}

void PlatformerApp::Start()
{

#pragma region Scene Init
    scene.InitialiseScene();
#pragma endregion

#pragma region Rendering Init
    cubeMesh = new MCK::AssetType::Mesh("Test Mesh");
    cubeMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj"); 
    //cubeMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Suzanne.obj");

    greyMaterial = new MCK::AssetType::Material();
    greyMaterial->addUInt16Uniform("lightShaderID", 0);
    greyMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.8f, 0.8f, 0.8f));

    blueMaterial = new MCK::AssetType::Material();
    blueMaterial->addUInt16Uniform("lightShaderID", 0);
    blueMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.6f, 0.6f, 1.f));

    MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, m_UnlitShader);
    MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);

    MCK::Rendering::Renderer::AddUnlitShader(m_UnlitShader);
#pragma endregion

#pragma region Floor Init
    floorTransform.Position() = Eigen::Vector3f(0,-8, 0);
    floorTransform.Scale() = Eigen::Vector3f(6, 6, 6);

    floorMesh = new EntitySystem::MeshRendererComponent(cubeMesh, m_MonoColourShader, greyMaterial);

    Physics::CreateCollisionShapeInfo floorShape{};
    floorShape.colliderType = Physics::ColliderTypes::Box;
    floorShape.width = 6;
    floorShape.height = 6;
    floorShape.depth = 6;

    Physics::CollisionComponent* floorCollider = new Physics::CollisionComponent();
    floorCollider->SetCollisionShape(floorShape);

    EntitySystem::Entity* floorEntity = scene.CreateEntity();
    floorEntity->AddComponent(&floorTransform);
    floorEntity->AddComponent(floorMesh);
    floorEntity->AddComponent(floorCollider);
#pragma endregion

#pragma region Camera Init
    cameraComponent = new EntitySystem::PerspectiveCamera(1280.0f / 720.0f);
    cameraComponent->Position() = Eigen::Vector3f(0, 5, -10);
    cameraComponent->FrontDirection() = Eigen::Vector3f(0, -.7f, 1).normalized();
    cameraComponent->UpDirection() = Eigen::Vector3f(0, 1, 0).normalized();

    EntitySystem::Entity* cameraEnttiy = scene.CreateEntity();
    cameraEnttiy->AddComponent(cameraComponent);
    
#pragma endregion

#pragma region Player Init
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

    EntitySystem::InputComponent* playerInput = new EntitySystem::InputComponent();
    ExamplePlayer::ExamplePlayerController* playerController
        = new ExamplePlayer::ExamplePlayerController();

    EntitySystem::Entity* playerEntity = scene.CreateEntity();
    playerEntity->AddComponent(playerTransform);
    playerEntity->AddComponent(playerBody);
    playerEntity->AddComponent(playerRenderer);
    playerEntity->AddComponent(playerInput);
    playerEntity->AddComponent(playerController);

#pragma endregion


}

void PlatformerApp::Update()
{
    TimeManager::Update();
    scene.UpdateScene();
}

void PlatformerApp::End()
{
    scene.UnloadScene();
}

