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