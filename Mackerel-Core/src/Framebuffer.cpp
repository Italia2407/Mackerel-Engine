#include "FrameBuffer.h"

#include "Texture.h"

#include <iostream>

namespace MCK {
FrameBuffer::FrameBuffer(std::string a_FrameBufferName) :
	m_FrameBufferObject(GL_ZERO), m_FrameBufferName(a_FrameBufferName), m_ExternalDepthBufferTexture(false), m_DepthBufferTexture(nullptr) {}
FrameBuffer::~FrameBuffer()
{
	// Delete the Framebuffer Object
	glDeleteFramebuffers(1, &m_FrameBufferObject);

	// Delete the Colour Attachment Textures
	for (auto colourAttachment : m_ColourAttachmentTextures) {
	if (colourAttachment)
	{
		delete colourAttachment;
	}}

	// Delete Depth Buffer Texture, Only if Not Assigned from External Source
	if (m_DepthBufferTexture && !m_ExternalDepthBufferTexture)
	{
		delete m_DepthBufferTexture;
	}
}

bool FrameBuffer::CreateFrameBuffer()
{
	// Ensure FBO is not Already Created
	if (m_FrameBufferObject != GL_ZERO) {
		std::cout << "ERROR: Cannot Recreate " << m_FrameBufferName << " FBO" << std::endl;
		return false;
	}
	// Ensure Depth Buffer Texture Exists
	if (!m_DepthBufferTexture) {
		std::cout << "ERROR: " << m_FrameBufferName << " Needs an Assigned Depth Texture" << std::endl;
		return false;
	}
	// Ensure at Least 1 Colour Attachment Exists
	if (m_ColourAttachmentTextures.size() < 1) {
		std::cout << "ERROR: " << m_FrameBufferName << " Needs at least 1 Assigned Colour Attachment Texture" << std::endl;
		return false;
	}

	// Generate the Frame Buffer
	glGenFramebuffers(1, &m_FrameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);

	// Add Colour Attachment Textures
	std::vector<GLuint> colourAttachmentSlots;
	for (int i = 0; i < m_ColourAttachmentTextures.size(); i++)
	{
		auto colourAttachmentTexture = m_ColourAttachmentTextures[i];

		if (!colourAttachmentTexture || colourAttachmentTexture->getTextureID() == GL_ZERO)
		{
			std::cout << "ERROR: Colour Attachment Texture #" << i << " is not Valid" << std::endl;

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDeleteFramebuffers(1, &m_FrameBufferObject);
			m_FrameBufferObject = GL_ZERO;

			return false;
		}

		GLuint colorAttachmentSlot = GL_COLOR_ATTACHMENT0 + i;

		glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachmentSlot, GL_TEXTURE_2D, colourAttachmentTexture->getTextureID(), 0);
		colourAttachmentSlots.push_back(colorAttachmentSlot);
	}

	// Add Depth Attachment Texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBufferTexture->getTextureID(), 0);

	glDrawBuffers((GLuint)colourAttachmentSlots.size(), colourAttachmentSlots.data());

	// Check if Framebuffer was Created Properly
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR: " << m_FrameBufferName << " FBO Could not be Properly Created" << std::endl;
		std::cout << "Obtained Status: " << fboStatus << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

bool FrameBuffer::UseFrameBufferObject(Eigen::Vector4f clearColour, GLuint clearFlags)
{
	// Ensure Framebuffer Object Exists
	if (m_FrameBufferObject == GL_ZERO) {
		std::cout << "ERROR: Cannot Use Unexistant Framebuffer Object" << std::endl;
		return false;
	}
	// Bind the Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferObject);

	// Clear the Frame Buffer
	glClearColor(clearColour.x(), clearColour.y(), clearColour.z(), clearColour.w());
	glClear(clearFlags);

	return true;
}

bool FrameBuffer::AddFloatColourAttachment(GLuint width, GLuint height)
{
	if (m_ColourAttachmentTextures.size() > 32)
	{
		std::cout << "ERROR: Cannot Add more than 32 Colour Attachments to Framebuffer" << std::endl;

		return false;
	}

	// Create, Generate, and Add Float Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	if (!colourAttachemntTexture->GenerateFloatTexture(width, height))
	{
		std::cout << "ERROR: Could not Create Float Colour Attachment Texture" << std::endl;
		delete colourAttachemntTexture;

		return false;
	}

	m_ColourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddIntColourAttachment(GLuint width, GLuint height)
{
	if (m_ColourAttachmentTextures.size() > 32)
	{
		std::cout << "ERROR: Cannot Add more than 32 Colour Attachments to Framebuffer" << std::endl;

		return false;
	}

	// Create, Generate, and Add Int Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	if (!colourAttachemntTexture->GenerateIntTexture(width, height))
	{
		std::cout << "ERROR: Could not Create Int Colour Attachment Texture" << std::endl;
		delete colourAttachemntTexture;

		return false;
	}

	m_ColourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddUIntColourAttachment(GLuint width, GLuint height)
{
	if (m_ColourAttachmentTextures.size() > 32)
	{
		std::cout << "ERROR: Cannot Add more than 32 Colour Attachments to Framebuffer" << std::endl;

		return false;
	}

	// Create, Generate, and Add UInt Colour Attachment Texture
	AssetType::Texture* colourAttachemntTexture = new AssetType::Texture();
	if (!colourAttachemntTexture->GenerateUIntTexture(width, height))
	{
		std::cout << "ERROR: Could not Create UInt Colour Attachment Texture" << std::endl;
		delete colourAttachemntTexture;

		return false;
	}

	m_ColourAttachmentTextures.push_back(colourAttachemntTexture);

	return true;
}
bool FrameBuffer::AddDepthBufferTexture(GLuint a_Width, GLuint a_Height)
{
	if (m_DepthBufferTexture != nullptr)
	{
		std::cout << "ERROR: Cannot Assign another Depth Buffer Texture to Framebuffer" << std::endl;

		return false;
	}

	// Create, Generate, and Add Depth Buffer Texture
	m_DepthBufferTexture = new AssetType::Texture();
	if (!m_DepthBufferTexture->GenerateDepthTexture(a_Width, a_Height))
	{
		std::cout << "ERROR: Could not Generate Framebuffer's Depth Buffer Texture" << std::endl;
	}

	return true;
}

bool FrameBuffer::AssignExternalDepthBufferTexture(AssetType::Texture* depthBufferTexture)
{
	if (m_DepthBufferTexture != nullptr)
	{
		std::cout << "ERROR: Cannot Assign another Depth Buffer Texture to Framebuffer" << std::endl;

		return false;
	}

	// Attach External Depth Buffer Texture
	m_ExternalDepthBufferTexture = true;
	m_DepthBufferTexture = depthBufferTexture;

	return true;
}
}
