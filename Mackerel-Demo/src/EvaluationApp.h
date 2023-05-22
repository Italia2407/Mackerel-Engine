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

using namespace MCK;

class EvaluationApp : public MCK::App
{
public:
	EvaluationApp();
	~EvaluationApp();

	EvaluationApp(const EvaluationApp&) = delete;
	EvaluationApp& operator=(const EvaluationApp&) = delete;

private:
	AssetType::Mesh* cubeMesh;
	AssetType::Mesh* sphereMesh;
	AssetType::Mesh* boneMesh;
	AssetType::Material* greyMaterial;
	AssetType::Material* blueMaterial;
	AssetType::Material* yellowMaterial;

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

	//bool loadedDemo;
	int rigidbodyCount = 2000;
	int objectCount = 0;
	int lightCount = 1;
	int triangleCount = 0;
	double maxFPS = 0;
	double minFPS = 1000;
	double now = 0;
	double before = 0;
	double max = 0;
	int entitiesCount = 0;
	int loopsCount = 0;
	int animCount = 0;

	Rendering::DirectionLight* light;

public:
	void Start() override;
	void Update() override;
	void End() override;

	std::string GetCount();
	std::string GetCurrentFPS();

};