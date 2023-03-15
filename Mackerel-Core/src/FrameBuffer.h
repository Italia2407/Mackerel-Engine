#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

#include <vector>

// Forward Declarations
namespace MCK::AssetType {
class Texture;
}

namespace MCK {
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

private:
	GLuint _frameBufferObject;
	bool _isCreated;

	/*GLuint _width;
	GLuint _height;*/

	std::vector<AssetType::Texture*> _colourAttachmentTextures;
	AssetType::Texture* _depthBufferTexture;

public:
	bool IsCreated() { return _isCreated; }

	GLuint GetNumColourAttachments() { return (GLuint)_colourAttachmentTextures.size(); }
	AssetType::Texture* GetColourAttachmentTexture(GLuint slot) { return _colourAttachmentTextures[slot]; }

public:
	bool CreateFrameBuffer();
	bool UseFrameBufferObject(Eigen::Vector4f clearColour, GLuint clearFlags);

	bool AddFloatColourAttachment(GLuint width, GLuint height);
	bool AddIntColourAttachment(GLuint width, GLuint height);
	bool AddUIntColourAttachment(GLuint width, GLuint height);

	void AssignDepthBufferTexture(AssetType::Texture* depthBufferTexture);
};
}
