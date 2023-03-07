#include "FrameBuffer.h"

#include "Texture.h"

namespace MCK {
FrameBuffer::FrameBuffer() :
	_frameBufferObject(0), _isCreated(0), _depthBufferTexture(nullptr) {}
FrameBuffer::~FrameBuffer() {}

bool FrameBuffer::CreateFrameBuffer()
{
	// Ensure FBO is not Already Created
	if (_isCreated)
		return false;

	// Generate the Frame Buffer
	glGenBuffers(1, &_frameBufferObject);

	// Add Depth Attachemnt Texture
	if (!_depthBufferTexture)
	{// Ensure Depth Attachment Texture isn't nullptr
		// Write Error Message
		return false;
	}
	glFramebufferTexture2D(_frameBufferObject, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBufferTexture->getTextureID(), 0);

	// Add Colour Attachment Textures
	std::vector<GLuint> colourAttachmentSlots;
	for (int i = 0; i < colourAttachmentSlots.size(); i++)
	{
		auto colourAttachmentTexture = _colourAttachmentTextures[i];
		if (!colourAttachmentTexture)
		{// Ensure Colour Attachment Texture isn't nullptr
			// Write Error Message
			return false;
		}

		GLuint colourAttachmentSlot = GL_COLOR_ATTACHMENT0 + i;

		glFramebufferTexture2D(_frameBufferObject, colourAttachmentSlot, GL_TEXTURE_2D, colourAttachmentTexture->getTextureID(), 0);
		colourAttachmentSlots.push_back(colourAttachmentSlot);
	}
	glDrawBuffers((GLuint)colourAttachmentSlots.size(), &colourAttachmentSlots[0]);

	_isCreated = true;
	return true;
}

bool FrameBuffer::UseFrameBufferObject(Eigen::Vector4f clearColour, GLuint clearFlags)
{
	// Bind the Frame Buffer
	glBindBuffer(GL_FRAMEBUFFER, _frameBufferObject);

	// Clear the Frame Buffer
	glClearColor(clearColour.x(), clearColour.y(), clearColour.z(), clearColour.w());
	glClear(clearFlags);

	return true;
}

bool FrameBuffer::AddFloatColourAttachment(GLuint width, GLuint height)
{
	if (_colourAttachmentTextures.size() > 32)
	{// Frame Buffers cannot have more than 32 Colour Attachments
		return false;
	}

	// Create, Generate, and Add Float Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	colourAttachemntTexture->GenerateFloatTexture(width, height);

	_colourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddIntColourAttachment(GLuint width, GLuint height)
{
	if (_colourAttachmentTextures.size() > 32)
	{// Frame Buffers cannot have more than 32 Colour Attachments
		return false;
	}

	// Create, Generate, and Add Int Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	colourAttachemntTexture->GenerateIntTexture(width, height);

	_colourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddUIntColourAttachment(GLuint width, GLuint height)
{
	if (_colourAttachmentTextures.size() > 32)
	{// Frame Buffers cannot have more than 32 Colour Attachments
		return false;
	}

	// Create, Generate, and Add UInt Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	colourAttachemntTexture->GenerateUIntTexture(width, height);

	_colourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}

void FrameBuffer::AssignDepthBufferTexture(AssetType::Texture* depthBufferTexture)
{
	_depthBufferTexture = depthBufferTexture;
}
}
