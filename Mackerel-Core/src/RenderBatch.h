#pragma once

#include <vector>
#include <Eigen/Eigen.h>

// Forward Declarations
namespace MCK {
class UniformBuffer;
}
namespace MCK::AssetType {
class Material;

class Mesh;
class Shader;
}
namespace MCK::EntitySystem {
class TransformComponent;
class SkinnedMeshRendererComponent;
}

namespace MCK::Rendering {
class RenderBatch
{
public:
	RenderBatch(AssetType::Mesh* mesh, AssetType::Shader* shader, bool isAnimated = false);
	~RenderBatch();

	AssetType::Mesh* Mesh() const { return m_Mesh; }
	AssetType::Shader* Shader() const { return m_Shader; }
	inline bool IsAnimated() const { return isAnimated; }

private:
	struct Instance
	{
		Eigen::Matrix4f transformMatrix;
		AssetType::Material* material;
		MCK::EntitySystem::SkinnedMeshRendererComponent* pSkinnedMeshRenderer = nullptr;
	};

private:
	AssetType::Mesh* m_Mesh;
	AssetType::Shader* m_Shader;
	bool isAnimated = false;

	std::vector<Instance> m_Instances;

public:
	bool AddBatchInstance(const EntitySystem::TransformComponent& a_Transform, AssetType::Material* a_Material, MCK::EntitySystem::SkinnedMeshRendererComponent* a_pSkinnedMeshRenderer = nullptr);

	bool DrawBatchObjects(UniformBuffer* a_TransformBuffer, bool a_DepthOnly);
};
}