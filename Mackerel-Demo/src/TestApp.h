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

class TestApp : public MCK::App
{
public:
	TestApp();
	~TestApp();

	TestApp(const TestApp&) = delete;
	TestApp& operator=(const TestApp&) = delete;

private:
	Eigen::Vector3f m_CameraInput;
	void MoveCameraCallback(int32_t key, MCK::ButtonEvents ButtonEvents);

	MCK::EntitySystem::CameraComponent* m_Camera;

	MCK::EntitySystem::Entity* m_MeshEntity;

	MCK::AssetType::Mesh* m_Mesh;
	MCK::EntitySystem::TransformComponent m_MeshTransform;
	MCK::AssetType::Material* m_Material;

	MCK::AssetType::Shader* m_UnlitShader;
	MCK::AssetType::Shader* m_MonoColourShader;

public:
	void Start() override;
	void Update() override;
	void End() override;
};
