#include "Material.h"

#include "UniformBuffer.h"
#include "Texture.h"

#include <iostream>

namespace MCK::AssetType {
Material::Material() :
	_uniformBuffer(nullptr)
{
	// Initialise Uniform Buffer
	_uniformBuffer = new UniformBuffer();

	for (int i = 0; i < 32; i++)
	{// Initialise Material Texture References
		_materialTextures[i] = nullptr;
	}
}
Material::~Material()
{
	// Delete Uniform Buffer Object
	_uniformBuffer->DeleteUniformBufferObject();
	delete _uniformBuffer;
}

// Uniform Adders
/**
 * Add a new UInt08 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addUInt08Uniform(std::string name, uint8_t value)
{
	bool result = _uniformBuffer->AddUInt08BufferUniform(name, value);
	return result;
}
/**
 * Add a new UInt16 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addUInt16Uniform(std::string name, uint16_t value)
{
	bool result = _uniformBuffer->AddUInt16BufferUniform(name, value);
	return result;
}
/**
 * Add a new UInt32 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addUInt32Uniform(std::string name, uint32_t value)
{
	bool result = _uniformBuffer->AddUInt32BufferUniform(name, value);
	return result;
}
/**
 * Add a new UInt64 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addUInt64Uniform(std::string name, uint64_t value)
{
	bool result = _uniformBuffer->AddUInt64BufferUniform(name, value);
	return result;
}

/**
 * Add a new Int08 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addInt08Uniform(std::string name, int8_t value)
{
	bool result = _uniformBuffer->AddInt08BufferUniform(name, value);
	return result;
}
/**
 * Add a new Int16 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addInt16Uniform(std::string name, int16_t value)
{
	bool result = _uniformBuffer->AddInt16BufferUniform(name, value);
	return result;
}
/**
 * Add a new Int32 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addInt32Uniform(std::string name, int32_t value)
{
	bool result = _uniformBuffer->AddInt32BufferUniform(name, value);
	return result;
}
/**
 * Add a new Int64 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addInt64Uniform(std::string name, int64_t value)
{
	bool result = _uniformBuffer->AddInt64BufferUniform(name, value);
	return result;
}

/**
 * Add a new Float to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addFloatUniform(std::string name, float value)
{
	bool result = _uniformBuffer->AddFloatBufferUniform(name, value);
	return result;
}
/**
 * Add a new Double to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addDoubleUniform(std::string name, double value)
{
	bool result = _uniformBuffer->AddDoubleBufferUniform(name, value);
	return result;
}

/**
 * Add a new Vec2 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addVec2Uniform(std::string name, Eigen::Vector2f value)
{
	bool result = _uniformBuffer->AddVec2BufferUniform(name, value);
	return result;
}
/**
 * Add a new Vec3 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addVec3Uniform(std::string name, Eigen::Vector3f value)
{
	bool result = _uniformBuffer->AddVec3BufferUniform(name, value);
	return result;
}
/**
 * Add a new Vec4 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addVec4Uniform(std::string name, Eigen::Vector4f value)
{
	bool result = _uniformBuffer->AddVec4BufferUniform(name, value);
	return result;
}

/**
 * Add a new UVec2 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value)
{
	bool result = _uniformBuffer->AddUVec2BufferUniform(name, value);
	return result;
}
/**
 * Add a new UVec3 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value)
{
	bool result = _uniformBuffer->AddUVec3BufferUniform(name, value);
	return result;
}
/**
 * Add a new UVec4 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value)
{
	bool result = _uniformBuffer->AddUVec4BufferUniform(name, value);
	return result;
}

// Uniform Getters
/**
 * Get UInt08 Value from Uniform Buffer.
 * 
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<uint8_t> Material::GetUInt08Uniform(std::string name)
{
	auto value = _uniformBuffer->GetUInt08BufferUniform(name);
	return value;
}
/**
 * Get UInt16 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<uint16_t> Material::GetUInt16Uniform(std::string name)
{
	auto value = _uniformBuffer->GetUInt16BufferUniform(name);
	return value;
}
/**
 * Get UInt32 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<uint32_t> Material::GetUInt32Uniform(std::string name)
{
	auto value = _uniformBuffer->GetUInt32BufferUniform(name);
	return value;
}
/**
 * Get UInt64 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<uint64_t> Material::GetUInt64Uniform(std::string name)
{
	auto value = _uniformBuffer->GetUInt64BufferUniform(name);
	return value;
}

/**
 * Get Int08 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<int8_t> Material::GetInt08Uniform(std::string name)
{
	auto value = _uniformBuffer->GetInt08BufferUniform(name);
	return value;
}
/**
 * Get Int16 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<int16_t> Material::GetInt16Uniform(std::string name)
{
	auto value = _uniformBuffer->GetInt16BufferUniform(name);
	return value;
}
/**
 * Get UInt32 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<int32_t> Material::GetInt32Uniform(std::string name)
{
	auto value = _uniformBuffer->GetInt32BufferUniform(name);
	return value;
}
/**
 * Get Int64 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<int64_t> Material::GetInt64Uniform(std::string name)
{
	auto value = _uniformBuffer->GetInt64BufferUniform(name);
	return value;
}

/**
 * Get Float Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<float> Material::GetFloatUniform(std::string name)
{
	auto value = _uniformBuffer->GetFloatBufferUniform(name);
	return value;
}
/**
 * Get Double Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<double> Material::GetDoubleUniform(std::string name)
{
	auto value = _uniformBuffer->GetDoubleBufferUniform(name);
	return value;
}

/**
 * Get Vec2 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<Eigen::Vector2f> Material::GetVec2Uniform(std::string name)
{
	auto value = _uniformBuffer->GetVec2BufferUniform(name);
	return value;
}
/**
 * Get Vec3 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<Eigen::Vector3f> Material::GetVec3Uniform(std::string name)
{
	auto value = _uniformBuffer->GetVec3BufferUniform(name);
	return value;
}
/**
 * Get Vec4 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<Eigen::Vector4f> Material::GetVec4Uniform(std::string name)
{
	auto value = _uniformBuffer->GetVec4BufferUniform(name);
	return value;
}

/**
 * Get UVec2 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<Eigen::Vector2<uint32_t>> Material::GetUVec2Uniform(std::string name)
{
	auto value = _uniformBuffer->GetUVec2BufferUniform(name);
	return value;
}
/**
 * Get UVec3 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<Eigen::Vector3<uint32_t>> Material::GetUVec3Uniform(std::string name)
{
	auto value = _uniformBuffer->GetUVec3BufferUniform(name);
	return value;
}
/**
 * Get UVec4 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<Eigen::Vector4<uint32_t>> Material::GetUVec4Uniform(std::string name)
{
	auto value = _uniformBuffer->GetUVec4BufferUniform(name);
	return value;
}

/**
 * Get Reference to Material Texture.
 * 
 * \param slot: The Texture's Slot
 * \return The Texture's Reference
 */
Texture* Material::GetTexture(GLuint slot)
{
	// Ensure Array Access is in Bounds
	if (slot >= 32)
		return nullptr;

	return _materialTextures[slot];
}

// Uniform Setters
/**
 * Change Value of UInt08 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt08Uniform(std::string name, uint8_t value)
{
	bool result = _uniformBuffer->SetUInt08BufferUniform(name, value);
	return result;
}
/**
 * Change Value of UInt16 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt16Uniform(std::string name, uint16_t value)
{
	bool result = _uniformBuffer->SetUInt16BufferUniform(name, value);
	return result;
}
/**
 * Change Value of UInt32 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt32Uniform(std::string name, uint32_t value)
{
	bool result = _uniformBuffer->SetUInt32BufferUniform(name, value);
	return result;
}
/**
 * Change Value of UInt64 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt64Uniform(std::string name, uint64_t value)
{
	bool result = _uniformBuffer->SetUInt64BufferUniform(name, value);
	return result;
}

/**
 * Change Value of Int08 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt08Uniform(std::string name, int8_t value)
{
	bool result = _uniformBuffer->SetInt08BufferUniform(name, value);
	return result;
}
/**
 * Change Value of Int16 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt16Uniform(std::string name, int16_t value)
{
	bool result = _uniformBuffer->SetInt16BufferUniform(name, value);
	return result;
}
/**
 * Change Value of Int32 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt32Uniform(std::string name, int32_t value)
{
	bool result = _uniformBuffer->SetInt32BufferUniform(name, value);
	return result;
}
/**
 * Change Value of Int64 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt64Uniform(std::string name, int64_t value)
{
	bool result = _uniformBuffer->SetInt64BufferUniform(name, value);
	return result;
}

/**
 * Change Value of Float in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetFloatUniform(std::string name, float value)
{
	bool result = _uniformBuffer->SetFloatBufferUniform(name, value);
	return result;
}
/**
 * Change Value of Double in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetDoubleUniform(std::string name, double value)
{
	bool result = _uniformBuffer->SetDoubleBufferUniform(name, value);
	return result;
}

/**
 * Change Value of Vec2 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetVec2Uniform(std::string name, Eigen::Vector2f value)
{
	bool result = _uniformBuffer->SetVec2BufferUniform(name, value);
	return result;
}
/**
 * Change Value of Vec3 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetVec3Uniform(std::string name, Eigen::Vector3f value)
{
	bool result = _uniformBuffer->SetVec3BufferUniform(name, value);
	return result;
}
/**
 * Change Value of Vec4 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetVec4Uniform(std::string name, Eigen::Vector4f value)
{
	bool result = _uniformBuffer->SetVec4BufferUniform(name, value);
	return result;
}

/**
 * Change Value of UVec2 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value)
{
	bool result = _uniformBuffer->SetUVec2BufferUniform(name, value);
	return result;
}
/**
 * Change Value of UVec3 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value)
{
	bool result = _uniformBuffer->SetUVec3BufferUniform(name, value);
	return result;
}
/**
 * Change Value of UVec4 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value)
{
	bool result = _uniformBuffer->SetUVec4BufferUniform(name, value);
	return result;
}

/**
 * Set Reference to Material Texture.
 * 
 * \param slot: The Texture's Slot
 * \param texture: The Texture's Reference
 */
void Material::SetTexture(GLuint slot, Texture* texture)
{
	_materialTextures[slot] = texture;
}

/**
 * Bind the Material's Uniform Buffer.
 * 
 * \return Whether the Uniform Buffer was Successfully Bounded
 */
bool Material::bindUniformBuffer()
{
	if (!_uniformBuffer->IsCreated())
	{// Create Uniform Buffer Object
		if (!_uniformBuffer->CreateUniformBufferObject())
			return false;
	}

	// Bind Uniform Buffer to Slot 1
	_uniformBuffer->BindUniformBufferObject(1);
	return true;
}
/**
 * Bind the Material's Textures.
 * 
 */
void Material::bindTextures()
{
	for (int i = 0; i < 32; i++) {
	if (_materialTextures[i]) {
		GLuint textureSlotID = GL_TEXTURE0 + i;
		_materialTextures[i]->BindTexture(textureSlotID);
	}}
}

/**
 * Bind Material's Parameters to the Shader.
 * 
 * \return Whether the Material Parameters were Successfully Bounded
 */
bool Material::UseMaterial()
{
	if (!bindUniformBuffer())
	{// Uniform Buffer Could not be Bound
		return false;
	}

	bindTextures();

	return true;
}
/**
 * Reset the Material's Uniform Buffer and Textures.
 * 
 */
void Material::ResetMaterial()
{
	// Delete Material's Uniform Buffer Object
	_uniformBuffer->DeleteUniformBufferObject();

	// Clear Reference to all Textures
	for (int i = 0; i < 32; i++)
	{
		_materialTextures[i] = nullptr;
	}
}
}