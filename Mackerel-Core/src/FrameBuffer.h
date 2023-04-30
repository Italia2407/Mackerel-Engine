#pragma once

#include "GLinclude.h"
#include <Eigen/Eigen.h>

#include <vector>
#include <string>

// Forward Declarations
namespace MCK::AssetType {
class Texture;
}

namespace MCK {
class FrameBuffer
{
public:
	FrameBuffer(GLuint a_Width, GLuint a_Height);
	~FrameBuffer();

private:
	GLuint m_FrameBufferObject;
	
	GLuint m_Width;
	GLuint m_Height;

	std::vector<AssetType::Texture*> m_ColourAttachmentTextures;

	bool m_ExternalDepthBufferTexture;
	AssetType::Texture* m_DepthBufferTexture;

public:
	const bool& IsCreated() const { return m_FrameBufferObject != GL_ZERO; }

	GLuint GetNumColourAttachments() { return (GLuint)m_ColourAttachmentTextures.size(); }

	AssetType::Texture* GetColourAttachmentTexture(GLuint slot) { return (slot < m_ColourAttachmentTextures.size()) ? m_ColourAttachmentTextures[slot] : nullptr; }
	AssetType::Texture* GetInternalDepthBufferTexture() { return m_ExternalDepthBufferTexture ? m_DepthBufferTexture : m_DepthBufferTexture; }

public:
	bool CreateFrameBuffer();
	bool UseFrameBufferObject(Eigen::Vector4f clearColour, GLuint clearFlags);

	bool AddFloatColourAttachment();
	bool AddIntColourAttachment();
	bool AddUIntColourAttachment();
	bool AddDepthBufferTexture();

	bool ResizeFramebuffer(GLuint a_Width, GLuint a_Height);

	bool AssignExternalDepthBufferTexture(AssetType::Texture* depthBufferTexture);
};
}
