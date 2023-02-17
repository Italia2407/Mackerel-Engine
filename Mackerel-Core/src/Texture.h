#pragma once

#include <glad/glad.h>

namespace MCK::AssetType {
class Texture
{

private:
	GLuint _textureID;

public:
	GLuint getTextureID() { return _textureID; }

	bool GenerateTexture(GLuint width, GLuint height)
	{
		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_2D, _textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
};
}