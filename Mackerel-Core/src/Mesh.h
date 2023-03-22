#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

#include <vector>
#include <array>

namespace MCK::AssetType {
class Mesh
{
public:
	Mesh();
	~Mesh();

private:
	// Mesh GPU Data
	GLuint m_NumVertices;
	GLuint m_NumIndices;

	GLuint m_VertexArrayObject;

	GLuint m_VertexBufferObjects[4]{};
	GLuint m_IndexBufferObject;

public:
	bool LoadFromFile(std::string fileName);

	bool BindVertexArrayObject();
	void DrawMeshInstance();

private:
	bool generateVertexObjects(
		std::vector<Eigen::Vector3f> positions, 
		std::vector<Eigen::Vector3f> normals, 
		std::vector<Eigen::Vector2f> textureCoords, 
		std::vector<Eigen::Vector3f> tints, 
		std::vector<GLuint> indices);
};
}