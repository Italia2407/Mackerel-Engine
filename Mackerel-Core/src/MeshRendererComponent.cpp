#include "LoggingSystem.h"

#include "MeshRendererComponent.h"

// System Headers
#include "Renderer.h"

#include <format>

// Asset Headers
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

#include "MeshLibrary.h"
#include "ShaderLibrary.h"
#include "MaterialLibrary.h"

// Component System Headers
#include "Entity.h"
#include "TransformComponent.h"


namespace MCK::EntitySystem {

MeshRendererComponent::MeshRendererComponent() :
	m_Mesh(nullptr), m_Material(nullptr), m_Shader(nullptr), m_EntityTransformComponent(nullptr),
	m_MeshEnum((MeshEnum)0), m_ShaderEnum((ShaderEnum)0), m_MaterialEnum((MaterialEnum)0) {}

MeshRendererComponent::MeshRendererComponent(MeshEnum a_MeshEnum, ShaderEnum a_ShaderEnum, MaterialEnum a_MaterialEnum) :
	m_MeshEnum(a_MeshEnum), m_ShaderEnum(a_ShaderEnum), m_MaterialEnum(a_MaterialEnum), 
	m_Mesh(nullptr), m_Shader(nullptr), m_Material(nullptr),
	m_EntityTransformComponent(nullptr) {}

MeshRendererComponent::MeshRendererComponent(AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material) :
	m_Mesh(a_Mesh), m_Material(a_Material), m_Shader(a_Shader), m_EntityTransformComponent(nullptr),
	m_MeshEnum((MeshEnum)0), m_ShaderEnum((ShaderEnum)0), m_MaterialEnum((MaterialEnum)0) {}

MeshRendererComponent::~MeshRendererComponent()
{}

void MeshRendererComponent::OnCreate()
{
	// Load Mesh Renderer Assets
	if (m_Mesh == nullptr)
		MeshLibrary::LoadMesh(m_MeshEnum);
	if (m_Shader == nullptr)
		ShaderLibrary::LoadShader(m_ShaderEnum);
	if (m_Material == nullptr)
		MaterialLibrary::LoadMaterial(m_MaterialEnum);

	if(m_Mesh == nullptr)
		MeshLibrary::GetMesh(m_MeshEnum, m_Mesh);
	if(m_Shader == nullptr)
		ShaderLibrary::GetShader(m_ShaderEnum, m_Shader);
	if(m_Material == nullptr)
		MaterialLibrary::GetMaterial(m_MaterialEnum, m_Material);

	uint32_t tempID = 42;
	std::string tempName = "Answer to the Universe";

	// Load Transform Component from Entity
	m_EntityTransformComponent = entity->GetComponent<TransformComponent>();

	// Check if Entity Transform Component is Valid
	if (!m_EntityTransformComponent)
	{
		Logger::log(std::format("Entity #{} ({}) Mesh Renderer Component Requires an Assigned Transform Component.", tempID, tempName),
			Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
	}
}
void MeshRendererComponent::OnDestroy()
{

}
void MeshRendererComponent::OnUpdate()
{
	// Ensure Entity has Assigned Transform Component
	if (!m_EntityTransformComponent)
	{
		return;
	}

	// Queue the Data for the Rendering
	Rendering::Renderer::QueueMeshInstance(*m_EntityTransformComponent, m_Mesh, m_Shader, m_Material, false);
}

bool MeshRendererComponent::Deserialise(json a_Data)
{
	// Get Entity's Transform Component Data.
	a_Data = a_Data["data"];

	for (auto itt = a_Data.begin(); itt != a_Data.end(); ++itt)
	{
		if (itt.key() == "meshID")
		{
			int meshId = a_Data["meshID"];
			m_MeshEnum = static_cast<MeshEnum>(meshId);
		}
		else if (itt.key() == "shaderID")
		{
			int shaderId = a_Data["shaderID"];
			m_ShaderEnum = static_cast<ShaderEnum>(shaderId);
		}
		else if (itt.key() == "materialID")
		{
			int materialId = a_Data["materialID"];
			m_MaterialEnum = static_cast<MaterialEnum>(materialId);
		}
		else if (itt.key() == "modelPath")
		{
			std::string path = a_Data["modelPath"];
			m_Mesh = new AssetType::Mesh(path);
			m_Mesh->LoadFromFile(path);
		}
		else if (itt.key() == "texturePath")
		{
			MCK::ShaderLibrary::LoadShader(ShaderEnum::textured, "../Mackerel-Core/res/Shaders/frag/textured.glsl");
			MCK::ShaderLibrary::GetShader(ShaderEnum::textured, m_Shader);
		}
		else if (itt.key() == "meshID")
		{

		}
	}

	return true;
}
}
