#include "Mesh.h"

#include <iostream>

namespace MCK::AssetType {
Mesh::Mesh() :
	m_NumVertices(0), m_NumIndices(0), m_VertexArrayObject(GL_ZERO), m_IndexBufferObject(GL_ZERO) {}
Mesh::~Mesh()
{
	// Delete Vertex Array Object
	glDeleteBuffers(1, &m_VertexArrayObject);

	// Delete Vertex Buffer Objects
	glDeleteBuffers(4, m_VertexBufferObjects);

	// Delete Vertex Index Object
	glDeleteBuffers(1, &m_IndexBufferObject);
}

bool Mesh::LoadFromFile(std::string fileName)
{
	return false;
}

/**
 * Binds the Mesh's VAO to the GPU.
 * 
 * \return Whether the VAO could be Bounded
 */
bool Mesh::BindVertexArrayObject()
{
	// Bind the Vertex Array Object
	if (m_VertexArrayObject == GL_ZERO)
	{
		std::cout << "ERROR: Vertex Array Object does not Exist" << std::endl;

		return false;
	}
	glBindVertexArray(m_VertexArrayObject);

	// Bind the Index Buffer Object
	if (m_IndexBufferObject == GL_ZERO)
	{
		std::cout << "ERROR: Index Buffer Object does not Exist" << std::endl;

		return false;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject);

	return true;
}

/**  */
bool Mesh::generateVertexObjects(
	std::vector<float> positions,
	std::vector<float> normals,
	std::vector<float> textureCoords,
	std::vector<float> tints,
	std::vector<GLuint> indices)
{
	// Check that VAO does not Already Exist
	if (m_VertexArrayObject != GL_ZERO)
	{
		std::cout << "ERROR: Cannot Recreate Vertex Array Object" << std::endl;

		return false;
	}
	// Check that VBOs do not Already Exist
	for (auto vertexBufferObject : m_VertexBufferObjects) {
	if (vertexBufferObject != GL_ZERO)
	{
		std::cout << "ERROR: Cannot Recreate Vertex Buffer Objects" << std::endl;

		return false;
	}}
	// Check that IBO does not Already Exist
	if (m_IndexBufferObject != GL_ZERO)
	{
		std::cout << "ERROR: Cannot Recreate Index Buffer Object" << std::endl;

		return false;
	}

	// Create & Bind VAO
	glGenVertexArrays(1, &m_VertexArrayObject);
	glBindVertexArray(m_VertexArrayObject);

	// Create VBOs
	glCreateBuffers(4, m_VertexBufferObjects);

	// Create & Bind Positions VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

	// Bind Normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(float) * 3), nullptr);

	// Bind Texture Coords VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[2]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords.size(), &textureCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(float) * 2), nullptr);

	// Bind Tints VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[3]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tints.size(), &tints[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(float) * 4), nullptr);

	// Create & Bind IBO
	glGenBuffers(1, &m_IndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Unbind Global States
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}
}
