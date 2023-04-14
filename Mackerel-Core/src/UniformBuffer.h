#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <map>
#include <optional>

// Forward Declarations
namespace MCK {
struct UniformData;
}

namespace MCK {
class UniformBuffer
{
public:
	UniformBuffer(std::string a_UniformBufferName = "UniformBuffer");
	~UniformBuffer();

private:
	GLuint m_UniformBufferObject;
	std::string m_UnifromBufferName;

	GLuint m_BufferByteSize;
	std::map<std::string, UniformData*> m_BufferUniforms;

public:
	GLuint getBufferByteSize() { return m_BufferByteSize; }
	GLuint getPaddingSize() { return 16 - (m_BufferByteSize % 16); }
	bool IsCreated() { return m_UniformBufferObject != GL_ZERO; }

public:
	bool CreateUniformBufferObject();
	bool DeleteUniformBufferObject();

	bool BindUniformBufferObject(GLuint a_BindingSlot);

private:
	void addBufferUniform(std::string a_Name, UniformData* a_BufferUniform);

public:
	// UInt08
	bool AddUInt08BufferUniform(std::string name, uint8_t value);
	std::optional<uint8_t> GetUInt08BufferUniform(std::string name);
	bool SetUInt08BufferUniform(std::string name, uint8_t value);
	// UInt16
	bool AddUInt16BufferUniform(std::string name, uint16_t value);
	std::optional<uint16_t> GetUInt16BufferUniform(std::string name);
	bool SetUInt16BufferUniform(std::string name, uint16_t value);
	// UInt32
	bool AddUInt32BufferUniform(std::string name, uint32_t value);
	std::optional<uint32_t> GetUInt32BufferUniform(std::string name);
	bool SetUInt32BufferUniform(std::string name, uint32_t value);
	// UInt64
	bool AddUInt64BufferUniform(std::string name, uint64_t value);
	std::optional<uint64_t> GetUInt64BufferUniform(std::string name);
	bool SetUInt64BufferUniform(std::string name, uint64_t value);

	// Int08
	bool AddInt08BufferUniform(std::string name, int8_t value);
	std::optional<int8_t> GetInt08BufferUniform(std::string name);
	bool SetInt08BufferUniform(std::string name, int8_t value);
	// Int16
	bool AddInt16BufferUniform(std::string name, int16_t value);
	std::optional<int16_t> GetInt16BufferUniform(std::string name);
	bool SetInt16BufferUniform(std::string name, int16_t value);
	// Int32
	bool AddInt32BufferUniform(std::string name, int32_t value);
	std::optional<int32_t> GetInt32BufferUniform(std::string name);
	bool SetInt32BufferUniform(std::string name, int32_t value);
	// Int64
	bool AddInt64BufferUniform(std::string name, int64_t value);
	std::optional<int64_t> GetInt64BufferUniform(std::string name);
	bool SetInt64BufferUniform(std::string name, int64_t value);

	// Float
	bool AddFloatBufferUniform(std::string name, float value);
	std::optional<float> GetFloatBufferUniform(std::string name);
	bool SetFloatBufferUniform(std::string name, float value);
	// Double
	bool AddDoubleBufferUniform(std::string name, double value);
	std::optional<double> GetDoubleBufferUniform(std::string name);
	bool SetDoubleBufferUniform(std::string name, double value);

	// Vec2
	bool AddVec2BufferUniform(std::string name, glm::vec2 value);
	std::optional<glm::vec2> GetVec2BufferUniform(std::string name);
	bool SetVec2BufferUniform(std::string name, glm::vec2 value);
	// Vec3
	bool AddVec3BufferUniform(std::string name, glm::vec3 value);
	std::optional<glm::vec3> GetVec3BufferUniform(std::string name);
	bool SetVec3BufferUniform(std::string name, glm::vec3 value);
	// Vec4
	bool AddVec4BufferUniform(std::string name, glm::vec4 value);
	std::optional<glm::vec4> GetVec4BufferUniform(std::string name);
	bool SetVec4BufferUniform(std::string name, glm::vec4 value);

	// UVec2
	bool AddUVec2BufferUniform(std::string name, glm::uvec2 value);
	std::optional<glm::uvec2> GetUVec2BufferUniform(std::string name);
	bool SetUVec2BufferUniform(std::string name, glm::uvec2 value);
	// UVec3
	bool AddUVec3BufferUniform(std::string name, glm::uvec3 value);
	std::optional<glm::uvec3> GetUVec3BufferUniform(std::string name);
	bool SetUVec3BufferUniform(std::string name, glm::uvec3 value);
	// UVec4
	bool AddUVec4BufferUniform(std::string name, glm::uvec4 value);
	std::optional<glm::uvec4> GetUVec4BufferUniform(std::string name);
	bool SetUVec4BufferUniform(std::string name, glm::uvec4 value);

	// Mat4
	bool AddMat4BufferUniform(std::string name, glm::mat4 value);
	std::optional<glm::mat4> GetMat4BufferUniform(std::string name);
	bool SetMat4BufferUniform(std::string name, glm::mat4 value);
};
}
