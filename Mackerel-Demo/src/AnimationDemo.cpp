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

            loaded = false;
    }

    void  Demo::AnimationDemo::AddEntities(EntitySystem::Scene& scene)
    {
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

        #pragma region AI Init
        AIBaseTransform = new EntitySystem::TransformComponent();
        AIBaseTransform->Position() = Eigen::Vector3f(-3.5f, -1.0f, -2.0f);
        AIBaseTransform->Scale() = Eigen::Vector3f(playerSize, playerSize, playerSize);

        AIRenderer = new EntitySystem::SkinnedMeshRendererComponent(meshMap["boneMesh"], shaderMap["m_texturedShader"], materialMap["skin"]);
        AIRenderer->SetDefaultAnimation("run");
        AIRenderer->SetTargetFPS(60.0f);

        Physics::CreateCollisionShapeInfo AIShape{};
        AIShape.colliderType = Physics::ColliderTypes::Box;
        AIShape.width = 0.2f;
        AIShape.height = playerSize;
        AIShape.depth = 0.2f;

        AIBody = new Physics::RigidbodyComponent();
        AIBody->SetCollisionShape(playerShape);

        AIMove = new EntitySystem::AIMovement();
        AIMove->setEndPosition(Eigen::Vector3f(-3.5f, 0.0f, 5.0f));

        AITransform = new EntitySystem::TransformComponent();
        AITransform->Position() = Eigen::Vector3f(0.0f, -1.0f, 0.0f);
#pragma endregion

        #pragma region Items Init
        EntitySystem::TransformComponent* leftModelTransform = new EntitySystem::TransformComponent();
        leftModelTransform->Position() = Eigen::Vector3f(-3.5f, -1.25f, -4.0f);
        leftModelTransform->Scale() = Eigen::Vector3f(1.0f, 1.0f, 1.0f);

        EntitySystem::SkinnedMeshRendererComponent* leftModelRenderer = new EntitySystem::SkinnedMeshRendererComponent(meshMap["boneMesh"], shaderMap["m_texturedShader"], materialMap["skin"]);
        leftModelRenderer->SetDefaultAnimation("run");
        leftModelRenderer->SetTargetFPS(60.0f);

        EntitySystem::TransformComponent* rightModelTransform = new EntitySystem::TransformComponent();
        rightModelTransform->Position() = Eigen::Vector3f(3.5f, -1.25f, -4.0f);
        rightModelTransform->Scale() = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
        EntitySystem::SkinnedMeshRendererComponent* rightModelRenderer = new EntitySystem::SkinnedMeshRendererComponent(meshMap["boneMesh"], shaderMap["m_texturedShader"], materialMap["skin"]);
        rightModelRenderer->SetDefaultAnimation("dance");
        rightModelRenderer->SetTargetFPS(60.0f);

        EntitySystem::TransformComponent* p1Transform = new EntitySystem::TransformComponent();
        p1Transform->Position() = Eigen::Vector3f(3.5f, -1.5f, -4.0f);
        p1Transform->Scale() = Eigen::Vector3f(0.7f, 0.25f, 0.5f);
        EntitySystem::MeshRendererComponent* p1Renderer = new EntitySystem::MeshRendererComponent(meshMap["cubeMesh"], shaderMap["m_MonoColourShader"], materialMap["podiumMaterial"]);

        EntitySystem::TransformComponent* p1BaseTransform = new EntitySystem::TransformComponent();
        p1BaseTransform->Position() = Eigen::Vector3f(3.5f, -1.875f, -4.0f);
        p1BaseTransform->Scale() = Eigen::Vector3f(1.2f, 0.125f, 1.0f);
        EntitySystem::MeshRendererComponent* p1BaseRenderer = new EntitySystem::MeshRendererComponent(meshMap["cubeMesh"], shaderMap["m_MonoColourShader"], materialMap["podiumMaterial"]);

        EntitySystem::TransformComponent* p2Transform = new EntitySystem::TransformComponent();
        p2Transform->Position() = Eigen::Vector3f(-3.5f, -1.5f, -4.0f);
        p2Transform->Scale() = Eigen::Vector3f(0.7f, 0.25f, 0.5f);
        EntitySystem::MeshRendererComponent* p2Renderer = new EntitySystem::MeshRendererComponent(meshMap["cubeMesh"], shaderMap["m_MonoColourShader"], materialMap["podiumMaterial"]);

        EntitySystem::TransformComponent* p2BaseTransform = new EntitySystem::TransformComponent();
        p2BaseTransform->Position() = Eigen::Vector3f(-3.5f, -1.875f, -4.0f);
        p2BaseTransform->Scale() = Eigen::Vector3f(1.2f, 0.125f, 1.0f);
        EntitySystem::MeshRendererComponent* p2BaseRenderer = new EntitySystem::MeshRendererComponent(meshMap["cubeMesh"], shaderMap["m_MonoColourShader"], materialMap["podiumMaterial"]);

        transforms.push_back(leftModelTransform);
        transforms.push_back(rightModelTransform);
        transforms.push_back(p1BaseTransform);
        transforms.push_back(p1Transform);
        transforms.push_back(p2BaseTransform);
        transforms.push_back(p2Transform);

        skinnedMeshes.insert(std::make_pair(leftModelRenderer, 0));
        skinnedMeshes.insert(std::make_pair(rightModelRenderer, 1));

        meshes.insert(std::make_pair(p1BaseRenderer, 2));
        meshes.insert(std::make_pair(p1Renderer, 3));
        meshes.insert(std::make_pair(p2BaseRenderer, 4));
        meshes.insert(std::make_pair(p2Renderer, 5));
#pragma endregion

        #pragma region UI Init
        uiComponent = new EntitySystem::UIComponent();
        MCK::AssetType::Texture* hudIMG = uiComponent->LoadUIImage("../Mackerel-Core/res/UI/PlayAnimation.png");
        uiComponent->CreateShape(true, ImVec2(10, 10), 1.0f, ImVec4(0.3f, 0.3f, 0.3f, 0.6f), 1.0f, MCK::UI::ShapeElement::ShapeType::Rectangle, ImVec2(276, 100), ImVec4(0.8f, 0.8f, 0.8f, 1.0f), 2.0f, hudIMG);
#pragma endregion

        components.push_back(floorTransform);
        components.push_back(floorMesh);
        components.push_back(floorCollider);
        components.push_back(deathFloorTransform);
        components.push_back(deathFloorMesh);
        components.push_back(deathFloorCollider);
        components.push_back(cameraComponent);
        components.push_back(cameraFollowComponent);
        components.push_back(playerBaseTransform);
        components.push_back(playerBody);
        components.push_back(playerRenderer);
        components.push_back(playerInput);
        components.push_back(playerController);
        components.push_back(playerTransform);
        components.push_back(AIBaseTransform);
        components.push_back(AIRenderer);
        components.push_back(AIBody);
        components.push_back(AIMove);
        components.push_back(AITransform);

        EntitySystem::Entity* floorEntity = scene.CreateEntity();
        floorEntity->AddComponent(floorTransform);
        floorEntity->AddComponent(floorMesh);
        floorEntity->AddComponent(floorCollider);

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

        EntitySystem::Entity* AIBaseEntity = scene.CreateEntity();
        AIBaseEntity->AddComponent(AIBaseTransform);
        AIBaseEntity->AddComponent(AIBody);
        AIBaseEntity->AddComponent(AIMove);

        EntitySystem::Entity* AIEntity = scene.CreateEntity();
        AIEntity->AddComponent(AIRenderer);
        AIEntity->AddComponent(AITransform);

        AIBaseEntity->AddChild(AIEntity);

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

        loaded = true;
    }

    void Demo::AnimationDemo::Unload()
    {
        for (const auto& component : components)
        {
            delete component;
        }

        for (const auto& item : skinnedMeshes) {
            delete item.first;
        }

        for (const auto& item : meshes) {
            delete item.first;
        }

        TimeManager::Release();
        components.clear();
        skinnedMeshes.clear();
        meshes.clear();
        loaded = false;
    }
}


