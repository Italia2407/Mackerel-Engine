#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

#include <vector>
#include <Eigen/Eigen.h>

#include "UniformBuffer.h"

namespace MCK::Rendering {
class RenderBatch
{
public:
	RenderBatch(AssetType::Shader* shader, AssetType::Mesh* mesh);
	~RenderBatch();

	struct Instance
	{
		Eigen::Vector3f position;
		Eigen::Quaternion<float> rotation;
		Eigen::Vector3f scale;

		AssetType::Material* material;
	};

private:
	AssetType::Shader* _shader;
	AssetType::Mesh* _mesh;

	UniformBuffer* _meshTransformBuffer;
	std::vector<Instance> _instances;

public:
	bool AddBatchInstance(Instance instance);

	bool LoadShaderMesh();
	void DrawBatchObjects();
};
}