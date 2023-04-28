#include "Texture.h"

#include <iostream>
#include <vector>

namespace MCK::AssetType {
bool Texture::GenerateFloatTexture(GLuint width, GLuint height)
{
	m_Format = GL_RGBA16F;
	m_TextureType = GL_RGBA;
	m_VariableType = GL_FLOAT;

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_Format, width, height, 0, m_TextureType, m_VariableType, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
bool Texture::GenerateIntTexture(GLuint width, GLuint height)
{
	m_Format = GL_R32I;
	m_TextureType = GL_RED_INTEGER;
	m_VariableType = GL_INT;

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_Format, width, height, 0, m_TextureType, m_VariableType, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
bool Texture::GenerateUIntTexture(GLuint width, GLuint height)
{
	m_Format = GL_R32UI;
	m_TextureType = GL_RED_INTEGER;
	m_VariableType = GL_UNSIGNED_INT;

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_Format, width, height, 0, m_TextureType, m_VariableType, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}
bool Texture::GenerateDepthTexture(GLuint a_Width, GLuint a_Height)
{
	m_Format = GL_DEPTH_COMPONENT32F;
	m_TextureType = GL_DEPTH_COMPONENT;
	m_VariableType = GL_FLOAT;

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_Format, a_Width, a_Height, 0, m_TextureType, m_VariableType, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool Texture::ResizeTexture(GLuint width, GLuint height)
{
	glBindTexture(GL_TEXTURE_2D, _textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_Format, width, height, 0, m_TextureType, m_VariableType, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	return true;
}
bool Texture::ClearTexture()
{
	return true;
}

bool Texture::BindTexture(GLuint a_TextureUnit)
{
	if (a_TextureUnit >= 32)
	{// Only 32 Texture Units Exist in OpenGL
		std::cout << "ERROR: Cannot Bind Texture to Unit 32 or Higher" << std::endl;

		return false;
	}

	GLuint textureUnitID = GL_TEXTURE0 + a_TextureUnit;
	glActiveTexture(textureUnitID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	return true;
}
}
