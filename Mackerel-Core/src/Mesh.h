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
		const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& textureCoords, const std::vector<float>& tints,
		const std::vector<GLuint>& indices);

public:
	bool LoadFromFile(std::string a_FilePath);

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
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f
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
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f
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
};
}