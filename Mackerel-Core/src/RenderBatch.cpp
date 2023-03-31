#include "RenderBatch.h"

#include "UniformBuffer.h"

#include "Material.h"

#include "Mesh.h"
#include "Shader.h"

#include <iostream>

// Logging Headers
#include "LoggingSystem.h"
#include <format>

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
		Logger::log("Cannot Use Invalid Transform Uniform Buffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Bind Mesh's VAO to the GPU
	if (!m_Mesh->BindVertexArrayObject()) {
		Logger::log("Cannot Bind Mesh's VAO", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load Shader Program to GPU
	if (!m_Shader->UseProjectionProgram()) {
		Logger::log("Cannot Use Shader Program", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Render All Mesh Instances
	for (size_t i = 0; i < m_Instances.size(); i++)
	{	auto instance = m_Instances[i];

		// Load Instance's Transform Uniforms
		if (!a_TransformBuffer->SetMat4BufferUniform("transformMatrix", instance.transform)) {
			Logger::log(std::format("Cannot Set Instance #{} Transform Matrix in Uniform Buffer", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Load Instance's Material Uniforms
		if (!instance.material || !instance.material->UseMaterial()) {
			Logger::log(std::format("Cannot Load Instance #{} Material", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		}

		// Draw Mesh Instance
		glDrawElements(GL_TRIANGLES, m_Mesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
	}

	// Unbind Mesh's VAO from GPU
	glBindVertexArray(0);

	return true;
}
}
