#pragma once

#include <Eigen/Eigen.h>
#include <InputComponent.h>

#include "CameraComponent.h"

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
            AssetType::Mesh* cubeMesh;
            AssetType::Material* greyMaterial;
            AssetType::Material* blueMaterial;
            AssetType::Shader* m_UnlitShader;
            AssetType::Shader* m_MonoColourShader;

            EntitySystem::TransformComponent floorTransform;
            EntitySystem::MeshRendererComponent* floorMesh;
            Physics::CollisionComponent* floorCollider;

            EntitySystem::PerspectiveCamera* cameraComponent;
            EntitySystem::CameraFollowComponent* cameraFollowComponent;

            EntitySystem::TransformComponent* playerTransform;
            Physics::RigidbodyComponent* playerBody;
            EntitySystem::MeshRendererComponent* playerRenderer;
            EntitySystem::InputComponent* playerInput;
            ExamplePlayer::ExamplePlayerController* playerController;

            EntitySystem::TransformComponent* audioTransform;
            EntitySystem::AudioEmitter* audioComponent;

            EntitySystem::UIComponent* uiComponent;

        };
    }
}
