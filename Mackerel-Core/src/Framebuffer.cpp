#include "FrameBuffer.h"

#include "Texture.h"

namespace MCK {
FrameBuffer::FrameBuffer() :
	m_FrameBufferObject(GL_ZERO), m_IsCreated(false), m_ExternalDepthBufferTexture(false), m_DepthBufferTexture(nullptr) {}
FrameBuffer::~FrameBuffer()
{
	// Delete Depth Buffer Texture, Only if Not Assigned from External Source
	if (m_DepthBufferTexture && !m_ExternalDepthBufferTexture)
	{
		delete m_DepthBufferTexture;
	}
}

bool FrameBuffer::CreateFrameBuffer()
{
	// Ensure FBO is not Already Created
	if (m_IsCreated)
	{
		return false;
	}

	// Generate the Frame Buffer
	glGenBuffers(1, &m_FrameBufferObject);

	// Add Depth Attachemnt Texture
	if (!m_DepthBufferTexture)
	{// Framebuffer needs a Depth Buffer Texture
		return false;
	}
	glFramebufferTexture2D(m_FrameBufferObject, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferTexture->getTextureID(), 0);

	// Add Colour Attachment Textures
	std::vector<GLuint> colourAttachmentSlots;
	for (int i = 0; i < colourAttachmentSlots.size(); i++)
	{
		auto colourAttachmentTexture = m_ColourAttachmentTextures[i];
		if (!colourAttachmentTexture)
		{// Ensure Colour Attachment Texture isn't nullptr
			// Write Error Message
			return false;
		}

		GLuint colourAttachmentSlot = GL_COLOR_ATTACHMENT0 + i;

		glFramebufferTexture2D(m_FrameBufferObject, colourAttachmentSlot, GL_TEXTURE_2D, colourAttachmentTexture->getTextureID(), 0);
		colourAttachmentSlots.push_back(colourAttachmentSlot);
	}
	glDrawBuffers((GLuint)colourAttachmentSlots.size(), &colourAttachmentSlots[0]);

	m_IsCreated = true;
	return true;
}

bool FrameBuffer::UseFrameBufferObject(Eigen::Vector4f clearColour, GLuint clearFlags)
{
	// Bind the Frame Buffer
	glBindBuffer(GL_FRAMEBUFFER, m_FrameBufferObject);

	// Clear the Frame Buffer
	glClearColor(clearColour.x(), clearColour.y(), clearColour.z(), clearColour.w());
	glClear(clearFlags);

	return true;
}

bool FrameBuffer::AddFloatColourAttachment(GLuint width, GLuint height)
{
	if (m_ColourAttachmentTextures.size() > 32)
	{// Frame Buffers cannot have more than 32 Colour Attachments
		return false;
	}

	// Create, Generate, and Add Float Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	colourAttachemntTexture->GenerateFloatTexture(width, height);

	m_ColourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddIntColourAttachment(GLuint width, GLuint height)
{
	if (m_ColourAttachmentTextures.size() > 32)
	{// Frame Buffers cannot have more than 32 Colour Attachments
		return false;
	}

	// Create, Generate, and Add Int Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	colourAttachemntTexture->GenerateIntTexture(width, height);

	m_ColourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddUIntColourAttachment(GLuint width, GLuint height)
{
	if (m_ColourAttachmentTextures.size() > 32)
	{// Frame Buffers cannot have more than 32 Colour Attachments
		return false;
	}

	// Create, Generate, and Add UInt Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	colourAttachemntTexture->GenerateUIntTexture(width, height);

	m_ColourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddDepthBufferTexture(GLuint a_Width, GLuint a_Height)
{
	// Check if Depth Buffer Wasn't Assigned Already
	if (m_DepthBufferTexture)
	{
		return false;
	}

	// Create, Generate, and Add Depth Buffer Texture
	m_DepthBufferTexture = new AssetType::Texture();
	m_DepthBufferTexture->GenerateFloatTexture(a_Width, a_Height);

	return true;
}

bool FrameBuffer::AssignExternalDepthBufferTexture(AssetType::Texture* depthBufferTexture)
{
	// Check if Depth Buffer Wasn't Assigned Already
	if (m_DepthBufferTexture)
	{
		return false;
	}

	m_ExternalDepthBufferTexture = true;
	m_DepthBufferTexture = depthBufferTexture;

	return true;
}
}
