#include "RenderBatch.h"

#include "UniformBuffer.h"

#include "Material.h"

#include "Mesh.h"
#include "Shader.h"

namespace MCK::Rendering {
RenderBatch::RenderBatch(AssetType::Mesh* mesh, AssetType::Shader* shader) :
	m_Mesh(mesh), m_Shader(shader)
{
	m_TransformsBuffer = new UniformBuffer();
}
RenderBatch::~RenderBatch()
{
	// Clear Transform Buffer Object
	if (m_TransformsBuffer)
		delete m_TransformsBuffer;

	// Empty all Instances
	m_Instances.clear();
}

bool RenderBatch::AddBatchInstance(AssetType::Material* a_Material, Eigen::Matrix4f a_Transform)
{
	Instance batchInstance{}; {
		batchInstance.material = a_Material;
		batchInstance.transform = a_Transform;
	}

	m_Instances.push_back(batchInstance);
	return true;
}

/**  */
bool RenderBatch::DrawBatchObjects()
{
	// Bind Mesh VAO to GPU
	if (!m_Mesh->BindVertexArrayObject())
	{
		return false;
	}

	// Create Mesh Transform Uniform Buffer Object
	m_TransformsBuffer->AddMat4BufferUniform("transform", Eigen::Matrix4f::Identity());
	if (!m_TransformsBuffer->CreateUniformBufferObject())
	{// Mesh Transform Uniform Buffer Object could not be Created
		return false;
	}

	// Bind Mesh Transform UBO to Slot 0
	m_TransformsBuffer->BindUniformBufferObject(0);

	// Render All Mesh Instances
	for (auto instance : m_Instances)
	{
		// Load Material Uniforms
		instance.material->UseMaterial();

		// Load Transform Uniforms
		m_TransformsBuffer->SetMat4BufferUniform("transform", instance.transform);

		m_Mesh->DrawMeshInstance();
	}

	// Unbind Mesh VAO from GPU
	glBindVertexArray(0);

	return true;
}
}
