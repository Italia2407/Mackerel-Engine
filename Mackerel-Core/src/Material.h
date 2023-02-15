#pragma once

#include "Shader.h"
#include "Texture.h"

#include "UniformData.h"

#include <string>
#include <list>

namespace MCK::AssetType {
class Material
{
	Material();
	~Material();

private:
	Shader* _shader;

	std::list<UniformData*> _uniforms;
};
}