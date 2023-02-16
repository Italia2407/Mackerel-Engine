#include "Material.h"

#include "UniformData.h"

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

	FloatUniform* floatUniform = dynamic_cast<FloatUniform*>(_uniforms[name]);
	if (!floatUniform)
	{// Uniform isn't float Type
		return false;
	}

	floatUniform->value = value;

	return true;
}

/**
 * Bind all the Material's Uniforms to their Shader.
 * 
 */
void Material::BindUniforms()
{
	for (auto [uniformName, uniform] : _uniforms)
	{
		uniform->Bind(_uniformState);
	}
}
}