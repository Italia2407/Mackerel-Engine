#include "RenderBatch.h"

#include "UniformBuffer.h"

#include "Material.h"

#include "Mesh.h"
#include "Shader.h"

#include <iostream>

namespace MCK::Rendering {
RenderBatch::RenderBatch(AssetType::Mesh* mesh, AssetType::Shader* shader) :
	m_Mesh(mesh), m_Shader(shader) {}
RenderBatch::~RenderBatch()
{
	// Clear all Mesh Instances
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
bool RenderBatch::DrawBatchObjects(UniformBuffer* a_TransformBuffer)
{
	// Ensure Mesh Uniform Buffer is Valid
	if (!a_TransformBuffer || !a_TransformBuffer->IsCreated()) {
		std::cout << "ERROR: Cannot Use Invalid Transform Uniform Buffer" << std::endl;
		return false;
	}

	// Bind Mesh's VAO to the GPU
	if (!m_Mesh->BindVertexArrayObject()) {
		std::cout << "ERROR: Cannot Bind Mesh's VAO" << std::endl;
		return false;
	}

	// Render All Mesh Instances
	for (size_t i = 0; i < m_Instances.size(); i++)
	{	auto instance = m_Instances[i];

		// Load Instance's Transform Uniforms
		if (!a_TransformBuffer->SetMat4BufferUniform("transform", instance.transform)) {
			std::cout << "ERROR: Cannot Set Instance #"  << i << " Transform Uniform in Transform Uniform Buffer" << std::endl;
			continue;
		}

		// Load Instance's Material Uniforms
		if (!instance.material || !instance.material->UseMaterial()) {
			std::cout << "ERROR: Cannot Load Instance #" << i << " Material" << std::endl;
		}

		// Draw Mesh Instance
		glDrawElements(GL_TRIANGLES, m_Mesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
	}

	// Unbind Mesh's VAO from GPU
	glBindVertexArray(0);

	return true;
}
}
