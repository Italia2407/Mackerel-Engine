#include "LoggingSystem.h"

#include "RenderBatch.h"

#include "UniformBuffer.h"

#include "Material.h"

#include "Mesh.h"
#include "Shader.h"

#include <iostream>

#include "TransformComponent.h"

#include <format>

namespace MCK::Rendering {
RenderBatch::RenderBatch(AssetType::Mesh* mesh, AssetType::Shader* shader, bool isAnimated) :
	m_Mesh(mesh), m_Shader(shader), isAnimated(isAnimated) {}
RenderBatch::~RenderBatch()
{
	// Clear all Mesh Instances
	m_Instances.clear();
}

bool RenderBatch::AddBatchInstance(const EntitySystem::TransformComponent& a_Transform, AssetType::Material* a_Material)
{
	Instance batchInstance{}; {
		batchInstance.material = a_Material;
		batchInstance.transformMatrix = a_Transform.GetTransformationMatrix();
	}

	m_Instances.push_back(batchInstance);
	return true;
}

/**  */
bool RenderBatch::DrawBatchObjects(UniformBuffer* a_TransformBuffer, bool a_DepthOnly)
{
	// Ensure Mesh Uniform Buffer is Valid
	if (!a_TransformBuffer || !a_TransformBuffer->IsCreated()) {
		Logger::log("Cannot Use Invalid Transform Uniform Buffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Bind Mesh's VAO to the GPU
	if (!m_Mesh->BindVertexArrayObject(isAnimated)) {
		Logger::log("Cannot Bind Mesh's VAO", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load Shader Program to GPU
	if (!a_DepthOnly)
	{// Use Regular Shader Program
		if (!m_Shader->UseShaderProgram(isAnimated)) {
			Logger::log("Cannot Use Shader Program", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			return false;
		}
	}
	else
	{// Use Depth Only Shader Program
		if (!AssetType::Shader::k_DepthOnlyShader->UseShaderProgram(isAnimated)) {
			Logger::log("Cannot Use Depth Only Shader Program", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			return false;
		}
	}

	// Render All Mesh Instances
	for (size_t i = 0; i < m_Instances.size(); i++)
	{	auto instance = m_Instances[i];

		// Load Instance's Transform Uniforms
		if (!a_TransformBuffer->SetMat4BufferUniform("transformMatrix", instance.transformMatrix)) {
			Logger::log(std::format("Cannot Set Instance #{} Transform Matrix in Uniform Buffer", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		if (!a_DepthOnly) {
		// Load Instance's Material Uniforms
		if (!instance.material || !instance.material->UseMaterial()) {
			Logger::log(std::format("Cannot Load Instance #{} Material", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		}}

		// Draw Mesh Instance
		//glDrawArrays(GL_TRIANGLES, 0, m_Mesh->NumVertices() * 3);
		glDrawElements(GL_TRIANGLES, m_Mesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
	}

	// Unbind Mesh's VAO from GPU
	glBindVertexArray(0);

	return true;
}
}
