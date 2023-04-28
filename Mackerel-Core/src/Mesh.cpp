#include "LoggingSystem.h"
#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// Loggin Headers
#include "LoggingSystem.h"
#include <format>

// Module Only Structures
struct MeshVertex
{
	Eigen::Vector3f position;
	Eigen::Vector3f normal;
	Eigen::Vector2f textureCoord;
	Eigen::Vector3f tint;

	bool operator==(const MeshVertex& other) const
	{
		return position == other.position &&
			normal == other.normal &&
			textureCoord == other.textureCoord &&
			tint == other.tint;
	}
};

namespace MCK::AssetType {
Mesh::Mesh(std::string a_Name = "") :
	m_Name(a_Name),
	m_NumVertices(0), m_NumIndices(0),
	m_VertexArrayObject(GL_ZERO), m_VertexBufferObjects({}), m_IndexBufferObject(GL_ZERO) {}
Mesh::~Mesh()
{
	// Delete Vertex Array Object
	if (m_VertexArrayObject != GL_ZERO)
		glDeleteBuffers(1, &m_VertexArrayObject);

	// Delete Vertex Buffer Objects
	for (auto vertexBufferObject : m_VertexBufferObjects) {
	if (vertexBufferObject != GL_ZERO)
		glDeleteBuffers(1, &vertexBufferObject);
	}

	// Delete Vertex Index Object
	if (m_IndexBufferObject != GL_ZERO)
		glDeleteBuffers(1, &m_IndexBufferObject);
}

/**
 * Create the Mesh's OpenGL Objects used by the GPU.
 * 
 * \param a_Positions: The Mesh's Vertex Positions
 * \param a_Normals: The Mesh's Vertex Normals
 * \param a_TextureCoords: The Mesh's Vertex Texture Coordinates
 * \param a_Tints: The Mesh's Vertex Tints
 * \param a_Indices: The Mesh's Face Vertex Indices
 * \return Whether the OpenGL Objects could be Created
 */
bool Mesh::generateVertexObjects(
	const std::vector<float>& a_Positions,
	const std::vector<float>& a_Normals,
	const std::vector<float>& a_TextureCoords,
	const std::vector<float>& a_Tints,
	const std::vector<GLuint>& a_Indices)
{
	// Check that VAO does not Already Exist
	if (m_VertexArrayObject != GL_ZERO) {
		Logger::log("Cannot Recreate Vertex Array Object", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}
	// Check that VBOs do not Already Exist
	for (auto vertexBufferObject : m_VertexBufferObjects) {
	if (vertexBufferObject != GL_ZERO) {
		Logger::log("Cannot Recreate Vertex Buffer Objects", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}}
	// Check that IBO does not Already Exist
	if (m_IndexBufferObject != GL_ZERO) {
		Logger::log("Cannot Recreate Index Buffer Object", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create & Bind VAO
	glGenVertexArrays(1, &m_VertexArrayObject);
	glBindVertexArray(m_VertexArrayObject);

	// Create VBOs
	glCreateBuffers(4, m_VertexBufferObjects.data());

	// Create & Bind Positions VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a_Positions.size(), a_Positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

	// Bind Normals VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[1]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a_Normals.size(), &a_Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(float) * 3), nullptr);

	// Bind Texture Coords VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[2]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a_TextureCoords.size(), &a_TextureCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(float) * 2), nullptr);

	// Bind Tints VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjects[3]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * a_Tints.size(), &a_Tints[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (GLsizei)(sizeof(float) * 3), nullptr);

	// Create & Bind IBO
	glGenBuffers(1, &m_IndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * a_Indices.size(), &a_Indices[0], GL_STATIC_DRAW);

	// Unbind Global OpenGL States
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

/**
 * Load Mesh from Source File.
 * 
 * \param a_FilePath: The Mesh's File Path
 * \return Whether the Mesh could be Loaded
 */
bool Mesh::LoadFromFile(std::string a_FilePath)
{
	// Open Mesh File with TinyOBJ
	tinyobj::ObjReader fileReader;

	if (!fileReader.ParseFromFile(a_FilePath)) {
		if (!fileReader.Error().empty()) {
			Logger::log(std::format("TinyOBJ Reader: {}", fileReader.Error()), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		}
	}

	auto& attributes = fileReader.GetAttrib();
	auto& shapes = fileReader.GetShapes();

	// Store OBJ Data in Float Arrays
	std::vector<float> vertexPositions;
	std::vector<float> vertexNormals;
	std::vector<float> vertexTextureCoords;
	std::vector<float> vertexTints;

	std::vector<GLuint> vertexIndices;

	std::vector<MeshVertex> uniqueVertices;
	for (const auto& shape : shapes) {
	for (const auto& index : shape.mesh.indices)
	{
		MeshVertex vertex{};

		// Read Positon & Colour Values
		if (index.vertex_index >= 0) {
			vertex.position.x() = attributes.vertices[(3 * index.vertex_index) + 0];
			vertex.position.y() = attributes.vertices[(3 * index.vertex_index) + 1];
			vertex.position.z() = attributes.vertices[(3 * index.vertex_index) + 2];

			auto colourIndex = (3 * index.vertex_index) + 2;
			if (colourIndex < attributes.colors.size()) {
				vertex.tint.x() = attributes.colors[colourIndex - 2];
				vertex.tint.y() = attributes.colors[colourIndex - 1];
				vertex.tint.z() = attributes.colors[colourIndex - 0];
			}
			else {
				vertex.tint = Eigen::Vector3f(1.0f, 1.0f, 1.0f);
			}
		}
		// Read Normal Value
		if (index.normal_index >= 0) {
			vertex.normal.x() = attributes.normals[(3 * index.normal_index) + 0];
			vertex.normal.y() = attributes.normals[(3 * index.normal_index) + 1];
			vertex.normal.z() = attributes.normals[(3 * index.normal_index) + 2];
		}
		// Read Texture Coord Value
		if (index.texcoord_index >= 0) {
			vertex.textureCoord.x() = attributes.texcoords[(2 * index.texcoord_index) + 0];
			vertex.textureCoord.y() = attributes.texcoords[(2 * index.texcoord_index) + 1];
		}

		// TODO: Perhaps use Unordered Set, with Hash Function instead
		GLuint vertexIndex = (GLuint)uniqueVertices.size();
		bool duplicateVertex = false;
		for (size_t i = 0; i < uniqueVertices.size(); i++) {
			auto uniqueVertex = uniqueVertices[i];
			if (uniqueVertex == vertex) {
				vertexIndex = (GLuint)i;
				duplicateVertex = true;
				break;
			}
		}

		// Add Vertex to Unique Set
		if (!duplicateVertex)
		{
			uniqueVertices.push_back(vertex);

			// Add Vertex Params to Arrays
			vertexPositions.push_back(vertex.position.x());
			vertexPositions.push_back(vertex.position.y());
			vertexPositions.push_back(vertex.position.z());

			vertexNormals.push_back(vertex.normal.x());
			vertexNormals.push_back(vertex.normal.y());
			vertexNormals.push_back(vertex.normal.z());

			vertexTextureCoords.push_back(vertex.textureCoord.x());
			vertexTextureCoords.push_back(vertex.textureCoord.y());

			vertexTints.push_back(vertex.tint.x());
			vertexTints.push_back(vertex.tint.y());
			vertexTints.push_back(vertex.tint.z());
		}
		vertexIndices.push_back(vertexIndex);
	}}

	m_NumVertices = (GLuint)uniqueVertices.size();
	m_NumIndices = (GLuint)vertexIndices.size();

	// Generate Mesh GPU Data
	if (!generateVertexObjects(vertexPositions, vertexNormals, vertexTextureCoords, vertexTints, vertexIndices)) {
		Logger::log("Failed to Generate Mesh GPU Objects", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}

/**
 * Binds the Mesh's VAO to the GPU.
 * 
 * \return Whether the Mesh's VAO could be Bound
 */
bool Mesh::BindVertexArrayObject()
{
	// Bind the Vertex Array Object
	if (m_VertexArrayObject == GL_ZERO) {
		Logger::log("Vertex Array Object does not Exist", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}
	glBindVertexArray(m_VertexArrayObject);

	return true;
}
}
