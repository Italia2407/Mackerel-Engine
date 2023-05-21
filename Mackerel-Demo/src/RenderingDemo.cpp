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
#pragma region Meshes Init
        AssetType::Mesh* boneMesh = new AssetType::Mesh("Bone Mesh");
        boneMesh->LoadFromFile("../Mackerel-Core/res/Meshes/xbot.gltf");
        AssetType::Mesh* cubeMesh = new AssetType::Mesh("Cube Mesh");
        cubeMesh->LoadFromFile("../Mackerel-Core/res/Meshes/Primitives/cube.obj");

        meshMap["boneMesh"] = boneMesh;
        meshMap["cubeMesh"] = cubeMesh;
#pragma endregion

#pragma region Materials Init
        AssetType::Material* floorMaterial = new AssetType::Material();
        floorMaterial->addUInt16Uniform("lightShaderID", 0);
        floorMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.4f, 0.3f, 0.15f));

        AssetType::Material* blackMaterial = new AssetType::Material();
        blackMaterial->addUInt16Uniform("lightShaderID", 0);
        blackMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.0f, 0.0f, 0.0f));

        AssetType::Material* podiumMaterial = new AssetType::Material();
        podiumMaterial->addUInt16Uniform("lightShaderID", 0);
        podiumMaterial->addVec3Uniform("albedoColour", Eigen::Vector3f(0.85f, 0.85f, 0.85f));

        AssetType::Material* skin = new AssetType::Material();
        skin->LoadFromFile("../Mackerel-Core/res/Materials/xbot.mtl", 0);

        materialMap["floorMaterial"] = floorMaterial;
        materialMap["blackMaterial"] = blackMaterial;
        materialMap["podiumMaterial"] = podiumMaterial;
        materialMap["skin"] = skin;
#pragma endregion

#pragma region Shaders Init
        AssetType::Shader* m_MonoColourShader;
        AssetType::Shader* m_texturedShader;

        MCK::ShaderLibrary::GetShader(ShaderEnum::__FRAG_MONOCOLOUR, m_MonoColourShader);
        MCK::ShaderLibrary::LoadShader(ShaderEnum::textured, "../Mackerel-Core/res/Shaders/frag/textured.glsl");
        MCK::ShaderLibrary::GetShader(ShaderEnum::textured, m_texturedShader);

        shaderMap["m_MonoColourShader"] = m_MonoColourShader;
        shaderMap["m_texturedShader"] = m_texturedShader;
#pragma endregion

#pragma region Floor Init
        floorTransform = new EntitySystem::TransformComponent();
        floorTransform->Position() = Eigen::Vector3f(0, -5, -0);
        floorTransform->Scale() = Eigen::Vector3f(6, 3, 6);

        floorMesh = new EntitySystem::MeshRendererComponent(meshMap["cubeMesh"], shaderMap["m_MonoColourShader"], materialMap["floorMaterial"]);

        Physics::CreateCollisionShapeInfo floorShape{};
        floorShape.colliderType = Physics::ColliderTypes::Box;
        floorShape.width = 6;
        floorShape.height = 3;
        floorShape.depth = 6;
        floorShape.mesh = meshMap["cubeMesh"];

        floorCollider = new Physics::CollisionComponent();
        floorCollider->SetCollisionShape(floorShape);
#pragma endregion

#pragma region Death Floor Init
        deathFloorTransform = new EntitySystem::TransformComponent();
        deathFloorTransform->Position() = Eigen::Vector3f(0, -30, 0);
        deathFloorTransform->Scale() = Eigen::Vector3f(30.0f, 0.1f, 30.0f);

        deathFloorMesh = new EntitySystem::MeshRendererComponent(meshMap["cubeMesh"], shaderMap["m_MonoColourShader"], materialMap["blackMaterial"]);

        Physics::CreateCollisionShapeInfo deathFloorShape{};
        deathFloorShape.colliderType = Physics::ColliderTypes::Box;
        deathFloorShape.width = 30;
        deathFloorShape.height = 0.1f;
        deathFloorShape.depth = 30;

        deathFloorCollider = new Physics::CollisionComponent();
        deathFloorCollider->SetCollisionShape(deathFloorShape);
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
        q = Eigen::AngleAxisf(PI, Eigen::Vector3f::UnitY());

        playerBaseTransform = new EntitySystem::TransformComponent();
        playerBaseTransform->Position() = Eigen::Vector3f(0.0f, 10.0f, 0.0f);
        playerBaseTransform->Scale() = Eigen::Vector3f(playerSize, playerSize, playerSize);
        playerBaseTransform->Rotation() = q;

        playerRenderer = new EntitySystem::SkinnedMeshRendererComponent(meshMap["boneMesh"], shaderMap["m_texturedShader"], materialMap["skin"]);
        playerRenderer->SetDefaultAnimation("idle");
        playerRenderer->SetTargetFPS(60.0f);

        Physics::CreateCollisionShapeInfo playerShape{};
        playerShape.colliderType = Physics::ColliderTypes::Box;
        playerShape.width = 0.2f;
        playerShape.height = playerSize;
        playerShape.depth = 0.2f;

        playerBody = new Physics::RigidbodyComponent();
        playerBody->SetCollisionShape(playerShape);

        playerInput = new EntitySystem::InputComponent();
        playerController = new ExamplePlayer::ExamplePlayerController();

        playerTransform = new EntitySystem::TransformComponent();
        playerTransform->Position() = Eigen::Vector3f(0.0f, -1.0f, 0.0f);
#pragma endregion

#pragma region UI Init
        uiComponent = new EntitySystem::UIComponent();
        MCK::AssetType::Texture* hudIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/PlayAnimation.png");
        uiComponent->CreateShape(true, ImVec2(10, 10), 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, ImVec2(276, 100), ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f, hudIMG);
#pragma endregion
    }

    void  Demo::RenderingDemo::AddEntities(EntitySystem::Scene& scene)
    {
        scene.LoadSceneAdditive("../scenes/lvl1/scene.scn");
        scene.LoadSceneAdditive("../scenes/lvl1/LightScene/scene.scn");

        EntitySystem::Entity* deathFloorEntity = scene.CreateEntity();
        deathFloorEntity->AddTag("death");
        deathFloorEntity->AddComponent(deathFloorTransform);
        deathFloorEntity->AddComponent(deathFloorMesh);
        deathFloorEntity->AddComponent(deathFloorCollider);

        EntitySystem::Entity* cameraEntity = scene.CreateEntity();
        cameraEntity->AddComponent(cameraComponent);
        cameraEntity->AddComponent(cameraFollowComponent);

        EntitySystem::Entity* playerBaseEntity = scene.CreateEntity();
        playerBaseEntity->AddTag("Player");
        playerBaseEntity->AddComponent(playerBaseTransform);
        playerBaseEntity->AddComponent(playerBody);
        playerBaseEntity->AddComponent(playerInput);
        playerBaseEntity->AddComponent(playerController);

        EntitySystem::Entity* playerEntity = scene.CreateEntity();
        playerEntity->AddComponent(playerRenderer);
        playerEntity->AddComponent(playerTransform);

        playerBaseEntity->AddChild(playerEntity);

        for (const auto& item : skinnedMeshes) {
            EntitySystem::Entity* newEntity = scene.CreateEntity();
            newEntity->AddComponent(transforms[item.second]);
            newEntity->AddComponent(item.first);
        }

        for (const auto& item : meshes) {
            EntitySystem::Entity* newEntity = scene.CreateEntity();
            newEntity->AddComponent(transforms[item.second]);
            newEntity->AddComponent(item.first);
        }

        EntitySystem::Entity* uiEntity = scene.CreateEntity();
        uiEntity->AddComponent(uiComponent);
    }
}


