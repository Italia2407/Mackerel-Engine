#include "MeshRendererComponent.h"

// System Headers
#include "Renderer.h"

// Logging Headers
#include "LoggingSystem.h"
#include <format>

// Asset Headers
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

#include "MeshLibrary.h"
#include "ShaderLibrary.h"
#include "MaterialLibrary.h"


namespace MCK::EntitySystem {
MeshRendererComponent::MeshRendererComponent(MeshEnum a_MeshEnum, ShaderEnum a_ShaderEnum, MaterialEnum a_MaterialEnum) :
	m_MeshEnum(a_MeshEnum), m_ShaderEnum(a_ShaderEnum), m_MaterialEnum(a_MaterialEnum), m_Mesh(nullptr), m_Shader(nullptr), m_Material(nullptr) {}
MeshRendererComponent::~MeshRendererComponent()
{}

TypeInfoRef MeshRendererComponent::GetType()
{
	return typeid(MeshRendererComponent);
}

void MeshRendererComponent::OnCreate()
{
	// Load Mesh Renderer Assets
	MeshLibrary::GetMesh(m_MeshEnum, m_Mesh);
	ShaderLibrary::GetShader(m_ShaderEnum, m_Shader);
	MaterialLibrary::GetMaterial(m_MaterialEnum, m_Material);

	uint32_t tempID = 42;
	std::string tempName = "Answer to the Universe";
	// TODO: Load Transform Component from Entity

	// TODO: Check if Entity Transform Component is Valid
	if (true)
	{
		auto errorMsg = std::format("Entity #{} ({}) Mesh Renderer Component Requires an Assigned Transform Component.", tempID, tempName);
	}
}
void MeshRendererComponent::OnDestroy()
{

}
void MeshRendererComponent::OnUpdate()
{
	Eigen::Matrix4f tempTransform = Eigen::Matrix4f::Identity();

	// Queue the Data for the Rendering
	Rendering::Renderer::QueueMeshInstance(tempTransform, m_Mesh, m_Shader, m_Material, false);
}

bool MeshRendererComponent::Deserialise(json a_Data)
{
	return false;
}
}
