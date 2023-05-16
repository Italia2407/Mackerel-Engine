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

#include "RenderingDemo.h"
#include "PhysicsDemo.h"
#include "AnimationDemo.h"
#include "AudioDemo.h"

using namespace MCK;

class FinalDemoApp : public MCK::App
{
public:
	FinalDemoApp();
	~FinalDemoApp();

	FinalDemoApp(const FinalDemoApp&) = delete;
	FinalDemoApp& operator=(const FinalDemoApp&) = delete;

private:
	// Demos
	Demo::RenderingDemo renderingDemo;
	Demo::PhysicsDemo physicsDemo;
	Demo::AnimationDemo animationDemo;
	Demo::AudioDemo audioDemo;

	// Scene
	EntitySystem::Scene scene; 
	EntitySystem::Scene constantScene;

	bool loadedDemo;

	// Rendering
	Rendering::DirectionLight* light;

public:
	void Start() override;
	void Update() override;
	void End() override;

	std::string GetCurrentRuntime();

	void LoadRenderingDemo();
	void LoadPhysicsDemo();
	void LoadAnimationDemo();
	void LoadAudioDemo();
};