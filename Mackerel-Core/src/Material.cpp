#include "Material.h"

#include "UniformBuffer.h"
#include "Texture.h"

#include <iostream>

namespace MCK::AssetType {
Material::Material()
{
	// Initialise Uniform Buffer
	m_UniformBuffer = new UniformBuffer();

	// Initialise Material Textures References
	for (int i = 0; i < 32; i++)
	{
		m_MaterialTextures[i] = nullptr;
	}
}
Material::~Material()
{
	// Delete Uniform Buffer Object
	if (m_UniformBuffer)
		delete m_UniformBuffer;
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
	bool result = m_UniformBuffer->AddUInt08BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddUInt16BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddUInt32BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddUInt64BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddInt08BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddInt16BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddInt32BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddInt64BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddFloatBufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddDoubleBufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddVec2BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddVec3BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddVec4BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddUVec2BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddUVec3BufferUniform(name, value);
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
	bool result = m_UniformBuffer->AddUVec4BufferUniform(name, value);
	return result;
}

/**
 * Add a new Mat4 to the Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::addMat4Uniform(std::string name, Eigen::Matrix4f value)
{
	bool result = m_UniformBuffer->AddMat4BufferUniform(name, value);
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
	auto value = m_UniformBuffer->GetUInt08BufferUniform(name);
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
	auto value = m_UniformBuffer->GetUInt16BufferUniform(name);
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
	auto value = m_UniformBuffer->GetUInt32BufferUniform(name);
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
	auto value = m_UniformBuffer->GetUInt64BufferUniform(name);
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
	auto value = m_UniformBuffer->GetInt08BufferUniform(name);
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
	auto value = m_UniformBuffer->GetInt16BufferUniform(name);
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
	auto value = m_UniformBuffer->GetInt32BufferUniform(name);
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
	auto value = m_UniformBuffer->GetInt64BufferUniform(name);
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
	auto value = m_UniformBuffer->GetFloatBufferUniform(name);
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
	auto value = m_UniformBuffer->GetDoubleBufferUniform(name);
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
	auto value = m_UniformBuffer->GetVec2BufferUniform(name);
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
	auto value = m_UniformBuffer->GetVec3BufferUniform(name);
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
	auto value = m_UniformBuffer->GetVec4BufferUniform(name);
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
	auto value = m_UniformBuffer->GetUVec2BufferUniform(name);
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
	auto value = m_UniformBuffer->GetUVec3BufferUniform(name);
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
	auto value = m_UniformBuffer->GetUVec4BufferUniform(name);
	return value;
}

/**
 * Get Mat4 Value from Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \return The Uniform's Value, if Existent
 */
std::optional<Eigen::Matrix4f> Material::GetMat4Uniform(std::string name)
{
	auto value = m_UniformBuffer->GetMat4BufferUniform(name);
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

	return m_MaterialTextures[slot];
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
	bool result = m_UniformBuffer->SetUInt08BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetUInt16BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetUInt32BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetUInt64BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetInt08BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetInt16BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetInt32BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetInt64BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetFloatBufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetDoubleBufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetVec2BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetVec3BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetVec4BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetUVec2BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetUVec3BufferUniform(name, value);
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
	bool result = m_UniformBuffer->SetUVec4BufferUniform(name, value);
	return result;
}

/**
 * Change Value of Mat4 in Uniform Buffer.
 *
 * \param name: The Uniform's Name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetMat4Uniform(std::string name, Eigen::Matrix4f value)
{
	bool result = m_UniformBuffer->SetMat4BufferUniform(name, value);
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
	m_MaterialTextures[slot] = texture;
}

/**
 * Bind Material's Parameters to the Shader.
 * 
 * \return Whether the Material Parameters were Successfully Bounded
 */
bool Material::UseMaterial()
{
	// Create Uniform Buffer Object if Non-Existant
	if (!m_UniformBuffer->IsCreated()) {
	if (!m_UniformBuffer->CreateUniformBufferObject())
		return false;
	}

	// Bind Uniform Buffer to Slot 2
	m_UniformBuffer->BindUniformBufferObject(2);

	// Bind Material Textures to their Respective Slots
	for (int i = 0; i < 32; i++) {
	if (m_MaterialTextures[i])
		m_MaterialTextures[i]->BindTexture(GL_TEXTURE0 + i);
	}

	return true;
}
/**
 * Reset the Material's Uniform Buffer and Textures.
 * 
 */
void Material::ResetMaterial()
{
	// Delete Material's Uniform Buffer Object
	m_UniformBuffer->DeleteUniformBufferObject();

	// Clear Reference to all Textures
	for (int i = 0; i < 32; i++)
	{
		m_MaterialTextures[i] = nullptr;
	}
}

bool Material::LoadFromFile(std::string a_FilePath, int materialIndex)
{
	std::ifstream file(a_FilePath);

	if (!file.is_open())
	{
		MCK::Logger::log(std::format("Failed to load material from file: ", a_FilePath), MCK::Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	std::string line;
	int currentMaterialIndex = -1;
	GLuint textureSlot = 0;

	while (std::getline(file, line))
	{
		std::istringstream lineStream(line);
		std::string identifier;
		lineStream >> identifier;

		if (identifier == "newmtl")
		{
			currentMaterialIndex++;

			if (currentMaterialIndex != materialIndex)
			{
				continue;
			}
		}

		// If we're not at the desired material, skip processing the rest of the loop
		if (currentMaterialIndex != materialIndex)
		{
			continue;
		}

		if (identifier == "Kd")
		{
			Eigen::Vector3f colour;
			lineStream >> colour.x() >> colour.y() >> colour.z();

			this->addVec3Uniform("diffuseColour", colour);
		}
		else if (identifier == "Ka")
		{
			Eigen::Vector3f colour;
			lineStream >> colour.x() >> colour.y() >> colour.z();

			this->addVec3Uniform("ambientColour", colour);
		}
		else if (identifier == "Ks")
		{
			Eigen::Vector3f colour;
			lineStream >> colour.x() >> colour.y() >> colour.z();

			this->addVec3Uniform("specularColour", colour);
		}
		else if (identifier == "map_Kd" || identifier == "map_Ks" || identifier == "map_Bump" || identifier == "bump")
		{
			std::string texturePath;
			lineStream >> texturePath;

			if (textureSlot < 32)
			{
				AssetType::Texture* texture = new AssetType::Texture();
				texture->LoadFromFile(texturePath, false);
				this->SetTexture(textureSlot++, texture);
			}
			else
				MCK::Logger::log(std::format("Material has too many textures: ", a_FilePath), MCK::Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		}
	}

	file.close();
	return true;
}



}