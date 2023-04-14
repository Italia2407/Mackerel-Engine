#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MCK {
struct UniformData
{
	UniformData() :
		uniformOffset(0) {}

	GLuint uniformOffset;

	virtual void* getUniformValue() = 0;
	virtual GLuint getUniformSize() = 0;
};

struct UInt08Uniform : UniformData
{
	UInt08Uniform(uint8_t value) :
		UniformData(), value(value) {}

	uint8_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 1; }
};
struct UInt16Uniform : UniformData
{
	UInt16Uniform(uint16_t value) :
		UniformData(), value(value) {}

	uint16_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 2; }
};
struct UInt32Uniform : UniformData
{
	UInt32Uniform(uint32_t value) :
		UniformData(), value(value) {}

	uint32_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 4; }
};
struct UInt64Uniform : UniformData
{
	UInt64Uniform(uint64_t value) :
		UniformData(), value(value) {}

	uint64_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 8; }
};

struct Int08Uniform : UniformData
{
	Int08Uniform(int8_t value) :
		UniformData(), value(value) {}

	int8_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 1; }
};
struct Int16Uniform : UniformData
{
	Int16Uniform(int16_t value) :
		UniformData(), value(value) {}

	int16_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 2; }
};
struct Int32Uniform : UniformData
{
	Int32Uniform(int32_t value) :
		UniformData(), value(value) {}

	int32_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 4; }
};
struct Int64Uniform : UniformData
{
	Int64Uniform(int64_t value) :
		UniformData(), value(value) {}

	int64_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 8; }
};

struct FloatUniform : UniformData
{
	FloatUniform(float value) :
		UniformData(), value(value) {}

	float value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 4; }
};
struct DoubleUniform : UniformData
{
	DoubleUniform(double value) :
		UniformData(), value(value) {}

	double value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 8; }
};

struct Vec2Uniform : UniformData
{
	Vec2Uniform(glm::vec2 value) :
		UniformData(), value(value) {}

	glm::vec2 value;

	void* getUniformValue() override { return glm::value_ptr(value); }
	GLuint getUniformSize() override { return 8; }
};
struct Vec3Uniform : UniformData
{
	Vec3Uniform(glm::vec3 value) :
		UniformData(), value(value) {}

	glm::vec3 value;

	void* getUniformValue() override { return glm::value_ptr(value); }
	GLuint getUniformSize() override { return 16; }
};
struct Vec4Uniform : UniformData
{
	Vec4Uniform(glm::vec4 value) :
		UniformData(), value(value) {}

	glm::vec4 value;

	void* getUniformValue() override { return glm::value_ptr(value); }
	GLuint getUniformSize() override { return 16; }
};

struct UVec2Uniform : UniformData
{
	UVec2Uniform(glm::uvec2 value) :
		UniformData(), value(value) {}

	glm::uvec2 value;

	void* getUniformValue() override { return glm::value_ptr(value); }
	GLuint getUniformSize() override { return 8; }
};
struct UVec3Uniform : UniformData
{
	UVec3Uniform(glm::uvec3 value) :
		UniformData(), value(value) {}

	glm::uvec3 value;

	void* getUniformValue() override { return glm::value_ptr(value); }
	GLuint getUniformSize() override { return 16; }
};
struct UVec4Uniform : UniformData
{
	UVec4Uniform(glm::uvec4 value) :
		UniformData(), value(value) {}

	glm::uvec4 value;

	void* getUniformValue() override { return glm::value_ptr(value); }
	GLuint getUniformSize() override { return 16; }
};

struct Mat4Uniform : UniformData
{
	Mat4Uniform(glm::mat4 value) :
		UniformData(), value(value) {}

	glm::mat4 value;

	void* getUniformValue() override { return glm::value_ptr(value); }
	GLuint getUniformSize() override { return 64; }
};
}