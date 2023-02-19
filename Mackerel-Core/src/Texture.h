#pragma once

#include <glad/glad.h>

namespace MCK::AssetType {
class Texture
{

private:
	GLuint _textureID;

public:
	GLuint getTextureID() { return _textureID; }

	bool GenerateTexture(GLuint width, GLuint height);
	bool ClearTexture();
};
}