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

private:
	struct Instance
	{
		AssetType::Material* material;

		Eigen::Vector3f position;
		Eigen::Quaternion<float> rotation;
		Eigen::Vector3f scale;
	};

private:
	AssetType::Mesh* _mesh;
	AssetType::Shader* _shader;

	UniformBuffer* _meshTransformBuffer;
	std::vector<Instance> _instances;

public:
	bool AddBatchInstance(AssetType::Material* material, Eigen::Vector3f position, Eigen::Quaternion<float> rotation, Eigen::Vector3f scale);

	bool LoadMeshShader();
	void DrawBatchObjects();
};
}