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
		AssetType::Material* material;
		Eigen::Matrix4f transform;
	};

private:
	AssetType::Mesh* m_Mesh;
	AssetType::Shader* m_Shader;

	UniformBuffer* m_TransformsBuffer;
	std::vector<Instance> m_Instances;

public:
	bool AddBatchInstance(AssetType::Material* a_Material, Eigen::Matrix4f a_Transform);

	bool DrawBatchObjects();
};
}