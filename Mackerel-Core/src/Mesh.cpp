#include "Mesh.h"

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
	// Check that Vertex Array Object Exists
	if (!m_VertexArrayObject)
	{
		return false;
	}

	glBindVertexArray(m_VertexArrayObject);
	return true;
}
/**
 * Renders Single Instance of Mesh.
 */
void Mesh::DrawMeshInstance()
{
	glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0);
}

/**  */
bool Mesh::generateVertexObjects(
	std::vector<Eigen::Vector3f> positions,
	std::vector<Eigen::Vector3f> normals,
	std::vector<Eigen::Vector2f> textureCoords,
	std::vector<Eigen::Vector3f> tints,
	std::vector<GLuint> indices)
{
	// Check that VAO does not Already Exist
	if (m_VertexArrayObject)
	{
		return false;
	}

	// Check that VBOs do not Already Exist
	for (auto vertexBufferObject : m_VertexBufferObjects) {
	if (vertexBufferObject)
	{
		return false;
	}}
	// Check that IBO does not Already Exist
	if (m_IndexBufferObject)
	{
		return false;
	}

	// Create & Bind the Vertex Array Object
	glGenVertexArrays(1, &m_VertexArrayObject);
	glBindVertexArray(m_VertexArrayObject);

	// Create the Vertex Buffer Objects
	glGenBuffers(4, m_VertexBufferObjects);

	// Bind Positions Buffer
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[0]);
		glBufferData(m_VertexBufferObjects[0], sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	// Bind Normals Buffer
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[1]);
		glBufferData(m_VertexBufferObjects[1], sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	// Bind TextureCoords Buffer
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[2]);
		glBufferData(m_VertexBufferObjects[2], sizeof(textureCoords[0]) * textureCoords.size(), &textureCoords[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}
	// Bind Tints Buffer
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[3]);
		glBufferData(m_VertexBufferObjects[3], sizeof(tints[0]) * tints.size(), &tints[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Create & Bind the Index Buffer Object
	glGenBuffers(1, &m_IndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// Unbind Global Attributes
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}
}
