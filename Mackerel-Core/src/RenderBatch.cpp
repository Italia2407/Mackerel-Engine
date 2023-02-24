#include "RenderBatch.h"

#include "UniformBuffer.h"

#include "Material.h"

#include "Mesh.h"
#include "Shader.h"

namespace MCK::Rendering {
RenderBatch::RenderBatch(AssetType::Shader* shader, AssetType::Mesh* mesh) :
	_shader(shader), _mesh(mesh) {}
RenderBatch::~RenderBatch() {}

bool RenderBatch::AddBatchInstance(Instance instance)
{
	_instances.push_back(instance);
	return true;
}
}
