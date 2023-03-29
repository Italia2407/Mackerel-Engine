#pragma once

#include <glad/glad.h>

namespace MCK::AssetType {
class Texture
{

private:
	GLuint _textureID;

public:
	GLuint getTextureID() { return _textureID; }

	bool GenerateFloatTexture(GLuint width, GLuint height);
	bool GenerateIntTexture(GLuint width, GLuint height);
	bool GenerateUIntTexture(GLuint width, GLuint height);
	bool GenerateDepthTexture(GLuint a_Width, GLuint a_Height);

	bool ClearTexture();

	bool BindTexture(GLuint textureSlotID);
};
}