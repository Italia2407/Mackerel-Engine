#pragma once

#include <Eigen/Eigen.h>
#include <InputComponent.h>

#include "CameraComponent.h"

#include "AIMovement.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Scene.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "ShaderLibrary.h"
#include "PerspectiveCamera.h"
#include "CameraFollowComponent.h"
#include "Renderer.h"
#include "MeshRendererComponent.h"
#include "SkinnedMeshRendererComponent.h"
#include "TimeManager.h"
#include "PhysicsWorld.h"
#include "PhysicsHelpers.h"
#include "InputComponent.h"
#include "ExamplePlayerController.h"
#include "AudioEmitter.h"
#include "UIComponent.h"

#include "Light.h"
#include "ComponentRegistrar.h"

namespace MCK
{
    namespace Demo
    {
        class RenderingDemo
        {
        public:
            RenderingDemo();
            ~RenderingDemo();

            void AddEntities(EntitySystem::Scene& scene);
            void Init();

        private:
            std::unordered_map<std::string, AssetType::Mesh*> meshMap;
            std::unordered_map<std::string, AssetType::Material*> materialMap;
            std::unordered_map<std::string, AssetType::Shader*> shaderMap;

            EntitySystem::TransformComponent* floorTransform;
            EntitySystem::MeshRendererComponent* floorMesh;
            Physics::CollisionComponent* floorCollider;

            EntitySystem::TransformComponent* deathFloorTransform;
            EntitySystem::MeshRendererComponent* deathFloorMesh;
            Physics::CollisionComponent* deathFloorCollider;

            EntitySystem::PerspectiveCamera* cameraComponent;
            EntitySystem::CameraFollowComponent* cameraFollowComponent;

            EntitySystem::TransformComponent* playerBaseTransform;
            Physics::RigidbodyComponent* playerBody;
            EntitySystem::SkinnedMeshRendererComponent* playerRenderer;
            EntitySystem::InputComponent* playerInput;
            ExamplePlayer::ExamplePlayerController* playerController;

            EntitySystem::TransformComponent* playerTransform;

            std::vector<EntitySystem::TransformComponent*> transforms;
            std::unordered_map<EntitySystem::SkinnedMeshRendererComponent*, int> skinnedMeshes;
            std::unordered_map<EntitySystem::MeshRendererComponent*, int> meshes;

            EntitySystem::TransformComponent* AIBaseTransform;
            EntitySystem::SkinnedMeshRendererComponent* AIRenderer;
            Physics::RigidbodyComponent* AIBody;
            EntitySystem::AIMovement* AIMove;
            EntitySystem::TransformComponent* AITransform;

            EntitySystem::TransformComponent* audioTransform;
            EntitySystem::AudioEmitter* audioComponent;

            EntitySystem::UIComponent* uiComponent;
        };
    }
}
