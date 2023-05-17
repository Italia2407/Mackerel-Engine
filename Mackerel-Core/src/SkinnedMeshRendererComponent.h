#pragma once
#include "Component.h"

#include "MeshEnum.h"
#include "ShaderEnum.h"
#include "MaterialEnum.h"

// ozz animation library
#include <ozz/animation/runtime/animation.h>
#include <ozz/base/maths/soa_float4x4.h>
#include <ozz/base/maths/soa_transform.h>

namespace MCK::AssetType {
	class Mesh;
	class Shader;
	class Material;
}

namespace MCK::EntitySystem {
	class TransformComponent;
}

const static uint32_t MAX_JOINTS = 256; /* this needs to match the size of the joint_data array in vertex shaders that utilise animated meshes! */

namespace MCK::EntitySystem {
	class SkinnedMeshRendererComponent : public Component<SkinnedMeshRendererComponent>
	{
	public:
		SkinnedMeshRendererComponent();
		SkinnedMeshRendererComponent(MeshEnum a_MeshEnum, ShaderEnum a_ShaderEnum, MaterialEnum a_MaterialEnum);
		SkinnedMeshRendererComponent(AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material);
		~SkinnedMeshRendererComponent();


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

		// Data needed for animation
		std::vector<ozz::math::SoaTransform> localTransforms = {};
		std::vector<ozz::math::Float4x4> modelTransforms = {};
		GLint jointTransformShaderLoc{};

		ozz::animation::SamplingJob smplJob;
		ozz::animation::LocalToModelJob ltmJob;

		void CreateAndUploadJointTransforms();

	public:

		void OnCreate() override;
		void OnDestroy() override;
		void OnUpdate() override;

		bool Deserialise(json a_Data) override;

		void PlayAnimation(std::string animation, float time = 0.0f, bool interrupt = true, bool queue = false, bool loop = false);
		bool SetAnimationPose(std::string animation, float time);
	};
}