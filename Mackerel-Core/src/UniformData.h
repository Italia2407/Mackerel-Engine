#pragma once

#include <glad/glad.h>
#include <string>

#include "Material.h"

namespace MCK {
struct UniformData
{
	UniformData(GLuint shaderProgram, std::string name);

	std::string uniformName;
	GLuint uniformLocation;

	virtual void Bind(AssetType::Material::UniformState& uniformState) = 0;
};

struct UInt08Uniform : UniformData
{
	UInt08Uniform(GLuint shaderProgram, std::string name, uint8_t value);

	uint8_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UInt16Uniform : UniformData
{
	UInt16Uniform(GLuint shaderProgram, std::string name, uint16_t value);

	uint16_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UInt32Uniform : UniformData
{
	UInt32Uniform(GLuint shaderProgram, std::string name, uint32_t value);

	uint32_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UInt64Uniform : UniformData
{
	UInt64Uniform(GLuint shaderProgram, std::string name, uint64_t value);

	uint64_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};

struct Int08Uniform : UniformData
{
	Int08Uniform(GLuint shaderProgram, std::string name, int8_t value);

	int8_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Int16Uniform : UniformData
{
	Int16Uniform(GLuint shaderProgram, std::string name, int16_t value);

	int16_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Int32Uniform : UniformData
{
	Int32Uniform(GLuint shaderProgram, std::string name, int32_t value);

	int32_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Int64Uniform : UniformData
{
	Int64Uniform(GLuint shaderProgram, std::string name, int64_t value);

	int64_t value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};

struct FloatUniform : UniformData
{
	FloatUniform(GLuint shaderProgram, std::string name, float value);

	float value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};
struct DoubleUniform : UniformData
{
	DoubleUniform(GLuint shaderProgram, std::string name, double value);

	double value;

	void Bind(AssetType::Material::UniformState& uniformState) override;
};

//uvec2
//uvec3
//uvec4

//vec2
//vec3
//vec4
}