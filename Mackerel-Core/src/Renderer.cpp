#include "Renderer.h"

namespace MCK::Rendering {
Renderer::Renderer() :
	_GBuffer(0), _frameBuffer(0), _lightUniformBuffer(0) {}
Renderer::~Renderer()
{
	for (auto [GBTextureName, GBTexture] : _GBufferTextures)
	{// Delete all G Buffer Textures
		if (GBTexture)
			delete GBTexture;
	}
	_GBufferTextures.clear();
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
	_GBufferTextures[name] = newTexture;

	return true;
}

/**
 * Creates the Lighting Uniform Buffer for all Lighting Shaders.
 * 
 * \return Whether the Lighting Uniform Buffer was Successfully Created
 */
bool Renderer::bindLightUniformBuffer()
{
	// Generate the Lights Uniform Buffer
	glGenBuffers(1, &_lightUniformBuffer);
	glBufferData(_lightUniformBuffer, 6668, 0, GL_STATIC_DRAW);

	// Binds Uniform Block to Binding Point 0
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _lightUniformBuffer);

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

	// Load Light Data into Lighting Uniform Buffer
	{
	size_t numPointLights = _pointLights.size();
	size_t numDirectionLights = _directionLights.size();
	size_t numSpotLight = _spotLights.size();

	glBufferSubData(_lightUniformBuffer, 0, 4, &numPointLights);
	glBufferSubData(_lightUniformBuffer, 4, 4, &numPointLights);
	glBufferSubData(_lightUniformBuffer, 8, 4, &numPointLights);

	// Load Point Light Data
	for (int i = 0; i < _pointLights.size(); i++)
	{
		PointLight* pointLight = _pointLights[i];

		glBufferSubData(_lightUniformBuffer, 12 + 0, 16, pointLight->position.data());

		glBufferSubData(_lightUniformBuffer, 12 + 16, 16, pointLight->diffuseColour.data());
		glBufferSubData(_lightUniformBuffer, 12 + 32, 16, pointLight->specularColour.data());
		glBufferSubData(_lightUniformBuffer, 12 + 48, 16, pointLight->ambientColour.data());
	}
	// Load Direction Light Data
	for (int i = 0; i < _directionLights.size(); i++)
	{
		DirectionLight* directionLight = _directionLights[i];

		glBufferSubData(_lightUniformBuffer, 2060 + 0, 16, directionLight->direction.data());

		glBufferSubData(_lightUniformBuffer, 2060 + 16, 16, directionLight->diffuseColour.data());
		glBufferSubData(_lightUniformBuffer, 2060 + 32, 16, directionLight->specularColour.data());
		glBufferSubData(_lightUniformBuffer, 2060 + 48, 16, directionLight->ambientColour.data());
	}
	// Load Spot Light Data
	for (int i = 0; i < _spotLights.size(); i++)
	{
		SpotLight* spotLight = _spotLights[i];

		glBufferSubData(_lightUniformBuffer, 4108 + 0, 16, spotLight->position.data());
		glBufferSubData(_lightUniformBuffer, 4108 + 16, 16, spotLight->direction.data());

		glBufferSubData(_lightUniformBuffer, 4108 + 32, 16, spotLight->diffuseColour.data());
		glBufferSubData(_lightUniformBuffer, 4108 + 48, 16, spotLight->specularColour.data());
		glBufferSubData(_lightUniformBuffer, 4108 + 64, 16, spotLight->ambientColour.data());
	}
	}

	for (auto lightingShader : _lightingShaders)
	{
		lightingShader->getProgramID(); // Use Shader

		// Draw Quad Mesh
	}

	// TODO: Copy Depth Buffer (from Geometry Buffer) to Frame Buffer

	for (auto [GBTextureName, GBTexture] : _GBufferTextures)
	{// Clear G Buffer Textures
		GBTexture->ClearTexture();
	}
}
}