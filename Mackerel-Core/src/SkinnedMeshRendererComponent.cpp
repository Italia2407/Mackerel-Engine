#include "LoggingSystem.h"

#include "SkinnedMeshRendererComponent.h"

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

// Skinned Mesh Data
#include "SkinnedMeshData.h"

namespace MCK::EntitySystem {

	SkinnedMeshRendererComponent::SkinnedMeshRendererComponent() :
		m_Mesh(nullptr), m_Material(nullptr), m_Shader(nullptr), m_EntityTransformComponent(nullptr),
		m_MeshEnum((MeshEnum)0), m_ShaderEnum((ShaderEnum)0), m_MaterialEnum((MaterialEnum)0) {}

	SkinnedMeshRendererComponent::SkinnedMeshRendererComponent(MeshEnum a_MeshEnum, ShaderEnum a_ShaderEnum, MaterialEnum a_MaterialEnum) :
		m_MeshEnum(a_MeshEnum), m_ShaderEnum(a_ShaderEnum), m_MaterialEnum(a_MaterialEnum),
		m_Mesh(nullptr), m_Shader(nullptr), m_Material(nullptr),
		m_EntityTransformComponent(nullptr) {}

	SkinnedMeshRendererComponent::SkinnedMeshRendererComponent(AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material) :
		m_Mesh(a_Mesh), m_Material(a_Material), m_Shader(a_Shader), m_EntityTransformComponent(nullptr),
		m_MeshEnum((MeshEnum)0), m_ShaderEnum((ShaderEnum)0), m_MaterialEnum((MaterialEnum)0) {}

	SkinnedMeshRendererComponent::~SkinnedMeshRendererComponent()
	{}

	void SkinnedMeshRendererComponent::CreateAndUploadJointTransforms()
	{
		localTransforms.resize(m_Mesh->NumberOfSoaJoints(), ozz::math::SoaTransform());
		modelTransforms.resize(m_Mesh->NumberOfModelJoints(), ozz::math::Float4x4());

		ltmJob.input = ozz::span<const ozz::math::SoaTransform>(localTransforms.data(), localTransforms.size());
		ltmJob.output = ozz::span<ozz::math::Float4x4>(modelTransforms.data(), modelTransforms.size());
		ltmJob.Run();

		glUniformMatrix4fv(jointTransformShaderLoc,
			static_cast<GLsizei>(modelTransforms.size()),
			GL_TRUE,
			reinterpret_cast<float*>(modelTransforms.data()));
	}

	void SkinnedMeshRendererComponent::OnCreate()
	{
		// Load Mesh Renderer Assets
		// MeshLibrary::LoadMesh(m_MeshEnum);
		ShaderLibrary::LoadShader(m_ShaderEnum);
		MaterialLibrary::LoadMaterial(m_MaterialEnum);

		MeshLibrary::GetMesh(m_MeshEnum, m_Mesh);
		ShaderLibrary::GetShader(m_ShaderEnum, m_Shader);
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

		// Create buffers needed for animation
		assert(m_Mesh->m_hasRig);
		jointTransformShaderLoc = m_Shader->GetShaderUniformLocation("joint_data");
		CreateAndUploadJointTransforms();
	}
	void SkinnedMeshRendererComponent::OnDestroy()
	{

	}
	void SkinnedMeshRendererComponent::OnUpdate()
	{
		// Queue the Data for the Rendering
		Rendering::Renderer::QueueMeshInstance(*m_EntityTransformComponent, m_Mesh, m_Shader, m_Material, false);
	}

	bool SkinnedMeshRendererComponent::Deserialise(json a_Data)
	{
		// Get Entity's Transform Component Data.
		a_Data = a_Data["data"];

		// Get Transform Component's Position
		int meshId = a_Data["meshID"];
		int shaderId = a_Data["shaderID"];
		int materialId = a_Data["materialID"];

		m_MeshEnum = static_cast<MeshEnum>(meshId);
		m_ShaderEnum = static_cast<ShaderEnum>(meshId);
		m_MaterialEnum = static_cast<MaterialEnum>(meshId);

		return true;
	}

	void SkinnedMeshRendererComponent::PlayAnimation(std::string animation, float time, bool interrupt, bool queue, bool loop)
	{
	}

	bool SkinnedMeshRendererComponent::SetAnimationPose(std::string animation, float time)
	{
		//if (!m_Mesh->m_hasRig)
		//	Logger::log(std::format("SetAnimationPose() was called with a mesh without a rig!"
		//		"Did you mistakenly assign a non-skinned mesh to a SkinnedMeshRenderer component?"),
		//		Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");

		assert(m_Mesh->m_animData->animations.contains(animation));

		ozz::animation::SamplingJob::Context smplContext(m_Mesh->m_animData->max_channels);

		smplJob.animation = &(m_Mesh->m_animData->animations[animation]);
		smplJob.ratio = fmod((time / smplJob.animation->duration()), 1.0f);
		smplJob.context = &smplContext;
		smplJob.output = ozz::span<ozz::math::SoaTransform>(localTransforms.data(), localTransforms.size());

		if (smplJob.Validate())
			smplJob.Run();
		else
			return false;

		ltmJob.input = ozz::span<const ozz::math::SoaTransform>(localTransforms.data(), localTransforms.size());
		ltmJob.output = ozz::span<ozz::math::Float4x4>(modelTransforms.data(), modelTransforms.size());
		ltmJob.Run();

		glUniformMatrix4fv(jointTransformShaderLoc,
			static_cast<GLsizei>(modelTransforms.size()),
			GL_TRUE,
			reinterpret_cast<float*>(modelTransforms.data()));

		return true;
	}
}
