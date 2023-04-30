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
}

namespace MCK::Rendering {
class RenderBatch
{
public:
	RenderBatch(AssetType::Mesh* mesh, AssetType::Shader* shader);
	~RenderBatch();

	AssetType::Mesh* Mesh() const { return m_Mesh; }
	AssetType::Shader* Shader() const { return m_Shader; }

private:
	struct Instance
	{
		Eigen::Matrix4f transformMatrix;
		AssetType::Material* material;
	};

private:
	AssetType::Mesh* m_Mesh;
	AssetType::Shader* m_Shader;

	std::vector<Instance> m_Instances;

public:
	bool AddBatchInstance(const EntitySystem::TransformComponent& a_Transform, AssetType::Material* a_Material);

	bool DrawBatchObjects(UniformBuffer* a_TransformBuffer, bool a_DepthOnly);
};
}