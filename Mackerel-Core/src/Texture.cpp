#include "Texture.h"

#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

bool Texture::LoadFromFile(std::string a_FilePath, bool isFor2DObject)
{
	int width, height, channels;

	// Load image with stb_image
	unsigned char* data = stbi_load(a_FilePath.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		// Detrmine format of the image
		GLenum format = GL_RGB;
		if (channels == 1)
			format = GL_RED;
		else if (channels == 3)
			format = GL_RGB;
		else if (channels == 4)
			format = GL_RGBA;

		// Generete and bind the OpenGL texture
		glGenTextures(1, &_textureID);
		glBindTexture(GL_TEXTURE_2D, _textureID);

		// Set  tex parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		if (isFor2DObject)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		}
		
		// Load texture data into OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Free image data
		stbi_image_free(data);

		return true;
	}
	else
	{
		// Failed to load image
		const char* failure_reason = stbi_failure_reason();
		MCK::Logger::log(std::format("Failed to load image: ", failure_reason, ". Path: ",  a_FilePath), MCK::Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		stbi_image_free(data);
		return false;
	}
}
}
