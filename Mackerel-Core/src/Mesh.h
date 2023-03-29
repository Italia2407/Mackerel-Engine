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
	// Mesh Properties
	GLuint m_NumVertices;
	GLuint m_NumIndices;

	// Mesh GPU Data
	GLuint m_VertexArrayObject;

	GLuint m_VertexBufferObjects[4]{};
	GLuint m_IndexBufferObject;

public:
	const GLuint& NumIndices() const { return m_NumIndices; }

public:
	bool LoadFromFile(std::string fileName);

	bool BindVertexArrayObject();

	// Default Mesh Shapes
	void LoadDisplayMesh()
	{
		std::vector<float> positions{
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f
		};
		std::vector<float> normals{
			0.0f, 0.0f, 1.0f, // Should Point Towards Camera
			0.0f, 0.0f, 1.0f, // Should Point Towards Camera
			0.0f, 0.0f, 1.0f, // Should Point Towards Camera
			0.0f, 0.0f, 1.0f  // Should Point Towards Camera
		};
		std::vector<float> textureCoords{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};
		std::vector<float> tints{
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};

		std::vector<GLuint> indices{
			0, 1, 2,
			0, 2, 3
		};

		// Load Mesh Properties
		m_NumVertices = (GLuint)positions.size() / 3;
		m_NumIndices = (GLuint)indices.size();

		// Generate the Mesh GPU Data
		generateVertexObjects(positions, normals, textureCoords, tints, indices);
	}
	void LoadSmallDisplayMesh()
	{
		std::vector<float> positions{
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::vector<float> normals{
			0.0f, 0.0f, 1.0f, // Should Point Towards Camera
			0.0f, 0.0f, 1.0f, // Should Point Towards Camera
			0.0f, 0.0f, 1.0f, // Should Point Towards Camera
			0.0f, 0.0f, 1.0f  // Should Point Towards Camera
		};
		std::vector<float> textureCoords{
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,
			0.0f, 1.0f
		};
		std::vector<float> tints{
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};

		std::vector<GLuint> indices{
			0, 1, 2,
			0, 2, 3
		};

		// Load Mesh Properties
		m_NumVertices = (GLuint)positions.size() / 3;
		m_NumIndices = (GLuint)indices.size();

		// Generate the Mesh GPU Data
		generateVertexObjects(positions, normals, textureCoords, tints, indices);
	}

private:
	bool generateVertexObjects(
		std::vector<float> positions,
		std::vector<float> normals,
		std::vector<float> textureCoords,
		std::vector<float> tints,
		std::vector<GLuint> indices);
};
}