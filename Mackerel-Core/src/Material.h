#pragma once

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

#include <string>
#include <map>

namespace MCK {
struct UniformData;
}

namespace MCK::AssetType {
class Material
{
public:
	Material(Shader* shader);
	~Material();

	struct UniformState
	{
		GLuint texSlots = 0;
	};

private:
	Shader* _shader;

	UniformState _uniformState;
	std::map<std::string, UniformData*> _uniforms;

	UniformData* getUniform(std::string name);

public:
	Shader* shader() { return _shader; }

	bool AddFloatUniform(std::string name, float value);
	bool SetFloatUniform(std::string name, float value);
	bool AddDoubleUniform(std::string name, double value);
	bool SetDoubleUniform(std::string name, double value);

	bool AddTextureUniform(std::string name, AssetType::Texture* texture);
	bool SetTextureUniform(std::string name, AssetType::Texture* texture);

	bool BindUniforms();
	void ResetUniforms();
};
}