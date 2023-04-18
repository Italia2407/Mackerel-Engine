#pragma once

#include <Eigen/Dense.h>
#include <cstdint>

#include <App.h>
#include <Assets.h>
#include <AudioEngine.h>
#include <Entity.h>
#include <Input.h>
#include <PerspectiveCamera.h>
#include <Renderer.h>
#include <Scene.h>

#include <TransformComponent.h>
#include <MeshRendererComponent.h>
#include <TestComponent.h>
#include <RigidbodyComponent.h>
#include <CollisionComponent.h>
#include <InputComponent.h>

class DemoApp : public MCK::App
{
	public:
		DemoApp();
		~DemoApp();

		DemoApp(const DemoApp&) = delete;
		DemoApp& operator=(const DemoApp&) = delete;

	private:
		Eigen::Vector2f cubeInput;
        void MovingCubeCallback(int32_t key, MCK::ButtonEvents ButtonEvents);

		MCK::EntitySystem::Scene scene;
		MCK::EntitySystem::Entity* entity = nullptr;
		MCK::EntitySystem::Entity* cameraEntity = nullptr;
		MCK::EntitySystem::Entity* floorEntity = nullptr;
		MCK::EntitySystem::TransformComponent transformComp;
		MCK::EntitySystem::TestComponent testComp;
		MCK::Physics::RigidbodyComponent rigidComp;
		MCK::EntitySystem::TransformComponent floorTransform;
		MCK::Physics::CollisionComponent collisionComp;
		MCK::EntitySystem::InputComponent inputComp;  

		MCK::Input::InputSubReceipt receipt;

		MCK::AssetType::Mesh* testMesh = nullptr;
		MCK::AssetType::Material* testMaterial = nullptr;

		MCK::AssetType::Shader* testShader = nullptr;
		MCK::AssetType::Shader* lightShader = nullptr;

		MCK::EntitySystem::MeshRendererComponent* meshRenderer = nullptr;
		MCK::EntitySystem::MeshRendererComponent* floorRenderer = nullptr;

		MCK::EntitySystem::PerspectiveCamera* cameraComponent;

	public:
		void Start() override;
		void Update() override;
		void End() override;
};