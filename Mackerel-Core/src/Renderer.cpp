#include "Renderer.h"

namespace MCK::Rendering {
Renderer::Renderer() :
	_GBuffer(0), _frameBuffer(0) {}
Renderer::~Renderer()
{
	for (auto [GBTextureName, GBTexture] : _GBufferTextures)
	{// Delete all G Buffer Textures
		if (GBTexture)
			delete GBTexture;
	}
	_GBufferTextures.clear();

	for (auto lightingMaterial : _lightingMaterials)
	{// Delete all Lighting Materials
		if (lightingMaterial)
			delete lightingMaterial;
	}
	_lightingMaterials.clear();
}

bool Renderer::addGBufferTexture(std::string name)
{
	if (!_GBufferTextures.contains(name))
	{// Deferred Buffer Texture with Given Name already Exists
		return false;
	}
	if (_GBufferTextures.size() >= 32)
	{// Cannot Attach any more Textures to Frame Buffer
		return false;
	}

	AssetType::Texture* newTexture = new AssetType::Texture();
	for (auto material : _lightingMaterials)
	{// Add G Buffer Texture to all Lighting Materials
		material->AddTextureUniform(name, newTexture);
	}

	_GBufferTextures[name] = newTexture;
	return true;
}
bool Renderer::addLightingMaterial(AssetType::Shader* lightingShader)
{
	for (auto lightingMaterial : _lightingMaterials) {
	if (lightingMaterial->shader() == lightingShader)
	{// Check if no duplicate Lighting Material Exist
		return false;
	}
	}

	AssetType::Material* newLightingMaterial = new AssetType::Material(lightingShader);

	for (auto [GBTextureName, GBTexture] : _GBufferTextures)
	{// Add all G Buffer Textures to Material
		newLightingMaterial->AddTextureUniform(GBTextureName, GBTexture);
	}

	_lightingMaterials.push_back(newLightingMaterial);
	return true;
}

void Renderer::bindGBuffer(GLuint screenWidth, GLuint screenHeight)
{
	// Generate Geometry Buffer
	glGenFramebuffers(1, &_GBuffer);

	std::vector<GLuint> GBufferAttachments;
	for (auto [GBTextureName, GBTexture] : _GBufferTextures)
	{
		GLuint attachementSlot = GL_COLOR_ATTACHMENT0 + (GLuint)GBufferAttachments.size();
		GBTexture->GenerateTexture(screenWidth, screenHeight);

		glFramebufferTexture2D(_GBuffer, attachementSlot, GL_TEXTURE_2D, GBTexture->getTextureID(), 0);
		GBufferAttachments.push_back(attachementSlot);
	}
	glDrawBuffers((GLuint)GBufferAttachments.size(), &GBufferAttachments[0]);

	// TODO: Generate Depth Buffer
}

void Renderer::renderGBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _GBuffer);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto geometryBatch : _geometryBatches)
	{
		geometryBatch->LoadShaderMesh();
		geometryBatch->DrawBatchObjects();
	}
}
void Renderer::renderFrameBuffer()
{
	// Generate Frame Buffer
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto lightingMaterial : _lightingMaterials)
	{
		lightingMaterial->shader(); // Use Shader
		lightingMaterial->BindUniforms();

		// Draw Quad Mesh

		lightingMaterial->ResetUniforms();
	}

	// TODO: Copy Depth Buffer (from Geometry Buffer) to Frame Buffer

	for (auto [GBTextureName, GBTexture] : _GBufferTextures)
	{// Clear G Buffer Textures
		GBTexture->ClearTexture();
	}
}
}