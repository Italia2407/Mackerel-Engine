#pragma once

#include <glad/glad.h>

#include <map>
#include <optional>

namespace MCK::AssetType {
class Shader
{

private:
	GLuint _programID;

public:
	GLuint getProgramID() { return _programID; }
};
}