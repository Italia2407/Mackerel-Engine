#pragma once

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

#include <string>
#include <list>

struct UniformData;

namespace MCK::AssetType {
class Material
{
public:
	Material();
	~Material();

	struct UniformState
	{
		GLuint texSlots;
	};

private:
	Shader* _shader;

	UniformState _uniformState;
	std::list<UniformData*> _uniforms;

};
}