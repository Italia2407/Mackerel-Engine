#pragma once

#include <glad/glad.h>
#include <string>
#include <Eigen/Core.h>

#include "Material.h"
#include "Texture.h"

namespace MCK {
struct UniformData
{
	UniformData(GLuint location, std::string name);

	GLuint uniformLocation;
	std::string uniformName;

	virtual bool Bind(AssetType::Material::UniformState& uniformState) = 0;
};

struct UInt08Uniform : UniformData
{
	UInt08Uniform(GLuint location, std::string name, uint8_t value);

	uint8_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UInt16Uniform : UniformData
{
	UInt16Uniform(GLuint location, std::string name, uint16_t value);

	uint16_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UInt32Uniform : UniformData
{
	UInt32Uniform(GLuint location, std::string name, uint32_t value);

	uint32_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UInt64Uniform : UniformData
{
	UInt64Uniform(GLuint location, std::string name, uint64_t value);

	uint64_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};

struct Int08Uniform : UniformData
{
	Int08Uniform(GLuint location, std::string name, int8_t value);

	int8_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Int16Uniform : UniformData
{
	Int16Uniform(GLuint location, std::string name, int16_t value);

	int16_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Int32Uniform : UniformData
{
	Int32Uniform(GLuint location, std::string name, int32_t value);

	int32_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Int64Uniform : UniformData
{
	Int64Uniform(GLuint location, std::string name, int64_t value);

	int64_t value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};

struct FloatUniform : UniformData
{
	FloatUniform(GLuint location, std::string name, float value);

	float value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct DoubleUniform : UniformData
{
	DoubleUniform(GLuint location, std::string name, double value);

	double value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};

struct Vec2Uniform : UniformData
{
	Vec2Uniform(GLuint location, std::string name, Eigen::Vector2f value);

	Eigen::Vector2f value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Vec3Uniform : UniformData
{
	Vec3Uniform(GLuint location, std::string name, Eigen::Vector3f value);

	Eigen::Vector3f value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct Vec4Uniform : UniformData
{
	Vec4Uniform(GLuint location, std::string name, Eigen::Vector4f value);

	Eigen::Vector4f value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};

struct UVec2Uniform : UniformData
{
	UVec2Uniform(GLuint location, std::string name, Eigen::Vector2<uint32_t> value);

	Eigen::Vector2<uint32_t> value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UVec3Uniform : UniformData
{
	UVec3Uniform(GLuint location, std::string name, Eigen::Vector3<uint32_t> value);

	Eigen::Vector3<uint32_t> value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
struct UVec4Uniform : UniformData
{
	UVec4Uniform(GLuint location, std::string name, Eigen::Vector4<uint32_t> value);

	Eigen::Vector4<uint32_t> value;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};

struct TextureUniform : UniformData
{
	TextureUniform(GLuint location, std::string name, AssetType::Texture* texture);

	AssetType::Texture* texture;

	bool Bind(AssetType::Material::UniformState& uniformState) override;
};
}