#include "AnimationDemo.h"

const float PI = 3.14159265358979323846f;

namespace MCK
{
    Demo::AnimationDemo::AnimationDemo()
    {
       
    }

    Demo::AnimationDemo::~AnimationDemo()
    {
        // Delete resources: TODO
    }

    void Demo::AnimationDemo::Init()
    {
        #pragma region Rendering Init
            AssetType::Mesh* boneMesh = new AssetType::Mesh("Bone Mesh");
            boneMesh->LoadFromFile("../Mackerel-Core/res/Meshes/xbot.gltf");
            AssetType::Mesh* groundMesh = new AssetType::Mesh("Cube Mesh");
            groundMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");

            meshMap["boneMesh"] = boneMesh;
            meshMap["groundMesh"] = groundMesh;

            AssetType::Material* floorMaterial = new AssetType::Material();
            floorMaterial->addUInt16Uniform("lightShaderID", 0);
            floorMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.2f, 0.5f, 0.2f));

            AssetType::Material* skin = new AssetType::Material();
            skin->addUInt16Uniform("lightShaderID", 0);
            skin->addVec3Uniform("albedoColour", Eigen::Vector3f(0.8f, 0.2f, 0.2f));

            materialMap["floorMaterial"] = floorMaterial;
            materialMap["skin"] = skin;

            AssetType::Shader* m_MonoColourShader;
            AssetType::Shader* m_texturedShader;

            MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);
            MCK::ShaderLibrary::LoadShader(ShaderEnum::textured, "../Mackerel-Core/res/Shaders/frag/textured.glsl");
            MCK::ShaderLibrary::GetShader(ShaderEnum::textured, m_texturedShader);

            shaderMap["m_MonoColourShader"] = m_MonoColourShader;
            shaderMap["m_texturedShader"] = m_texturedShader;
        #pragma endregion

        #pragma region Floor Init
            floorTransform.Position() = Eigen::Vector3f(0, -8, -2);
            floorTransform.Scale() = Eigen::Vector3f(6, 6, 6);

            floorMesh = new EntitySystem::MeshRendererComponent(meshMap["groundMesh"], shaderMap["m_MonoColourShader"], materialMap["floorMaterial"]);

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
            cameraComponent->FrontDirection() = Eigen::Vector3f(0, -.6f, -1).normalized();
            cameraComponent->UpDirection() = Eigen::Vector3f(0, 1, 0).normalized();
            cameraFollowComponent = new EntitySystem::CameraFollowComponent();
        #pragma endregion

        #pragma region Player Init
            float playerSize = 1.0f;
            Eigen::Quaternionf q;
            q = Eigen::AngleAxisf(PI, Eigen::Vector3f::UnitY()); // M_PI is Pi in C++, equivalent to 180 degrees

            playerTransform = new EntitySystem::TransformComponent();
            playerTransform->Position() = Eigen::Vector3f(0.0f, 0.0f, 0.0f);
            playerTransform->Scale() = Eigen::Vector3f(playerSize, playerSize, playerSize);
            playerTransform->Rotation() = q;

            playerRenderer = new EntitySystem::SkinnedMeshRendererComponent(meshMap["boneMesh"], shaderMap["m_MonoColourShader"], materialMap["skin"]);
            playerRenderer->SetDefaultAnimation("idle");
            playerRenderer->SetTargetFPS(60.0f);

            Physics::CreateCollisionShapeInfo playerShape{};
            playerShape.colliderType = Physics::ColliderTypes::Box;
            playerShape.width = playerSize;
            playerShape.height = playerSize;
            playerShape.depth = playerSize;

            playerBody = new Physics::RigidbodyComponent();
            playerBody->SetCollisionShape(playerShape);

            playerInput = new EntitySystem::InputComponent();
            playerController = new ExamplePlayer::ExamplePlayerController();
        #pragma endregion

            leftModelTransform = new EntitySystem::TransformComponent();
            leftModelTransform->Position() = Eigen::Vector3f(-3.5f, 0.0f, 0.0f);
            leftModelTransform->Scale() = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
            leftModelTransform->Rotation() = q;
            leftModelRenderer = new EntitySystem::SkinnedMeshRendererComponent(meshMap["boneMesh"], shaderMap["m_MonoColourShader"], materialMap["skin"]);
            leftModelRenderer->SetDefaultAnimation("run");
            
            rightModelTransform = new EntitySystem::TransformComponent();
            rightModelTransform->Position() = Eigen::Vector3f(3.5f, 0.0f, 0.0f);
            rightModelTransform->Scale() = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
            rightModelTransform->Rotation() = q;
            rightModelRenderer = new EntitySystem::SkinnedMeshRendererComponent(meshMap["boneMesh"], shaderMap["m_MonoColourShader"], materialMap["skin"]);
            rightModelRenderer->SetDefaultAnimation("dance");

        #pragma region UI Init
            uiComponent = new EntitySystem::UIComponent();
            MCK::AssetType::Texture* hudIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/PlayAnimation.png");
            uiComponent->CreateShape(true, ImVec2(10, 10), 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, ImVec2(276, 100), ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f, hudIMG);
        #pragma endregion
    }

    void  Demo::AnimationDemo::AddEntities(EntitySystem::Scene& scene)
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

        EntitySystem::Entity* leftModelEntity = scene.CreateEntity();
        leftModelEntity->AddComponent(leftModelTransform);
        leftModelEntity->AddComponent(leftModelRenderer);

        EntitySystem::Entity* rightModelEntity = scene.CreateEntity();
        rightModelEntity->AddComponent(rightModelTransform);
        rightModelEntity->AddComponent(rightModelRenderer);

        EntitySystem::Entity* uiEntity = scene.CreateEntity();
        uiEntity->AddComponent(uiComponent);
    }
}


