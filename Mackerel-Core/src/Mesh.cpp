#include "LoggingSystem.h"
#include "Mesh.h"

#include "SkinnedMeshData.h"

#include <algorithm>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

/* tinygltf - general gltf loading */
#undef max
#define TINYGLTF_IMPLEMENTATION
#include "tiny_gltf.h"

/* ozz - animation loading and handling */
#include <ozz/animation/runtime/animation.h>
#include <ozz/base/io/stream.h>
#include <ozz/base/io/archive.h>

// Loggin Headers
#include "LoggingSystem.h"
#include <format>

const std::vector<std::string> MCK::AssetType::Mesh::obj_ext = { "obj" };
const std::vector<std::string> MCK::AssetType::Mesh::gltf_ascii_ext = { "gltf" };
const std::vector<std::string> MCK::AssetType::Mesh::gltf_binary_ext = { "glb" };

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
	m_VertexArrayObject(GL_ZERO), m_VertexBufferObjects({}), m_IndexBufferObject(GL_ZERO)
{
	m_VertexBufferObjects.resize(4, {});
}
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

	// Delete skinned mesh daata if applicable
	if (m_animData != nullptr)
		delete m_animData;
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
	// Get the extension of the mesh file to deduce it's file type.
	std::string ext = a_FilePath.substr(a_FilePath.find_last_of(".") + 1);

	// Is it an obj file?
	if (std::find(obj_ext.begin(), obj_ext.end(), ext) != obj_ext.end())
	{
		return LoadObj(a_FilePath);
	}
	
	// Is it an ascii gltf file?
	if (std::find(gltf_ascii_ext.begin(), gltf_ascii_ext.end(), ext) != gltf_ascii_ext.end())
	{
		return LoadGltf(a_FilePath, false);
	}

	// Is it a binary gltf file?
	if (std::find(gltf_binary_ext.begin(), gltf_binary_ext.end(), ext) != gltf_binary_ext.end())
	{
		return LoadGltf(a_FilePath, true);
	}

	// Unrecognized file type
	Logger::log(std::format("Mesh: Unsupported file type with extension: .{}", ext), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
	return false;
}

bool Mesh::LoadObj(std::string& a_FilePath)
{
	m_hasRig = false;

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
	std::vector<float> vertexTextureCoords;
	std::vector<float> vertexTints;

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
		}
	}

	m_NumVertices = (GLuint)uniqueVertices.size();
	m_NumIndices = (GLuint)vertexIndices.size();

	// Generate Mesh GPU Data
	if (!generateVertexObjects(vertexPositions, vertexNormals, vertexTextureCoords, vertexTints, vertexIndices)) {
		Logger::log("Failed to Generate OBJ Mesh GPU Objects", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}

bool Mesh::LoadGltf(std::string& a_FilePath, bool isBinary)
{
	if (m_animData != nullptr)
		delete m_animData;
	m_animData = new SkinnedMeshData();

	tinygltf::TinyGLTF gltfLoader;

	/* attempt to load the mesh data */
	if (!LoadGltfData(a_FilePath, isBinary, &m_animData->gltfModel, &gltfLoader))
		return false;

	/* determine if the model is animated and extract data */
	if (!GltfExtractUpload(a_FilePath))
		return false;

	return true;
}
bool Mesh::LoadGltfData(std::string& a_FilePath, bool isBinary, tinygltf::Model* gltfModel, tinygltf::TinyGLTF* gltfLoader)
{
	std::string gltfErr;
	std::string gltfWarn;

	bool ret = false;
	if (isBinary)
		ret = gltfLoader->LoadBinaryFromFile(gltfModel, &gltfErr, &gltfWarn, a_FilePath);
	else
		ret = gltfLoader->LoadASCIIFromFile(gltfModel, &gltfErr, &gltfWarn, a_FilePath);

	if (!gltfWarn.empty())
	{
		Logger::log(std::format("tinygltf: {}", gltfWarn), Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
	}

	if (!gltfErr.empty())
	{
		Logger::log(std::format("tinygltf: {}", gltfErr), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
	}

	if (!ret)
	{
		Logger::log(std::format("tinygltf: Failed to parse glTF file at: {}", a_FilePath), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}
bool Mesh::GltfExtractUpload(std::string& a_FilePath)
{
	std::vector<float> vertexTextureCoords;
	std::vector<float> vertexTints;

	vertexPositions.clear();
	vertexTints.clear();
	vertexNormals.clear();
	vertexTextureCoords.clear();
	vertexIndices.clear();

	// checking if the loaded model has an associated skeleton
	m_hasRig = false;
	for (auto mesh : m_animData->gltfModel.meshes)
	{
		for (auto primitive : mesh.primitives)
		{
			/* extract POSITION data... (and flub the tint data */
			uint32_t accessor_ind = primitive.attributes["POSITION"];

			tinygltf::Accessor accessor = m_animData->gltfModel.accessors[accessor_ind];
			tinygltf::BufferView bufferView = m_animData->gltfModel.bufferViews[accessor.bufferView];
			tinygltf::Buffer buffer = m_animData->gltfModel.buffers[bufferView.buffer];

			float* positions = reinterpret_cast<float*>(buffer.data.data() + bufferView.byteOffset);
			for (uint32_t i = 0; i < accessor.count * 3; i++)
			{
				vertexPositions.push_back(positions[i]);
				vertexTints.push_back(1.0f);
			}

			/* ...then get NORMAL data... */
			accessor_ind = primitive.attributes["NORMAL"];

			accessor = m_animData->gltfModel.accessors[accessor_ind];
			bufferView = m_animData->gltfModel.bufferViews[accessor.bufferView];
			buffer = m_animData->gltfModel.buffers[bufferView.buffer];

			float* normals = reinterpret_cast<float*>(buffer.data.data() + bufferView.byteOffset);
			for (uint32_t i = 0; i < accessor.count * 3; i++)
				vertexNormals.push_back(normals[i]);

			/* ...then get TEXCOORD data... */
			accessor_ind = primitive.attributes["TEXCOORD_0"];

			accessor = m_animData->gltfModel.accessors[accessor_ind];
			bufferView = m_animData->gltfModel.bufferViews[accessor.bufferView];
			buffer = m_animData->gltfModel.buffers[bufferView.buffer];

			float* uvs = reinterpret_cast<float*>(buffer.data.data() + bufferView.byteOffset);
			for (uint32_t i = 0; i < accessor.count * 2; i++)
				vertexTextureCoords.push_back(uvs[i]);

			/* ...then the vertex indices. */
			accessor = m_animData->gltfModel.accessors[primitive.indices];
			bufferView = m_animData->gltfModel.bufferViews[accessor.bufferView];
			buffer = m_animData->gltfModel.buffers[bufferView.buffer];

			uint16_t* indices = reinterpret_cast<uint16_t*>(buffer.data.data() + bufferView.byteOffset);
			for (uint32_t i = 0; i < accessor.count; i++)
				vertexIndices.push_back(indices[i]);

			/* store the final tallies */
			m_NumVertices = static_cast<GLuint>(vertexPositions.size());
			m_NumIndices = static_cast<GLuint>(vertexIndices.size());

			/* is the first joints slot filled? */
			if (primitive.attributes.contains("JOINTS_0") && m_hasRig == false)
			{
				/* this model has a rig */
				m_hasRig = true;

				/* load its animations */
				if (!GltfLoadAnimationData(a_FilePath))
					return false;
			}
		}
	}

	// Generate Mesh GPU Data
	if (!generateVertexObjects(vertexPositions, vertexNormals, vertexTextureCoords, vertexTints, vertexIndices)) {
		Logger::log("Failed to Generate GLTF Mesh GPU Objects", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}
bool Mesh::GltfLoadAnimationData(std::string& a_FilePath)
{
	/* acquire animations */
	m_animData->animationIndices.clear();
	for (auto animation = m_animData->gltfModel.animations.begin(); animation != m_animData->gltfModel.animations.end(); animation++)
	{
		m_animData->animationIndices.emplace(
			std::pair<std::string, uint16_t>(
				(*animation).name,
				static_cast<uint16_t>(animation - m_animData->gltfModel.animations.begin())));
	}

	/* load skeleton */
	std::string ozzPath = a_FilePath.substr(0, a_FilePath.find_last_of(".")) + ".ozz";
	ozz::io::File skeleFile(ozzPath.c_str(), "rb");
	ozz::io::IArchive archive(&skeleFile);

	if (!archive.TestTag<ozz::animation::Skeleton>())
	{
		Logger::log("Mesh .ozz file does not contain a skeleton!", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	archive >> m_animData->skeleton;

	return true;
}

void Mesh::SetAnimationPose(std::string animation, float time)
{
	if (!m_hasRig)
		Logger::log(std::format("SetAnimationPose() was called on a mesh without a rig!"
			"Did you mistakenly assign a non-skinned mesh to a SkinnedMeshRenderer component?"),
			Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");

	
}

int Mesh::NumberOfSoaJoints()
{
	if (m_hasRig)
	{
		assert(m_animData != nullptr);

		return m_animData->skeleton.num_soa_joints();
	}
	else
	{
		return 0;
	}
}
int Mesh::NumberOfModelJoints()
{
	if (m_hasRig)
	{
		assert(m_animData != nullptr);

		return m_animData->skeleton.num_joints();
	}
	else
	{
		return 0;
	}
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
