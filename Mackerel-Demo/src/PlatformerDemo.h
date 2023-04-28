#pragma once
#include "App.h"


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

using namespace MCK;

class PlatformerApp : public MCK::App
{
public:
	PlatformerApp();
	~PlatformerApp();

	PlatformerApp(const PlatformerApp&) = delete;
	PlatformerApp& operator=(const PlatformerApp&) = delete;

private:
	AssetType::Mesh* cubeMesh;
	AssetType::Material* greyMaterial;
	AssetType::Material* blueMaterial;

	AssetType::Shader* m_UnlitShader;
	AssetType::Shader* m_MonoColourShader;

	// Components
	// Floor
	EntitySystem::TransformComponent floorTransform;
	EntitySystem::MeshRendererComponent* floorMesh;

	// Camera
	EntitySystem::PerspectiveCamera* cameraComponent;
	
	// Scene
	EntitySystem::Scene scene;

	Rendering::DirectionLight* light;

public:
	void Start() override;
	void Update() override;
	void End() override;

	void ButtonCallbackTestFunction();
};