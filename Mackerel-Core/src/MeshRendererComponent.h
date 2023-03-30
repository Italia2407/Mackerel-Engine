#pragma once
#include "Component.h"

#include "MeshEnum.h"
#include "ShaderEnum.h"
#include "MaterialEnum.h"

namespace MCK::AssetType {
class Mesh;
class Shader;
class Material;
}

namespace MCK::EntitySystem {
class MeshRendererComponent : public Component
{
public:
	MeshRendererComponent(MeshEnum a_MeshEnum, ShaderEnum a_ShaderEnum, MaterialEnum a_MaterialEnum);
	~MeshRendererComponent();

private:
	// TODO: Reference to Entity Transform Component

	// Asset References
	MeshEnum m_MeshEnum;
	ShaderEnum m_ShaderEnum;
	MaterialEnum m_MaterialEnum;

	AssetType::Mesh* m_Mesh;
	AssetType::Shader* m_Shader;
	AssetType::Material* m_Material;

public:
	TypeInfoRef GetType() override;

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate() override;

	bool Deserialise(json a_Data) override;

};
}