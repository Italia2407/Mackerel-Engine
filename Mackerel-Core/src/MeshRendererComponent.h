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
class TransformComponent;
}

namespace MCK::EntitySystem {
class MeshRendererComponent : public Component<MeshRendererComponent>
{
public:
	MeshRendererComponent();
	MeshRendererComponent(MeshEnum a_MeshEnum, ShaderEnum a_ShaderEnum, MaterialEnum a_MaterialEnum);
	MeshRendererComponent(AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material);
	~MeshRendererComponent();

private:
	// Reference to Entity Transform Component
	TransformComponent* m_EntityTransformComponent;

	// Asset References
	MeshEnum m_MeshEnum;
	ShaderEnum m_ShaderEnum;
	MaterialEnum m_MaterialEnum;

	AssetType::Mesh* m_Mesh;
	AssetType::Shader* m_Shader;
	AssetType::Material* m_Material;

public:

	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate() override;

	bool Deserialise(json a_Data) override;

};
}