#include "UniformBuffer.h"

#include "UniformData.h"

namespace MCK {
UniformBuffer::UniformBuffer(std::string a_UniformBufferName) :
	m_UniformBufferObject(GL_ZERO), m_UnifromBufferName(a_UniformBufferName),
	m_BufferByteSize(0) {}
UniformBuffer::~UniformBuffer()
{
	DeleteUniformBufferObject();

	// Delete all Buffer Uniforms
	for (auto [BUName, BUData] : m_BufferUniforms)
	{
		if (BUData)
			delete BUData;
	}
	m_BufferUniforms.clear();
}

/**
 * Create a Uniform Buffer Object.
 *
 * \return Whether the Uniform Buffer Object was Successfully Created
 */
bool UniformBuffer::CreateUniformBufferObject()
{
	if (IsCreated())
		return false;

	// Create the Uniform Buffer Object Handle
	glGenBuffers(1, &m_UniformBufferObject);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);

	glBufferData(GL_UNIFORM_BUFFER, m_BufferByteSize, nullptr, GL_DYNAMIC_DRAW);

	// Attach Buffer Uniform Values to Uniform Buffer Object
	for (auto [BUName, BUData] : m_BufferUniforms)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, BUData->uniformOffset, BUData->getUniformSize(), BUData->getUniformValue());
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}
/**
 * Delete the Uniform Buffer Object.
 *
 * \return Whether the Uniform Buffer Object was Successfully Deleted
 */
bool UniformBuffer::DeleteUniformBufferObject()
{
	return true;
}

/**
 * Bind the Uniform Buffer Object.
 *
 * \param bindingSlot: The Slot to which teh UBO is Bounded
 * \return Whether the UBO was Successfully Bounded
 */
bool UniformBuffer::BindUniformBufferObject(GLuint a_BindingSlot)
{
	if (!IsCreated())
		return false;

	// Bind Uniform Buffer to Slot
	glBindBufferRange(GL_UNIFORM_BUFFER, a_BindingSlot, m_UniformBufferObject, 0, m_BufferByteSize);
	return true;
}

void UniformBuffer::addBufferUniform(std::string a_Name, UniformData* a_BufferUniform)
{
	// Add Padding if New Buffer Uniform Removes 16 Bytes Allignment
	if (getPaddingSize() != 16 && getPaddingSize() < a_BufferUniform->getUniformSize())
		m_BufferByteSize += getPaddingSize();

	// Save & Set Buffer Uniform Parameters
	a_BufferUniform->uniformOffset = m_BufferByteSize;
	m_BufferByteSize += a_BufferUniform->getUniformSize();

	m_BufferUniforms[a_Name] = a_BufferUniform;
}


// UInt08
/**
 * Add New UInt08 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddUInt08BufferUniform(std::string name, uint8_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	UInt08Uniform* bufferUniform = new UInt08Uniform(value);
	addBufferUniform(name, bufferUniform);
	
	return true;
}
/**
 * Get Value of UInt08 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<uint8_t> UniformBuffer::GetUInt08BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	UInt08Uniform* castBufferUniform = dynamic_cast<UInt08Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt08 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of UInt08 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetUInt08BufferUniform(std::string name, uint8_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	UInt08Uniform* castBufferUniform = dynamic_cast<UInt08Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt08 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// UInt16
/**
 * Add New UInt16 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddUInt16BufferUniform(std::string name, uint16_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	UInt16Uniform* bufferUniform = new UInt16Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of UInt16 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<uint16_t> UniformBuffer::GetUInt16BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	UInt16Uniform* castBufferUniform = dynamic_cast<UInt16Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt16 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of UInt16 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetUInt16BufferUniform(std::string name, uint16_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	UInt16Uniform* castBufferUniform = dynamic_cast<UInt16Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt16 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// UInt32
/**
 * Add New UInt32 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddUInt32BufferUniform(std::string name, uint32_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	UInt32Uniform* bufferUniform = new UInt32Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of UInt32 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<uint32_t> UniformBuffer::GetUInt32BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	UInt32Uniform* castBufferUniform = dynamic_cast<UInt32Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt32 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of UInt32 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetUInt32BufferUniform(std::string name, uint32_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	UInt32Uniform* castBufferUniform = dynamic_cast<UInt32Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt32 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// UInt64
/**
 * Add New UInt64 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddUInt64BufferUniform(std::string name, uint64_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	UInt64Uniform* bufferUniform = new UInt64Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of UInt64 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<uint64_t> UniformBuffer::GetUInt64BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	UInt64Uniform* castBufferUniform = dynamic_cast<UInt64Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt64 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of UInt64 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetUInt64BufferUniform(std::string name, uint64_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	UInt64Uniform* castBufferUniform = dynamic_cast<UInt64Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UInt64 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}


// Int08
/**
 * Add New Int08 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddInt08BufferUniform(std::string name, int8_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Int08Uniform* bufferUniform = new Int08Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Int08 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<int8_t> UniformBuffer::GetInt08BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Int08Uniform* castBufferUniform = dynamic_cast<Int08Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int08 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Int08 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetInt08BufferUniform(std::string name, int8_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Int08Uniform* castBufferUniform = dynamic_cast<Int08Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int08 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// Int16
/**
 * Add New Int16 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddInt16BufferUniform(std::string name, int16_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Int16Uniform* bufferUniform = new Int16Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Int16 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<int16_t> UniformBuffer::GetInt16BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Int16Uniform* castBufferUniform = dynamic_cast<Int16Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int16 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Int16 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetInt16BufferUniform(std::string name, int16_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Int16Uniform* castBufferUniform = dynamic_cast<Int16Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int16 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// Int32
/**
 * Add New Int32 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddInt32BufferUniform(std::string name, int32_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Int32Uniform* bufferUniform = new Int32Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Int32 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<int32_t> UniformBuffer::GetInt32BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Int32Uniform* castBufferUniform = dynamic_cast<Int32Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int32 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Int32 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetInt32BufferUniform(std::string name, int32_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Int32Uniform* castBufferUniform = dynamic_cast<Int32Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int32 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// Int64
/**
 * Add New Int64 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddInt64BufferUniform(std::string name, int64_t value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Int64Uniform* bufferUniform = new Int64Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Int64 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<int64_t> UniformBuffer::GetInt64BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Int64Uniform* castBufferUniform = dynamic_cast<Int64Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int64 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Int64 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetInt64BufferUniform(std::string name, int64_t value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Int64Uniform* castBufferUniform = dynamic_cast<Int64Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Int64 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}


// Float
/**
 * Add New Float Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddFloatBufferUniform(std::string name, float value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	FloatUniform* bufferUniform = new FloatUniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Float Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<float> UniformBuffer::GetFloatBufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	FloatUniform* castBufferUniform = dynamic_cast<FloatUniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Float Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Float Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetFloatBufferUniform(std::string name, float value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	FloatUniform* castBufferUniform = dynamic_cast<FloatUniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Float Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// Double
/**
 * Add New Double Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddDoubleBufferUniform(std::string name, double value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	DoubleUniform* bufferUniform = new DoubleUniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Double Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<double> UniformBuffer::GetDoubleBufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	DoubleUniform* castBufferUniform = dynamic_cast<DoubleUniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Double Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Double Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetDoubleBufferUniform(std::string name, double value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	DoubleUniform* castBufferUniform = dynamic_cast<DoubleUniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Double Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}


// Vec2
/**
 * Add New Vec2 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddVec2BufferUniform(std::string name, glm::vec2 value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Vec2Uniform* bufferUniform = new Vec2Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Vec2 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<glm::vec2> UniformBuffer::GetVec2BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Vec2Uniform* castBufferUniform = dynamic_cast<Vec2Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Vec2 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Vec2 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetVec2BufferUniform(std::string name, glm::vec2 value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Vec2Uniform* castBufferUniform = dynamic_cast<Vec2Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Vec2 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// Vec3
/**
 * Add New Vec3 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddVec3BufferUniform(std::string name, glm::vec3 value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Vec3Uniform* bufferUniform = new Vec3Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Vec3 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<glm::vec3> UniformBuffer::GetVec3BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Vec3Uniform* castBufferUniform = dynamic_cast<Vec3Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Vec3 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Vec3 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetVec3BufferUniform(std::string name, glm::vec3 value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Vec3Uniform* castBufferUniform = dynamic_cast<Vec3Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Vec3 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// Vec4
/**
 * Add New Vec4 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddVec4BufferUniform(std::string name, glm::vec4 value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Vec4Uniform* bufferUniform = new Vec4Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Vec4 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<glm::vec4> UniformBuffer::GetVec4BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Vec4Uniform* castBufferUniform = dynamic_cast<Vec4Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Vec4 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Vec4 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetVec4BufferUniform(std::string name, glm::vec4 value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Vec4Uniform* castBufferUniform = dynamic_cast<Vec4Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Vec4 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}


// UVec2
/**
 * Add New UVec2 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddUVec2BufferUniform(std::string name, glm::uvec2 value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	UVec2Uniform* bufferUniform = new UVec2Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of UVec2 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<glm::uvec2> UniformBuffer::GetUVec2BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	UVec2Uniform* castBufferUniform = dynamic_cast<UVec2Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UVec2 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of UVec2 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetUVec2BufferUniform(std::string name, glm::uvec2 value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	UVec2Uniform* castBufferUniform = dynamic_cast<UVec2Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UVec2 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// UVec3
/**
 * Add New UVec3 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddUVec3BufferUniform(std::string name, glm::uvec3 value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	UVec3Uniform* bufferUniform = new UVec3Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of UVec3 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<glm::uvec3> UniformBuffer::GetUVec3BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	UVec3Uniform* castBufferUniform = dynamic_cast<UVec3Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UVec3 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of UVec3 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetUVec3BufferUniform(std::string name, glm::uvec3 value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	UVec3Uniform* castBufferUniform = dynamic_cast<UVec3Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UVec3 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// UVec4
/**
 * Add New UVec4 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddUVec4BufferUniform(std::string name, glm::uvec4 value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	UVec4Uniform* bufferUniform = new UVec4Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of UVec4 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<glm::uvec4> UniformBuffer::GetUVec4BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	UVec4Uniform* castBufferUniform = dynamic_cast<UVec4Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UVec4 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of UVec4 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetUVec4BufferUniform(std::string name, glm::uvec4 value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	UVec4Uniform* castBufferUniform = dynamic_cast<UVec4Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't UVec4 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}

// Mat4
/**
 * Add New Mat4 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \param value: The Buffer Uniform's Value
 * \return Whether the Buffer Uniform was Successfully Added
 */
bool UniformBuffer::AddMat4BufferUniform(std::string name, glm::mat4 value)
{
	if (IsCreated())
	{// Cannot add Buffer Uniforms if UBO is already Created
		return false;
	}
	if (m_BufferUniforms.contains(name))
	{// Buffer Uniform already Exists with the Given Name
		return false;
	}

	Mat4Uniform* bufferUniform = new Mat4Uniform(value);
	addBufferUniform(name, bufferUniform);

	return true;
}
/**
 * Get Value of Mat4 Buffer Uniform.
 *
 * \param name: The Buffer Uniform's Name
 * \return The Buffer Uniform's Value, if Existent
 */
std::optional<glm::mat4> UniformBuffer::GetMat4BufferUniform(std::string name)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return {};
	}
	Mat4Uniform* castBufferUniform = dynamic_cast<Mat4Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Mat4 Type
		return {};
	}

	return castBufferUniform->value;
}
/**
 * Change Value of Mat4 Buffer Uniform.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's New Value
 * \return  Whether the Uniform's Value was Successfully Changed
 */
bool UniformBuffer::SetMat4BufferUniform(std::string name, glm::mat4 value)
{
	if (!m_BufferUniforms.contains(name))
	{// No Buffer Uniform Exists with the Given Name
		return false;
	}
	Mat4Uniform* castBufferUniform = dynamic_cast<Mat4Uniform*>(m_BufferUniforms[name]);
	if (!castBufferUniform)
	{// Buffer Uniform isn't Mat4 Type
		return false;
	}

	// Update Buffer Uniform Data
	castBufferUniform->value = value;

	// Update Uniform Buffer Object Data
	glBindBuffer(GL_UNIFORM_BUFFER, m_UniformBufferObject);
	glBufferSubData(GL_UNIFORM_BUFFER, castBufferUniform->uniformOffset, castBufferUniform->getUniformSize(), castBufferUniform->getUniformValue());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return true;
}
}
