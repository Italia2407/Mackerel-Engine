#include "LoggingSystem.h"

#include "SkinnedMeshRendererComponent.h"

// System Headers
#include "Renderer.h"
#include "TimeManager.h"

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

// ozz animation library
#define OZZ_BULLET_COMPATIBILITY
#include <ozz/animation/runtime/animation.h>

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
		localTransforms.resize(m_Mesh->NumberOfSoaJoints(), ozz::math::SoaTransform::identity());
		modelTransforms.resize(m_Mesh->NumberOfModelJoints(), ozz::math::Float4x4::identity());

		ltmJob.skeleton = &m_Mesh->m_animData->skeleton;
		ltmJob.input = ozz::span<const ozz::math::SoaTransform>(localTransforms.data(), localTransforms.size());
		ltmJob.output = ozz::span<ozz::math::Float4x4>(modelTransforms.data(), modelTransforms.size());

		if (ltmJob.Validate())
			ltmJob.Run();
		else
		{
			Logger::log(std::format("Animation local-to-model job failed!"),
				Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		}

		m_Shader->UseShaderProgram(true);

		glUniformMatrix4fv(jointTransformShaderLoc,
			static_cast<GLsizei>(modelTransforms.size()),
			GL_FALSE,
			reinterpret_cast<float*>(modelTransforms.data()));

		glUniformMatrix4fv(invBindShaderLoc,
			static_cast<GLsizei>(m_Mesh->m_animData->inverseBindMatrices.size()),
			GL_FALSE,
			reinterpret_cast<float*>(m_Mesh->m_animData->inverseBindMatrices.data()));
	}

	void SkinnedMeshRendererComponent::DefaultPose(float time)
	{
		if (default_anim == nullptr)
		{
			for (uint32_t i = 0; i < localTransforms.size(); i++)
				localTransforms[i] =ozz::math::SoaTransform::identity();

			ltmJob.skeleton = &m_Mesh->m_animData->skeleton;
			ltmJob.input = ozz::span<const ozz::math::SoaTransform>(localTransforms.data(), localTransforms.size());
			ltmJob.output = ozz::span<ozz::math::Float4x4>(modelTransforms.data(), modelTransforms.size());

			if (ltmJob.Validate())
				ltmJob.Run();
			else
			{
				Logger::log(std::format("Animation local-to-model job failed!"),
					Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
			}

			glUniformMatrix4fv(jointTransformShaderLoc,
				static_cast<GLsizei>(modelTransforms.size()),
				GL_TRUE,
				reinterpret_cast<float*>(modelTransforms.data()));
		}
		else
		{
			bool finished = false;
			SetAnimationPose(default_anim, time, &finished, true);
		}
	}

	void SkinnedMeshRendererComponent::DoFrame()
	{
		/* set timer for next animation update */
		TimeManager::setScaledTimer(std::pair<double, std::function<void()>>(1.0f / target_fps, do_frame_callback));

		/* calculate run time of the current animation */
		double time = TimeManager::GetScaledUpTime();
		double progress = time - start_time;

		/* are we currently playing an animation? */
		if (animationQueue.empty())
		{
			DefaultPose();
		}
		else
		{
			bool is_finished = false;
			SetAnimationPose(animationQueue.front().animation, static_cast<float>(progress) + animationQueue.front().offset_time, &is_finished, animationQueue.front().loop);

			/* is this animation finished */
			if (is_finished)
			{
				if (animationQueue.size() > 1 || animationQueue.front().loop == false) /* if the last item in the list is set to loop, loop it */
				{
					/* animation is finished */
					animationQueue.pop_front();
					
					/* setup next animation */
					while (animationQueue.empty() == false)
					{
						/* is the next queued animation valid? */
						if (m_Mesh->m_animData->animations.contains(animationQueue.front().animation_name))
						{
							/* yes, set it up so it can be played next animation frame */
							animationQueue.front().animation = &m_Mesh->m_animData->animations[animationQueue.front().animation_name];
							break;
						}
						else
						{
							/* no, discard it */
							animationQueue.pop_front();
						}
					}
				}	
			}
		}
	}

	bool SkinnedMeshRendererComponent::SetAnimationPose(ozz::animation::Animation* animation, float time, bool* out_finished, bool loop)
	{
		//if (!m_Mesh->m_hasRig)
		//	Logger::log(std::format("SetAnimationPose() was called with a mesh without a rig!"
		//		"Did you mistakenly assign a non-skinned mesh to a SkinnedMeshRenderer component?"),
		//		Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");

		ozz::animation::SamplingJob::Context smplContext(m_Mesh->m_animData->max_channels);

		smplJob.animation = animation;

		float ratio = time / smplJob.animation->duration();
		if (ratio > 1.0f)
		{
			ratio = (loop) ? fmod(ratio, 1.0f) : 1.0f;
			*out_finished = true;
		}
		else
		{
			*out_finished = false;
		}

		smplJob.ratio = ratio;
		smplJob.context = &smplContext;
		smplJob.output = ozz::span<ozz::math::SoaTransform>(localTransforms.data(), localTransforms.size());

		if (smplJob.Validate())
			smplJob.Run();
		else
		{
			Logger::log(std::format("Animation sampling job failed!"),
				Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");

			*out_finished = false;
			return false;
		}

		ltmJob.skeleton = &m_Mesh->m_animData->skeleton;
		ltmJob.input = ozz::span<const ozz::math::SoaTransform>(localTransforms.data(), localTransforms.size());
		ltmJob.output = ozz::span<ozz::math::Float4x4>(modelTransforms.data(), modelTransforms.size());
		if (ltmJob.Validate())
			ltmJob.Run();
		else
		{
			Logger::log(std::format("Animation local-to-model job failed!"),
				Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");

			*out_finished = false;
			return false;
		}

		//printf("><><><>< %f\n", ratio);
		//auto fi = reinterpret_cast<float*>(modelTransforms.data());
		//for (uint32_t i = 0; i < 16 * modelTransforms.size(); i++)
		//{
		//	if (i % 4 == 0)
		//		printf("\n");

		//	if (i % 16 == 0)
		//		printf("\n");

		//	printf("%f ", fi[i]);
		//}
		//printf("\n=====================\n");

		m_Shader->UseShaderProgram(true);

		glUniformMatrix4fv(jointTransformShaderLoc,
			static_cast<GLsizei>(modelTransforms.size()),
			GL_FALSE,
			reinterpret_cast<float*>(modelTransforms.data()));

		return true;
	}

	void SkinnedMeshRendererComponent::OnCreate()
	{
		// Load Mesh Renderer Assets
		// MeshLibrary::LoadMesh(m_MeshEnum);
		// ShaderLibrary::LoadShader(m_ShaderEnum);
		// MaterialLibrary::LoadMaterial(m_MaterialEnum);

		// MeshLibrary::GetMesh(m_MeshEnum, m_Mesh);
		// ShaderLibrary::GetShader(m_ShaderEnum, m_Shader);
		// MaterialLibrary::GetMaterial(m_MaterialEnum, m_Material);

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

		/* Create buffers needed for animation */
		assert(m_Mesh->m_hasRig);
		jointTransformShaderLoc = m_Shader->GetShaderUniformLocation("joint_data", m_Mesh->m_hasRig);
		invBindShaderLoc = m_Shader->GetShaderUniformLocation("inv_bind", m_Mesh->m_hasRig);
		CreateAndUploadJointTransforms();

		/* create animation update callback */
		do_frame_callback = std::bind(&SkinnedMeshRendererComponent::DoFrame, this);

		/* start the animation loop */
		TimeManager::setScaledTimer(std::pair<double, std::function<void()>>(1.0f / target_fps, do_frame_callback));
	}
	void SkinnedMeshRendererComponent::OnDestroy()
	{

	}
	void SkinnedMeshRendererComponent::OnUpdate()
	{
		// Queue the Data for the Rendering
		Rendering::Renderer::QueueMeshInstance(*m_EntityTransformComponent, m_Mesh, m_Shader, m_Material, false, true);
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
		if (interrupt == true)
			/* if we're interrupting, clear the queue. this guarantees it will be played next animation frame */
			animationQueue.clear();
		else if (animationQueue.empty() == false && queue == false)
			/* if there are queued animations and the animation is set to not be enqueued, discard it */
			return;

		/* add the new animation to the queue (it will be the only animation in the queue if interrupt is true) */
		assert(m_Mesh->m_animData->animations.contains(animation));

		animationQueue.push_back({});
		animationQueue.back().animation_name = animation;
		animationQueue.back().animation = &m_Mesh->m_animData->animations[animation];
		animationQueue.back().offset_time = time;
		animationQueue.back().loop = loop;
	}
	bool SkinnedMeshRendererComponent::SetDefaultAnimation(std::string animation)
	{
		if (m_Mesh->m_animData->animations.contains(animation))
		{
			default_anim = &m_Mesh->m_animData->animations[animation];
			return true;
		}
		else
			return false;
	}
	void SkinnedMeshRendererComponent::SetTargetFPS(float fps)
	{
		target_fps = fps;
	}
}
