#pragma once

#include <glad/glad.h>
#include <Eigen/Core.h>

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
	GLuint getUniformSize() override { return 4; }
};
struct UInt16Uniform : UniformData
{
	UInt16Uniform(uint16_t value) :
		UniformData(), value(value) {}

	uint16_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 4; }
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
	GLuint getUniformSize() override { return 4; }
};

struct Int08Uniform : UniformData
{
	Int08Uniform(int8_t value) :
		UniformData(), value(value) {}

	int8_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 4; }
};
struct Int16Uniform : UniformData
{
	Int16Uniform(int16_t value) :
		UniformData(), value(value) {}

	int16_t value;

	void* getUniformValue() override { return &value; }
	GLuint getUniformSize() override { return 4; }
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
	Vec2Uniform(Eigen::Vector2f value) :
		UniformData(), value(value) {}

	Eigen::Vector2f value;

	void* getUniformValue() override { return value.data(); }
	GLuint getUniformSize() override { return 8; }
};
struct Vec3Uniform : UniformData
{
	Vec3Uniform(Eigen::Vector3f value) :
		UniformData(), value(value) {}

	Eigen::Vector3f value;

	void* getUniformValue() override { return value.data(); }
	GLuint getUniformSize() override { return 16; }
};
struct Vec4Uniform : UniformData
{
	Vec4Uniform(Eigen::Vector4f value) :
		UniformData(), value(value) {}

	Eigen::Vector4f value;

	void* getUniformValue() override { return value.data(); }
	GLuint getUniformSize() override { return 16; }
};

struct UVec2Uniform : UniformData
{
	UVec2Uniform(Eigen::Vector2<uint32_t> value) :
		UniformData(), value(value) {}

	Eigen::Vector2<uint32_t> value;

	void* getUniformValue() override { return value.data(); }
	GLuint getUniformSize() override { return 8; }
};
struct UVec3Uniform : UniformData
{
	UVec3Uniform(Eigen::Vector3<uint32_t> value) :
		UniformData(), value(value) {}

	Eigen::Vector3<uint32_t> value;

	void* getUniformValue() override { return value.data(); }
	GLuint getUniformSize() override { return 16; }
};
struct UVec4Uniform : UniformData
{
	UVec4Uniform(Eigen::Vector4<uint32_t> value) :
		UniformData(), value(value) {}

	Eigen::Vector4<uint32_t> value;

	void* getUniformValue() override { return value.data(); }
	GLuint getUniformSize() override { return 16; }
};

struct Mat4Uniform : UniformData
{
	Mat4Uniform(Eigen::Matrix4f value) :
		UniformData(), value(value) {}

	Eigen::Matrix4f value;

	void* getUniformValue() override { return value.data(); }
	GLuint getUniformSize() override { return 64; }
};
}