#include "UniformData.h"

namespace MCK {
UniformData::UniformData(GLuint shaderProgram, std::string name) :
	uniformName(name), uniformLocation(glGetUniformLocation(shaderProgram, name.c_str())) {}


FloatUniform::FloatUniform(GLuint shaderProgram, std::string name, float value) :
	UniformData(shaderProgram, name), value(value) {}

void FloatUniform::Bind(AssetType::Material::UniformState& uniformState)
{
	glUniform1f(uniformLocation, value);
}
}