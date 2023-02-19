#include "UniformData.h"

namespace MCK {
UniformData::UniformData(GLuint shaderProgrmID, std::string name) :
	uniformLocation(glGetUniformLocation(shaderProgrmID, name.c_str())), uniformName(name) {}

// uint8
UInt08Uniform::UInt08Uniform(GLuint location, std::string name, uint8_t value) :
	UniformData(location, name), value(value) {}
bool UInt08Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1ui(uniformLocation, value);
	return true;
}
// uint16
UInt16Uniform::UInt16Uniform(GLuint location, std::string name, uint16_t value) :
	UniformData(location, name), value(value) {}
bool UInt16Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1ui(uniformLocation, value);
	return true;
}
// uint32
UInt32Uniform::UInt32Uniform(GLuint location, std::string name, uint32_t value) :
	UniformData(location, name), value(value) {}
bool UInt32Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1ui(uniformLocation, value);
	return true;
}
// uint64 
UInt64Uniform::UInt64Uniform(GLuint location, std::string name, uint64_t value) :
	UniformData(location, name), value(value) {}
bool UInt64Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1ui(uniformLocation, static_cast<GLuint>(value));
	return true;
}

// int8
Int08Uniform::Int08Uniform(GLuint location, std::string name, int8_t value) :
	UniformData(location, name), value(value) {}
bool Int08Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1i(uniformLocation, value);
	return true;
}
// int16
Int16Uniform::Int16Uniform(GLuint location, std::string name, int16_t value) :
	UniformData(location, name), value(value) {}
bool Int16Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1i(uniformLocation, value);
	return true;
}
// int32
Int32Uniform::Int32Uniform(GLuint location, std::string name, int32_t value) :
	UniformData(location, name), value(value) {}
bool Int32Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1i(uniformLocation, value);
	return true;
}
// int64
Int64Uniform::Int64Uniform(GLuint location, std::string name, int64_t value) :
	UniformData(location, name), value(value) {}
bool Int64Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1i(uniformLocation, static_cast<GLint>(value));
	return true;
}

// float
FloatUniform::FloatUniform(GLuint location, std::string name, float value) :
	UniformData(location, name), value(value) {}
bool FloatUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1f(uniformLocation, value);
	return true;
}
// double
DoubleUniform::DoubleUniform(GLuint location, std::string name, double value) :
	UniformData(location, name), value(value) {}
bool DoubleUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform1d(uniformLocation, value);
	return true;
}

//vec2
Vec2Uniform::Vec2Uniform(GLuint location, std::string name, Eigen::Vector2f value) :
	UniformData(location, name), value(value) {}
bool Vec2Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform2f(uniformLocation, value[0], value[1]);
	return true;
}
//vec3
Vec3Uniform::Vec3Uniform(GLuint location, std::string name, Eigen::Vector3f value) :
	UniformData(location, name), value(value) {}
bool Vec3Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform3f(uniformLocation, value[0], value[1], value[2]);
	return true;
}
//vec4
Vec4Uniform::Vec4Uniform(GLuint location, std::string name, Eigen::Vector4f value) :
	UniformData(location, name), value(value) {}
bool Vec4Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform4f(uniformLocation, value[0], value[1], value[2], value[3]);
	return true;
}

//uvec2
UVec2Uniform::UVec2Uniform(GLuint location, std::string name, Eigen::Vector2<uint32_t> value) :
	UniformData(location, name), value(value) {}
bool UVec2Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform2ui(uniformLocation, value[0], value[1]);
	return true;
}
//uvec3
UVec3Uniform::UVec3Uniform(GLuint location, std::string name, Eigen::Vector3<uint32_t> value) :
	UniformData(location, name), value(value) {}
bool UVec3Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform3ui(uniformLocation, value[0], value[1], value[2]);
	return true;
}
//uvec4
UVec4Uniform::UVec4Uniform(GLuint location, std::string name, Eigen::Vector4<uint32_t> value) :
	UniformData(location, name), value(value) {}
bool UVec4Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	glUniform4ui(uniformLocation, value[0], value[1], value[2], value[3]);
	return true;
}

// Texture
TextureUniform::TextureUniform(GLuint location, std::string name, AssetType::Texture* texture) :
	UniformData(location, name), texture(texture) {}
bool TextureUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	if (!uniformLocation)
		return false;

	// Check if no more than 32 have been bound
	if (uniformState.texSlots >= 32)
		return false;

	// Get the Texture Slot
	GLuint texSlot = GL_TEXTURE0 + uniformState.texSlots;

	glBindTexture(texSlot, texture->getTextureID());
	glUniform1i(uniformLocation, uniformState.texSlots);

	uniformState.texSlots += 1;
	return true;
}
}