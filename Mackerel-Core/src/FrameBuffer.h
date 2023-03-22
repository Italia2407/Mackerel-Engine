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
	GLuint m_FrameBufferObject;
	bool m_IsCreated;

	std::vector<AssetType::Texture*> m_ColourAttachmentTextures;

	bool m_ExternalDepthBufferTexture;
	AssetType::Texture* m_DepthBufferTexture;

public:
	const bool& IsCreated() const { return m_IsCreated; }

	GLuint GetNumColourAttachments() { return (GLuint)m_ColourAttachmentTextures.size(); }
	AssetType::Texture* GetColourAttachmentTexture(GLuint slot) { return (slot < 32) ? m_ColourAttachmentTextures[slot] : nullptr; }

public:
	bool CreateFrameBuffer();
	bool UseFrameBufferObject(Eigen::Vector4f clearColour, GLuint clearFlags);

	bool AddFloatColourAttachment(GLuint width, GLuint height);
	bool AddIntColourAttachment(GLuint width, GLuint height);
	bool AddUIntColourAttachment(GLuint width, GLuint height);
	bool AddDepthBufferTexture(GLuint a_Width, GLuint a_Height);

	bool AssignExternalDepthBufferTexture(AssetType::Texture* depthBufferTexture);
};
}
