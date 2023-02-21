#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

#include "Shader.h"
#include "Texture.h"

#include <string>
#include <map>

namespace MCK {
struct UniformData;
}

namespace MCK::AssetType {
class Material
{
public:
	Material(Shader* shader);
	~Material();

	struct UniformState
	{
		GLuint texSlots = 0;
	};

private:
	Shader* _shader;

	UniformState _uniformState;
	std::map<std::string, UniformData*> _uniforms;

	UniformData* getUniform(std::string name);

public:
	Shader* shader() { return _shader; }

	bool AddUInt08Uniform(std::string name, uint8_t value);
	bool SetUInt08Uniform(std::string name, uint8_t value);
	bool AddUInt16Uniform(std::string name, uint16_t value);
	bool SetUInt16Uniform(std::string name, uint16_t value);
	bool AddUInt32Uniform(std::string name, uint32_t value);
	bool SetUInt32Uniform(std::string name, uint32_t value);
	bool AddUInt64Uniform(std::string name, uint64_t value);
	bool SetUInt64Uniform(std::string name, uint64_t value);

	bool AddInt08Uniform(std::string name, int8_t value);
	bool SetInt08Uniform(std::string name, int8_t value);
	bool AddInt16Uniform(std::string name, int16_t value);
	bool SetInt16Uniform(std::string name, int16_t value);
	bool AddInt32Uniform(std::string name, int32_t value);
	bool SetInt32Uniform(std::string name, int32_t value);
	bool AddInt64Uniform(std::string name, int64_t value);
	bool SetInt64Uniform(std::string name, int64_t value);

	bool AddFloatUniform(std::string name, float value);
	bool SetFloatUniform(std::string name, float value);
	bool AddDoubleUniform(std::string name, double value);
	bool SetDoubleUniform(std::string name, double value);

	bool AddVec2Uniform(std::string name, Eigen::Vector2f value);
	bool SetVec2Uniform(std::string name, Eigen::Vector2f value);
	bool AddVec3Uniform(std::string name, Eigen::Vector3f value);
	bool SetVec3Uniform(std::string name, Eigen::Vector3f value);
	bool AddVec4Uniform(std::string name, Eigen::Vector4f value);
	bool SetVec4Uniform(std::string name, Eigen::Vector4f value);

	bool AddUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value);
	bool SetUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value);
	bool AddUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value);
	bool SetUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value);
	bool AddUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value);
	bool SetUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value);

	bool AddTextureUniform(std::string name, AssetType::Texture* texture);
	bool SetTextureUniform(std::string name, AssetType::Texture* texture);

	bool BindUniforms();
	void ResetUniforms();
};
}