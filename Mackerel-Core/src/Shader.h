#pragma once

namespace MCK::AssetType {
class Shader
{

private:
	GLuint _programID;

public:
	GLuint getProgramID() { return _programID; }
};
}