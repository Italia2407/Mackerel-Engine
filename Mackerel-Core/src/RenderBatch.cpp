#include "RenderBatch.h"

#include "UniformBuffer.h"

#include "Material.h"

#include "Mesh.h"
#include "Shader.h"

namespace MCK::Rendering {
RenderBatch::RenderBatch(AssetType::Mesh* mesh, AssetType::Shader* shader) :
	_mesh(mesh), _shader(shader) {}
RenderBatch::~RenderBatch()
{
	// Clear Transform Buffer Object
	delete _meshTransformBuffer;

	// Empty all Instances
	_instances.clear();
}

bool RenderBatch::AddBatchInstance(AssetType::Material* material, Eigen::Vector3f position, Eigen::Quaternion<float> rotation, Eigen::Vector3f scale)
{
	Instance batchInstance{}; {
		batchInstance.material = material;

		batchInstance.position = position;
		batchInstance.rotation = rotation;
		batchInstance.scale = scale;
	}

	_instances.push_back(batchInstance);
	return true;
}
}
