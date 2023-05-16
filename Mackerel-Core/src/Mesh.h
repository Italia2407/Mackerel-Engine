#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

#include <vector>
#include <array>
#include <string>

namespace MCK
{
	struct SkinnedMeshData;
}

namespace tinygltf{
	class Model;
	class TinyGLTF;
}

namespace MCK::AssetType {
class Mesh
{
private:
	static const std::vector<std::string> obj_ext;
	static const std::vector<std::string> gltf_ascii_ext;
	static const std::vector<std::string> gltf_binary_ext;

public:
	Mesh(std::string a_Name);
	~Mesh();

	GLuint NumVertices() const { return m_NumVertices; }
	const GLuint& NumIndices() const { return m_NumIndices; }

	// Mesh CPU Data
	std::vector<float> vertexPositions;
	std::vector<float> vertexNormals;
	std::vector<GLuint> vertexIndices;

	// Gltf Data
	SkinnedMeshData* m_animData = nullptr;

private:
	std::string m_Name = "";
	bool m_hasRig = false;

	// Mesh Properties
	GLuint m_NumVertices{};
	GLuint m_NumIndices{};

	// Mesh GPU Data
	GLuint m_VertexArrayObject{};

	std::vector<GLuint> m_VertexBufferObjects{};
	GLuint m_IndexBufferObject{};

	bool generateVertexObjects(
		const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& textureCoords, const std::vector<float>& tints, const std::vector<GLuint>& a_Indices);

public:
	bool LoadFromFile(std::string a_FilePath);

	bool LoadObj(std::string& a_FilePath);

	bool LoadGltf(std::string& a_FilePath, bool isBinary);
	bool LoadGltfData(std::string& a_FilePath, bool isBinary, tinygltf::Model* gltfModel, tinygltf::TinyGLTF* gltfLoader);
	void GltfSetup();
	void GltfUploadToGPU();
	void LoadGltfAnimations();

	void SetAnimationPose(std::string animation, float time);

	bool BindVertexArrayObject();
};
}