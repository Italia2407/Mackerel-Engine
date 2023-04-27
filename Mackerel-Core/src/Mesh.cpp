#include "LoggingSystem.h"
#include "Mesh.h"

#include <format>

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
	// Open Mesh File
	std::ifstream meshFile(a_FilePath.c_str(), std::ios::in);
	if (!meshFile.good()) {
		meshFile.close();

		Logger::log(std::format("Cannot Load Mesh File", a_FilePath), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Reading Arrays
	std::vector<Eigen::Vector3f> readPositions;
	std::vector<Eigen::Vector3f> readNormals;
	std::vector<Eigen::Vector2f> readTextureCoords;
	std::vector<Eigen::Vector3f> readTints;

	std::vector<uint32_t> vertexIndices;
	std::vector<uint32_t> textureCoordIndices;
	std::vector<uint32_t> normalIndices;

	// Parse each Line of the Mesh File
	std::string meshFileReadLine;
	while (std::getline(meshFile, meshFileReadLine))
	{
		std::stringstream meshLineWordStream(meshFileReadLine);

		// Get the First Word of the Current Line
		std::string firstWord;
		meshLineWordStream >> firstWord;

		// Read Vertex Positions & Tints
		if (firstWord == "v")
		{
			// Positions
			std::string p0, p1, p2;
			meshLineWordStream >> p0;
			meshLineWordStream >> p1;
			meshLineWordStream >> p2;

			Eigen::Vector3f position(std::stof(p0), std::stof(p1), std::stof(p2));
			readPositions.push_back(position);

			// Tints
			std::string c0 = "1.0", c1 = "1,0", c2 = "1.0";
			meshLineWordStream >> c0;
			meshLineWordStream >> c1;
			meshLineWordStream >> c2;

			Eigen::Vector3f tint(std::stof(c0), std::stof(c1), std::stof(c2));
			readTints.push_back(tint);
		}
		// Read Vertex Normals
		else if (firstWord == "vn")
		{
			std::string n0, n1, n2;
			meshLineWordStream >> n0;
			meshLineWordStream >> n1;
			meshLineWordStream >> n2;

			Eigen::Vector3f normal(std::stof(n0), std::stof(n1), std::stof(n2));
			readNormals.push_back(normal);
		}
		// Read Vertex Normals
		else if (firstWord == "vt")
		{
			std::string t0, t1;
			meshLineWordStream >> t0;
			meshLineWordStream >> t1;

			Eigen::Vector2f textureCoord(std::stof(t0), std::stof(t1));
			readTextureCoords.push_back(textureCoord);
		}

		// Read Mesh Face
		else if (firstWord == "f")
		{
			// Loop for each Face Vertex
			for (int i = 0; i < 3; i++)
			{
				std::string f;
				meshLineWordStream >> f;
				std::istringstream face(f);

				// Get Face Vertex Indices
				std::string vertexIndex;
				std::string normalIndex;
				std::string textureCoordIndex;

				std::getline(face, vertexIndex, '/');
				std::getline(face, textureCoordIndex, '/');
				std::getline(face, normalIndex, '/');

				vertexIndices.push_back((uint32_t)std::stoi(vertexIndex)-1);
				textureCoordIndices.push_back((uint32_t)std::stoi(textureCoordIndex)-1);
				normalIndices.push_back((uint32_t)std::stoi(normalIndex)-1);
			}
		}
	}
	meshFile.close();

	m_NumVertices = (GLuint)readPositions.size();
	m_NumIndices = (GLuint)vertexIndices.size();

	// Computing
	std::vector<float> vertexPositions(m_NumVertices * 3);
	std::vector<float> vertexNormals(m_NumVertices * 3);
	std::vector<float> vertexTextureCoords(m_NumVertices * 2);
	std::vector<float> vertexTints(m_NumVertices * 3);

	for (int i = 0; i < vertexIndices.size(); i++)
	{
		auto vertexIndex = vertexIndices[i];
		auto normalIndex = normalIndices[i];
		auto texCoordIndex = textureCoordIndices[i];

		// Set Vertex Positions
		vertexPositions[vertexIndex * 3 + 0] = readPositions[vertexIndex].x();
		vertexPositions[vertexIndex * 3 + 1] = readPositions[vertexIndex].y();
		vertexPositions[vertexIndex * 3 + 2] = readPositions[vertexIndex].z();

		// Set Vertex Normals
		vertexNormals[vertexIndex * 3 + 0] = readNormals[normalIndex].x();
		vertexNormals[vertexIndex * 3 + 1] = readNormals[normalIndex].y();
		vertexNormals[vertexIndex * 3 + 2] = readNormals[normalIndex].z();

		// Set Vertex Texture Coords
		vertexTextureCoords[vertexIndex * 2 + 0] = readTextureCoords[texCoordIndex].x();
		vertexTextureCoords[vertexIndex * 2 + 1] = readTextureCoords[texCoordIndex].y();

		// Set Vertex Tints
		vertexTints[vertexIndex * 3 + 0] = readTints[vertexIndex].x();
		vertexTints[vertexIndex * 3 + 1] = readTints[vertexIndex].y();
		vertexTints[vertexIndex * 3 + 2] = readTints[vertexIndex].z();
	}

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
