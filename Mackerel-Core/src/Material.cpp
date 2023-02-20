#include "Material.h"

#include "UniformData.h"

#include <iostream>

namespace MCK::AssetType {
Material::Material(Shader* shader) :
	_shader(shader), _uniformState() {}

/**
 * Retrieve Uniform from Material
 * 
 * \param name: The Uniform's Name
 * \return Pointer to Uniform
 */
UniformData* Material::getUniform(std::string name)
{
	if (!_uniforms.contains(name))
	{// No Uniform Found with the Name
		return nullptr;
	}

	return _uniforms[name];
}

/**
 * Create a new UInt08 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddUInt08Uniform(std::string name, uint8_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	UInt08Uniform* newUniform = new UInt08Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of UInt08 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt08Uniform(std::string name, uint8_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	UInt08Uniform* uniform = dynamic_cast<UInt08Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't uint08 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new UInt16 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddUInt16Uniform(std::string name, uint16_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	UInt16Uniform* newUniform = new UInt16Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of UInt16 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt16Uniform(std::string name, uint16_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	UInt16Uniform* uniform = dynamic_cast<UInt16Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't uint16 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new UInt32 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddUInt32Uniform(std::string name, uint32_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	UInt32Uniform* newUniform = new UInt32Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of UInt32 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt32Uniform(std::string name, uint32_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	UInt32Uniform* uniform = dynamic_cast<UInt32Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't uint32 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new UInt64 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddUInt64Uniform(std::string name, uint64_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	UInt64Uniform* newUniform = new UInt64Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of UInt64 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUInt64Uniform(std::string name, uint64_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	UInt64Uniform* uniform = dynamic_cast<UInt64Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't uint64 Type
		return false;
	}

	uniform->value = value;

	return true;
}

/**
 * Create a new Int08 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddInt08Uniform(std::string name, int8_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	Int08Uniform* newUniform = new Int08Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Int08 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt08Uniform(std::string name, int8_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	Int08Uniform* uniform = dynamic_cast<Int08Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't int08 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new Int16 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddInt16Uniform(std::string name, int16_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	Int16Uniform* newUniform = new Int16Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Int16 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt16Uniform(std::string name, int16_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	Int16Uniform* uniform = dynamic_cast<Int16Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't int16 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new Int32 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddInt32Uniform(std::string name, int32_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	Int32Uniform* newUniform = new Int32Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Int32 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt32Uniform(std::string name, int32_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	Int32Uniform* uniform = dynamic_cast<Int32Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't int32 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new Int64 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddInt64Uniform(std::string name, int64_t value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	Int64Uniform* newUniform = new Int64Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Int64 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetInt64Uniform(std::string name, int64_t value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	Int64Uniform* uniform = dynamic_cast<Int64Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't int64 Type
		return false;
	}

	uniform->value = value;

	return true;
}

/**
 * Create a new Float Uniform.
 * 
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddFloatUniform(std::string name, float value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	FloatUniform* newUniform = new FloatUniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Float Uniform.
 * 
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetFloatUniform(std::string name, float value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	FloatUniform* uniform = dynamic_cast<FloatUniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't float Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new Double Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddDoubleUniform(std::string name, double value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	DoubleUniform* newUniform = new DoubleUniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Double Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetDoubleUniform(std::string name, double value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	DoubleUniform* uniform = dynamic_cast<DoubleUniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't double Type
		return false;
	}

	uniform->value = value;

	return true;
}

/**
 * Create a new Vec2 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddVec2Uniform(std::string name, Eigen::Vector2f value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	Vec2Uniform* newUniform = new Vec2Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Vec2 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetVec2Uniform(std::string name, Eigen::Vector2f value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	Vec2Uniform* uniform = dynamic_cast<Vec2Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't vec2 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new Vec3 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddVec3Uniform(std::string name, Eigen::Vector3f value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	Vec3Uniform* newUniform = new Vec3Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Vec3 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetVec3Uniform(std::string name, Eigen::Vector3f value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	Vec3Uniform* uniform = dynamic_cast<Vec3Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't vec3 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new Vec4 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddVec4Uniform(std::string name, Eigen::Vector4f value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	Vec4Uniform* newUniform = new Vec4Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of Vec4 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetVec4Uniform(std::string name, Eigen::Vector4f value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	Vec4Uniform* uniform = dynamic_cast<Vec4Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't vec4 Type
		return false;
	}

	uniform->value = value;

	return true;
}

/**
 * Create a new UVec2 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	UVec2Uniform* newUniform = new UVec2Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of UVec2 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUVec2Uniform(std::string name, Eigen::Vector2<uint32_t> value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	UVec2Uniform* uniform = dynamic_cast<UVec2Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't uvec2 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new UVec3 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	UVec3Uniform* newUniform = new UVec3Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of UVec3 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUVec3Uniform(std::string name, Eigen::Vector3<uint32_t> value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	UVec3Uniform* uniform = dynamic_cast<UVec3Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't uvec3 Type
		return false;
	}

	uniform->value = value;

	return true;
}
/**
 * Create a new UVec4 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's Value
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	UVec4Uniform* newUniform = new UVec4Uniform(_shader->getProgramID(), name, value);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Value of UVec4 Uniform.
 *
 * \param name: The Uniform's name
 * \param value: The Uniform's new Value
 * \return Whether the Uniform's Value was Successfully Changed
 */
bool Material::SetUVec4Uniform(std::string name, Eigen::Vector4<uint32_t> value)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	UVec4Uniform* uniform = dynamic_cast<UVec4Uniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't uvec4 Type
		return false;
	}

	uniform->value = value;

	return true;
}

/**
 * Create a new Texture Uniform.
 *
 * \param name: The Uniform's name
 * \param texture: The Uniform's Texture Reference
 * \return Whether the Uniform was Successfully Added
 */
bool Material::AddTextureUniform(std::string name, AssetType::Texture* texture)
{
	if (_uniforms.contains(name))
	{// Uniform already Exists with the Current Name
		return false;
	}

	TextureUniform* newUniform = new TextureUniform(_shader->getProgramID(), name, texture);
	_uniforms[name] = newUniform;

	return true;
}
/**
 * Change Reference of Texture Uniform.
 *
 * \param name: The Uniform's name
 * \param texture: The Uniform's new Reference
 * \return Whether the Uniform's Reference was Successfully Changed
 */
bool Material::SetTextureUniform(std::string name, AssetType::Texture* texture)
{
	if (!_uniforms.contains(name))
	{// No Uniform Exists with the Name
		return false;
	}

	TextureUniform* uniform = dynamic_cast<TextureUniform*>(_uniforms[name]);
	if (!uniform)
	{// Uniform isn't Texture Type
		return false;
	}

	uniform->texture = texture;

	return true;
}

/**
 * Bind all the Material's Uniforms to their Shader.
 * 
 * \return Whether the Uniforms were successfully bounded
 */
bool Material::BindUniforms()
{
	for (auto [uniformName, uniform] : _uniforms)
	{
		if (!uniform->Bind(_uniformState))
		{// Uniform could not be bound
			std::cerr << "ERROR: Could not Bind Uniform " << uniformName << std::endl;
			return false;
		}
	}

	return true;
}
/**  */
void Material::ResetUniforms()
{
	// Reset Textures
	for (int i = _uniformState.texSlots - 1; i >= 0; i--)
	{
		GLuint texSlot = GL_TEXTURE0 + i;
		glBindTexture(texSlot, 0);
	}
	_uniformState.texSlots = 0;
}
}