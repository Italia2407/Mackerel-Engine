#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

#include <string>
#include <map>
#include <optional>

// Forward Declarations
namespace MCK {
class UniformBuffer;
}
namespace MCK::AssetType {
class Texture;
}

namespace MCK::AssetType {
class Material
{
public:
	Material();
	~Material();

private:
	UniformBuffer* m_UniformBuffer;
	Texture* m_MaterialTextures[32];

	bool addUInt08Uniform(std::string name, uint8_t value);
	bool addUInt16Uniform(std::string name, uint16_t value);
	bool addUInt32Uniform(std::string name, uint32_t value);
	bool addUInt64Uniform(std::string name, uint64_t value);

	bool addInt08Uniform(std::string name, int8_t value);
	bool addInt16Uniform(std::string name, int16_t value);
	bool addInt32Uniform(std::string name, int32_t value);
	bool addInt64Uniform(std::string name, int64_t value);

	bool addFloatUniform(std::string name, float value);
	bool addDoubleUniform(std::string name, double value);

	bool addVec2Uniform(std::string name, Eigen::Vector2f value);
	bool addVec3Uniform(std::string name, Eigen::Vector3f value);
	bool addVec4Uniform(std::string name, Eigen::Vector4f value);

	bool addUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value);
	bool addUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value);
	bool addUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value);

	bool addMat4Uniform(std::string name, Eigen::Matrix4f value);

public:
	bool UseMaterial();
	void ResetMaterial();

public:
	bool LoadFromFile(std::string a_FilePath);
	
	std::optional<uint8_t> GetUInt08Uniform(std::string name);
	bool SetUInt08Uniform(std::string name, uint8_t value);
	std::optional<uint16_t> GetUInt16Uniform(std::string name);
	bool SetUInt16Uniform(std::string name, uint16_t value);
	std::optional<uint32_t> GetUInt32Uniform(std::string name);
	bool SetUInt32Uniform(std::string name, uint32_t value);
	std::optional<uint64_t> GetUInt64Uniform(std::string name);
	bool SetUInt64Uniform(std::string name, uint64_t value);

	std::optional<int8_t> GetInt08Uniform(std::string name);
	bool SetInt08Uniform(std::string name, int8_t value);
	std::optional<int16_t> GetInt16Uniform(std::string name);
	bool SetInt16Uniform(std::string name, int16_t value);
	std::optional<int32_t> GetInt32Uniform(std::string name);
	bool SetInt32Uniform(std::string name, int32_t value);
	std::optional<int64_t> GetInt64Uniform(std::string name);
	bool SetInt64Uniform(std::string name, int64_t value);

	std::optional<float> GetFloatUniform(std::string name);
	bool SetFloatUniform(std::string name, float value);
	std::optional<double> GetDoubleUniform(std::string name);
	bool SetDoubleUniform(std::string name, double value);

	std::optional<Eigen::Vector2f> GetVec2Uniform(std::string name);
	bool SetVec2Uniform(std::string name, Eigen::Vector2f value);
	std::optional<Eigen::Vector3f> GetVec3Uniform(std::string name);
	bool SetVec3Uniform(std::string name, Eigen::Vector3f value);
	std::optional<Eigen::Vector4f> GetVec4Uniform(std::string name);
	bool SetVec4Uniform(std::string name, Eigen::Vector4f value);

	std::optional<Eigen::Vector2<uint32_t>> GetUVec2Uniform(std::string name);
	bool SetUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value);
	std::optional<Eigen::Vector3<uint32_t>> GetUVec3Uniform(std::string name);
	bool SetUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value);
	std::optional<Eigen::Vector4<uint32_t>> GetUVec4Uniform(std::string name);
	bool SetUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value);

	std::optional<Eigen::Matrix4f> GetMat4Uniform(std::string name);
	bool SetMat4Uniform(std::string name, Eigen::Matrix4f value);

	Texture* GetTexture(GLuint slot);
	void SetTexture(GLuint slot, Texture* texture);
};
}