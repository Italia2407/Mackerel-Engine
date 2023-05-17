#include "RenderingDemo.h"

namespace MCK
{
    Demo::RenderingDemo::RenderingDemo()
    {
       
    }

    Demo::RenderingDemo::~RenderingDemo()
    {
        // Delete resources: TODO
    }

    void Demo::RenderingDemo::Init()
    {
        #pragma region Rendering Init
                AssetType::Mesh* groundMesh = new AssetType::Mesh("Cube Mesh");
                groundMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");

                AssetType::Mesh* playerMesh = new AssetType::Mesh("Player Mesh");
                playerMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Suzanne.obj");

                meshMap["groundMesh"] = groundMesh;
                meshMap["playerMesh"] = playerMesh;

                AssetType::Material* greyMaterial = new AssetType::Material();
                greyMaterial->addUInt16Uniform("lightShaderID", 0);
                greyMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.8f, 0.8f, 0.8f));

                AssetType::Material* blueMaterial = new AssetType::Material();
                blueMaterial->addUInt16Uniform("lightShaderID", 0);
                blueMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.6f, 0.6f, 1.f));

                materialMap["greyMaterial"] = greyMaterial;
                materialMap["blueMaterial"] = blueMaterial;

                AssetType::Shader* m_UnlitShader;
                AssetType::Shader* m_MonoColourShader;

                //MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, m_UnlitShader);
                MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT_SHADOWS, m_UnlitShader);
                MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);

                shaderMap["m_UnlitShader"] = m_UnlitShader;
                shaderMap["m_MonoColourShader"] = m_MonoColourShader;

                //MCK::Rendering::Renderer::AddUnlitShader(m_UnlitShader);
                MCK::Rendering::Renderer::AddDirectionLightShader(m_UnlitShader);
        #pragma endregion

        #pragma region Floor Init
                floorTransform.Position() = Eigen::Vector3f(0, -8, -2);
                floorTransform.Scale() = Eigen::Vector3f(6, 6, 6);

                floorMesh = new EntitySystem::MeshRendererComponent(meshMap["groundMesh"], shaderMap["m_MonoColourShader"], materialMap["greyMaterial"]);


                Physics::CreateCollisionShapeInfo floorShape{};
                floorShape.colliderType = Physics::ColliderTypes::Box;
                floorShape.width = 6;
                floorShape.height = 6;
                floorShape.depth = 6;

                floorCollider = new Physics::CollisionComponent();
                floorCollider->SetCollisionShape(floorShape);
        #pragma endregion

        #pragma region Camera Init
                cameraComponent = new EntitySystem::PerspectiveCamera(1280.0f / 720.0f);
                cameraComponent->Position() = Eigen::Vector3f(0, 5, 10);
                cameraComponent->FrontDirection() = Eigen::Vector3f(0, -.7f, -1).normalized();
                cameraComponent->UpDirection() = Eigen::Vector3f(0, 1, 0).normalized();
                cameraFollowComponent = new EntitySystem::CameraFollowComponent();
        #pragma endregion

        #pragma region Player Init
                playerTransform = new EntitySystem::TransformComponent();
                playerTransform->Position() = Eigen::Vector3f(0, 20, 0);
                playerTransform->Scale() = Eigen::Vector3f(0.3f, 0.3f, 0.3f);

                // Physics
                Physics::CreateCollisionShapeInfo playerShape{};
                playerShape.colliderType = Physics::ColliderTypes::Box;
                playerShape.width = 0.3f;
                playerShape.height = 0.3f;
                playerShape.depth = 0.3f;

                playerBody = new Physics::RigidbodyComponent();
                playerBody->SetCollisionShape(playerShape);
                //playerBody->DisableRotation();

                playerRenderer = new EntitySystem::MeshRendererComponent(meshMap["playerMesh"], shaderMap["m_MonoColourShader"], materialMap["blueMaterial"]);

                playerInput = new EntitySystem::InputComponent();
                playerController = new ExamplePlayer::ExamplePlayerController();
        #pragma endregion

        #pragma region Audio Init
                audioTransform = new EntitySystem::TransformComponent();
                audioTransform->Position() = Eigen::Vector3f(-30.f, 0, 0);

                audioComponent = new EntitySystem::AudioEmitter();
                audioComponent->SetSoundFileName("../Mackerel-Core/res/Sounds/Voyager.mp3");
        #pragma endregion

        #pragma region UI Init
                uiComponent = new EntitySystem::UIComponent();
                MCK::AssetType::Texture* hudIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/PlayRendering.png");
                uiComponent->CreateShape(true, ImVec2(10, 10), 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, ImVec2(276, 100), ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f, hudIMG);
        #pragma endregion
    }

    void  Demo::RenderingDemo::AddEntities(EntitySystem::Scene& scene)
    {
        EntitySystem::Entity* floorEntity = scene.CreateEntity();
        floorEntity->AddComponent(&floorTransform);
        floorEntity->AddComponent(floorMesh);
        floorEntity->AddComponent(floorCollider);

        EntitySystem::Entity* cameraEntity = scene.CreateEntity();
        cameraEntity->AddComponent(cameraComponent);
        cameraEntity->AddComponent(cameraFollowComponent);

        EntitySystem::Entity* playerEntity = scene.CreateEntity();
        playerEntity->AddTag("Player");
        playerEntity->AddComponent(playerTransform);
        playerEntity->AddComponent(playerBody);
        playerEntity->AddComponent(playerRenderer);
        playerEntity->AddComponent(playerInput);
        playerEntity->AddComponent(playerController);

        EntitySystem::Entity* audioEntity = scene.CreateEntity();
        audioEntity->AddComponent(audioTransform);
        audioEntity->AddComponent(audioComponent);

        EntitySystem::Entity* uiEntity = scene.CreateEntity();
        uiEntity->AddComponent(uiComponent);
    }
}


