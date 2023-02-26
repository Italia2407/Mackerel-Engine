#include "Texture.h"

namespace MCK::AssetType {
bool Texture::GenerateFloatTexture(GLuint width, GLuint height)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return true;
}
bool Texture::GenerateIntTexture(GLuint width, GLuint height)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16I, width, height, 0, GL_RGBA, GL_INT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return true;
}
bool Texture::GenerateUIntTexture(GLuint width, GLuint height)
{
	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16UI, width, height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return true;
}

bool Texture::ClearTexture()
{
	return true;
}

bool Texture::BindTexture(GLuint textureSlot)
{
	if (textureSlot >= 32)
	{// Only 32 Texture Slots Exist in OpenGL
		return false;
	}

	GLuint textureSlotID = GL_TEXTURE0 + textureSlot;
	glBindTexture(textureSlotID, _textureID);

	return true;
}
}
