#pragma once
#include <glad/glad.h>

#include <string>

namespace MCK::AssetType {
class Texture
{
private:
	GLuint _textureID;

	GLuint m_Format;
	GLuint m_TextureType;
	GLuint m_VariableType;

public:
	GLuint getTextureID() { return _textureID; }

	bool LoadFromFile(std::string a_FilePath);

	bool GenerateFloatTexture(GLuint width, GLuint height);
	bool GenerateIntTexture(GLuint width, GLuint height);
	bool GenerateUIntTexture(GLuint width, GLuint height);
	bool GenerateDepthTexture(GLuint a_Width, GLuint a_Height);

	bool ResizeTexture(GLuint width, GLuint height);
	bool ClearTexture();

	bool BindTexture(GLuint textureSlotID);
};
}