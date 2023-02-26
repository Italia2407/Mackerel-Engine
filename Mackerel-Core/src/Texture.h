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

	bool ClearTexture();

	bool BindTexture(GLuint textureSlotID);
};
}