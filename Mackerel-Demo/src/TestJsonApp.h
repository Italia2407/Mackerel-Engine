#pragma once
#include "App.h"

#include <Eigen/Eigen.h>
#include <InputComponent.h>

#include "CameraComponent.h"

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"

#include "Entity.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "Light.h"

class TestJsonApp : public MCK::App
{
public:
	TestJsonApp();
	~TestJsonApp();

	TestJsonApp(const TestJsonApp&) = delete;
	TestJsonApp& operator=(const TestJsonApp&) = delete;

private:
	MCK::EntitySystem::Scene scene;

public:
	void Start() override;
	void Update() override;
	void End() override;
};
