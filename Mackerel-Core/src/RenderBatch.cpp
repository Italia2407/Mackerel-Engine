#include "RenderBatch.h"

namespace MCK::Rendering {
RenderBatch::RenderBatch(AssetType::Shader* shader, AssetType::Mesh* mesh) :
	_shader(shader), _mesh(mesh) {}
RenderBatch::~RenderBatch() {}

bool RenderBatch::AddBatchInstance(Instance instance)
{
	if (instance.material->shader() != _shader)
	{// Batch Shader isn't same as Material Shader
		return false;
	}

	_instances.push_back(instance);
	return true;
}
}
