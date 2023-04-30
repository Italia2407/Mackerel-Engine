#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

#include <vector>
#include <array>
#include <string>

namespace MCK::AssetType {
class Mesh
{
public:
	Mesh(std::string a_Name);
	~Mesh();

	GLuint NumVertices() const { return m_NumVertices; }
	const GLuint& NumIndices() const { return m_NumIndices; }

private:
	std::string m_Name;

	// Mesh Properties
	GLuint m_NumVertices;
	GLuint m_NumIndices;

	// Mesh GPU Data
	GLuint m_VertexArrayObject;

	std::array<GLuint, 4> m_VertexBufferObjects;
	GLuint m_IndexBufferObject;

	bool generateVertexObjects(
		const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& textureCoords, const std::vector<float>& tints, const std::vector<GLuint>& a_Indices);

public:
	bool LoadFromFile(std::string a_FilePath);

	bool BindVertexArrayObject();
};
}