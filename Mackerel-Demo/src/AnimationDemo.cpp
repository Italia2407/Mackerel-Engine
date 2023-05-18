#include "AnimationDemo.h"

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
            boneMesh = new AssetType::Mesh("Bone Mesh");
            boneMesh->LoadFromFile("../Mackerel-Core/res/Meshes/xbot_binary.glb");
            cubeMesh = new AssetType::Mesh("Cube Mesh");
            cubeMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");

            greyMaterial = new AssetType::Material();
            greyMaterial->addUInt16Uniform("lightShaderID", 0);
            greyMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.8f, 0.8f, 0.8f));

            yellowMaterial = new AssetType::Material();
            yellowMaterial->addUInt16Uniform("lightShaderID", 0);
            yellowMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.8f, 0.8f, 0.2f));

            //MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT, m_UnlitShader);
            MCK::ShaderLibrary::GetShader(ShaderEnum::__LIGHT_UNLIT_SHADOWS, m_UnlitShader);
            MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);

            //MCK::Rendering::Renderer::AddUnlitShader(m_UnlitShader);
            MCK::Rendering::Renderer::AddDirectionLightShader(m_UnlitShader);
        #pragma endregion

        #pragma region Floor Init
            floorTransform.Position() = Eigen::Vector3f(0.0f, -5.0f, -2.0f);
            floorTransform.Scale() = Eigen::Vector3f(100.0f, 0.1f, 100.0f);

            floorMesh = new EntitySystem::MeshRendererComponent(cubeMesh, m_MonoColourShader, greyMaterial);

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
        #pragma endregion

        #pragma region Player Init
            playerTransform = new EntitySystem::TransformComponent();
            playerTransform->Position() = Eigen::Vector3f(0.0f, 0.0f, 5.0f);
            playerTransform->Scale() = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

            playerRenderer = new EntitySystem::SkinnedMeshRendererComponent(boneMesh, m_MonoColourShader, yellowMaterial);
            playerRenderer->SetDefaultAnimation("dance");
            playerRenderer->PlayAnimation("dance", 0.0f, true, false, true);
            playerRenderer->SetTargetFPS(60.0f);
        #pragma endregion

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

        EntitySystem::Entity* cameraEnttiy = scene.CreateEntity();
        cameraEnttiy->AddComponent(cameraComponent);

        EntitySystem::Entity* playerEntity = scene.CreateEntity();
        playerEntity->AddComponent(playerTransform);
        playerEntity->AddComponent(playerRenderer);

        EntitySystem::Entity* uiEntity = scene.CreateEntity();
        uiEntity->AddComponent(uiComponent);
    }
}


