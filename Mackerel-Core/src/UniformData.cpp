#include "UniformData.h"

namespace MCK {
UniformData::UniformData(GLuint shaderprogram, std::string name) :
	uniformName(name), uniformLocation(glGetUniformLocation(shaderprogram, name.c_str())) {}

// uint8
UInt08Uniform::UInt08Uniform(GLuint shaderprogram, std::string name, uint8_t value) :
	UniformData(shaderprogram, name), value(value) {}

void UInt08Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, value);
}

// uint16
UInt16Uniform::UInt16Uniform(GLuint shaderprogram, std::string name, uint16_t value) :
	UniformData(shaderprogram, name), value(value) {}

void UInt16Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, value);
}

// uint32
UInt32Uniform::UInt32Uniform(GLuint shaderprogram, std::string name, uint32_t value) :
	UniformData(shaderprogram, name), value(value) {}

void UInt32Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, value);
}

// uint64 
UInt64Uniform::UInt64Uniform(GLuint shaderprogram, std::string name, uint64_t value) :
	UniformData(shaderprogram, name), value(value) {}

void UInt64Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, static_cast<GLuint>(value));
}

// int8
Int08Uniform::Int08Uniform(GLuint shaderprogram, std::string name, int8_t value) :
	UniformData(shaderprogram, name), value(value) {}

void Int08Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, value);
}

// int16
Int16Uniform::Int16Uniform(GLuint shaderprogram, std::string name, int16_t value) :
	UniformData(shaderprogram, name), value(value) {}

void Int16Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, value);
}

// int32
Int32Uniform::Int32Uniform(GLuint shaderprogram, std::string name, int32_t value) :
	UniformData(shaderprogram, name), value(value) {}

void Int32Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, value);
}

// int64
Int64Uniform::Int64Uniform(GLuint shaderprogram, std::string name, int64_t value) :
	UniformData(shaderprogram, name), value(value) {}

void Int64Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, static_cast<GLint>(value));
}

// float
FloatUniform::FloatUniform(GLuint shaderprogram, std::string name, float value) :
	UniformData(shaderprogram, name), value(value) {}

void FloatUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1f(uniformLocation, value);
}

// double
DoubleUniform::DoubleUniform(GLuint shaderprogram, std::string name, double value) :
	UniformData(shaderprogram, name), value(value) {}

void DoubleUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1d(uniformLocation, value);
}
//vec2
Vec2Uniform::Vec2Uniform(GLuint shaderProgram, std::string name, Eigen::Vector2f value) :
	UniformData(shaderProgram, name), value(value) {}

void Vec2Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform2f(uniformLocation, value[0], value[1]);
}
//vec3
Vec3Uniform::Vec3Uniform(GLuint shaderProgram, std::string name, Eigen::Vector3f value) :
	UniformData(shaderProgram, name), value(value) {}

void Vec3Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform3f(uniformLocation, value[0], value[1], value[2]);
}
//vec4
Vec4Uniform::Vec4Uniform(GLuint shaderProgram, std::string name, Eigen::Vector4f value) :
	UniformData(shaderProgram, name), value(value) {}

void Vec4Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform4f(uniformLocation, value[0], value[1], value[2], value[3]);
}
}