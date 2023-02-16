#include "UniformData.h"

namespace MCK {
UniformData::UniformData(GLuint shaderProgram, std::string name) :
	uniformName(name), uniformLocation(glGetUniformLocation(shaderProgram, name.c_str())) {}

// uint8
UInt08Uniform::UInt08Uniform(GLuint shaderprogram, std::string name, uint8_t value) :
	UniformData(shaderProgram, name), value(value) {}

void UInt08Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, value);
}
// uint16
UInt16Uniform::UInt16Uniform(GLuint shaderprogram, std::string name, uint16_t value) :
	UniformData(shaderProgram, name), value(value) {}

void UInt16Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, value);
}
// uint32
UInt32Uniform::UInt32Uniform(GLuint shaderprogram, std::string name, uint32_t value) :
	UniformData(shaderProgram, name), value(value) {}

void UInt32Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, value);
}
// uint64 
UInt64Uniform::UInt64Uniform(GLuint shaderprogram, std::string name, uint64_t value) :
	UniformData(shaderProgram, name), value(value) {}

void UInt64Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1ui(uniformLocation, value);
}
// int8 uniform
Int08Uniform::Int08Uniform(GLuint shaderProgram, std::string name, int8_t value) :
	UniformData(shaderProgram, name), value(value) {}

void Int08Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, value);
}
// int16 uniform
Int16Uniform::Int16Uniform(GLuint shaderProgram, std::string name, int16_t value) :
	UniformData(shaderProgram, name), value(value) {}

void Int16Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, value);
}
// int32 Uniform
Int32Uniform::Int32Uniform(GLuint shaderProgram, std::string name, int32_t value) :
	UniformData(shaderProgram, name), value(value) {}

void Int32Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, value);
}
// int64 Uniform
Int64Uniform::Int64Uniform(GLuint shaderProgram, std::string name, int64_t value) :
	UniformData(shaderProgram, name), value(value) {}

void Int64Uniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1i(uniformLocation, value);
}
// float Uniform
FloatUniform::FloatUniform(GLuint shaderProgram, std::string name, float value) :
	UniformData(shaderProgram, name), value(value) {}

void FloatUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1f(uniformLocation, value);
}
// double
DoubleUniform::DoubleUniform(GLuint shaderProgram, std::string name, double value) :
	UniformData(shaderProgram, name), value(value) {}

void DoubleUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1d(shaderProgram, value);
}


}