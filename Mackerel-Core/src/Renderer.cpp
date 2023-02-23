#include "Renderer.h"

namespace MCK::Rendering {
Renderer::Renderer() :
	_GBuffer(0), _frameBuffer(0), _lightUniformBuffer(nullptr)
{
	// Initialise Light Uniform Buffer
	_lightUniformBuffer = new UniformBuffer();

	// Initialise Geometry Buffer Textures
	for (int i = 0; i < 32; i++)
	{
		_GBufferTextures[i] = new AssetType::Texture();
	}
}
Renderer::~Renderer()
{
	// Delete all Geometry Buffer Textures
	for (int i = 0; i < 32; i++)
	{
		if (_GBufferTextures[i])
			delete _GBufferTextures[i];
	}

	// Clear & Delete Light Uniform Buffer
	_lightUniformBuffer->DeleteUniformBufferObject();
	delete _lightUniformBuffer;
}

/**
 * Initialises the Renderer Instance.
 * 
 * \param screenWidth: The Target Screen's Width
 * \param screenHeight: The Target SCreen's Height
 * \return Whether the Renderer Instance was Successfully Initialised
 */
bool Renderer::initialiseRenderer(GLuint screenWidth, GLuint screenHeight)
{
	if (_GBuffer)
	{// Geometry Buffer was already Created
		return false;
	}
	if (_lightUniformBuffer->isCreated())
	{// Lighting Uniform Buffer Object was already Created
		return false;
	}

	// Create the Geometry Buffer
	if (!createGBuffer(screenWidth, screenHeight))
	{// Geometry Buffer could not be Created
		return false;
	}
	// Create the Lighting Uniform Buffer Object
	if (!createLightingUniformBuffer())
	{// Lighting Uniform Buffer Object could not be Created
		return false;
	}

	return true;
}

/**
 * Creates the Geometry Buffer.
 *
 * \param screenWidth: The Geometry Buffer's Width
 * \param screenHeight: The Geometry Buffer's Height
 * \return Whether the Geometry Buffer was Successfully Created
 */
bool Renderer::createGBuffer(GLuint screenWidth, GLuint screenHeight)
{
	// Generate Geometry Buffer
	glGenFramebuffers(1, &_GBuffer);

	std::vector<GLuint> GBufferAttachments;
	// Generate UInt Texture for First GBuffer Texture
	{
		_GBufferTextures[0]->GenerateUIntTexture(screenWidth, screenHeight);

		glFramebufferTexture2D(_GBuffer, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _GBufferTextures[0]->getTextureID(), 0);
		GBufferAttachments.push_back(GL_COLOR_ATTACHMENT0);
	}
	for (int i = 1; i < 32; i++)
	{// Generate Float Tetxures for Remaining GBuffer Textures
		_GBufferTextures[i]->GenerateFloatTexture(screenWidth, screenHeight);
		GLuint attachementSlot = GL_COLOR_ATTACHMENT0 + i;

		glFramebufferTexture2D(_GBuffer, attachementSlot, GL_TEXTURE_2D, _GBufferTextures[i]->getTextureID(), 0);
		GBufferAttachments.push_back(attachementSlot);
	}
	glDrawBuffers((GLuint)GBufferAttachments.size(), &GBufferAttachments[0]);

	// TODO: Generate Depth Buffer

	return true;
}
/**
 * Creates the Lighting Uniform Buffer Object.
 * 
 * \return Whether the Lighting Uniform Buffer Object was Successfully Created
 */
bool Renderer::createLightingUniformBuffer()
{
	// Add Num Lights Uniforms
	_lightUniformBuffer->AddUInt32BufferUniform("numPointLights", (uint32_t)_pointLights.size());
	_lightUniformBuffer->AddUInt32BufferUniform("numDirectionLights", (uint32_t)_pointLights.size());
	_lightUniformBuffer->AddUInt32BufferUniform("numSpotLights", (uint32_t)_pointLights.size());

	for (int i = 0; i < MAX_LIGHTS; i++)
	{// Add Buffer Uniforms for Point Lights
		std::string arrayEntryUniformName = "pointLights[" + std::to_string(i) + "]";

		std::string diffuseColourUniformName = arrayEntryUniformName + ".diffuseColour";
		Eigen::Vector4f diffuseColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);
		std::string specularColourUniformName = arrayEntryUniformName + ".specularColour";
		Eigen::Vector4f specularColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);
		std::string ambientColourUniformName = arrayEntryUniformName + ".ambientColour";
		Eigen::Vector4f ambientColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);

		std::string positionUniformName = arrayEntryUniformName + ".position";
		Eigen::Vector3f positionUniformValue(0.0f, 0.0f, 0.0f);

		if (i < _pointLights.size())
		{
			diffuseColourUniformValue = _pointLights[i]->diffuseColour;
			specularColourUniformValue = _pointLights[i]->specularColour;
			ambientColourUniformValue = _pointLights[i]->ambientColour;

			positionUniformValue = _pointLights[i]->position;
		}

		_lightUniformBuffer->AddVec4BufferUniform(diffuseColourUniformName, diffuseColourUniformValue);
		_lightUniformBuffer->AddVec4BufferUniform(specularColourUniformName, specularColourUniformValue);
		_lightUniformBuffer->AddVec4BufferUniform(ambientColourUniformName, ambientColourUniformValue);

		_lightUniformBuffer->AddVec3BufferUniform(positionUniformName, positionUniformValue);
	}
	for (int i = 0; i < MAX_LIGHTS; i++)
	{// Add Buffer Uniforms for Direction Lights
		std::string arrayEntryUniformName = "directionLights[" + std::to_string(i) + "]";

		std::string diffuseColourUniformName = arrayEntryUniformName + ".diffuseColour";
		Eigen::Vector4f diffuseColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);
		std::string specularColourUniformName = arrayEntryUniformName + ".specularColour";
		Eigen::Vector4f specularColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);
		std::string ambientColourUniformName = arrayEntryUniformName + ".ambientColour";
		Eigen::Vector4f ambientColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);

		std::string directionUniformName = arrayEntryUniformName + ".direction";
		Eigen::Vector3f directionUniformValue(0.0f, 0.0f, 0.0f);

		if (i < _directionLights.size())
		{
			diffuseColourUniformValue = _directionLights[i]->diffuseColour;
			specularColourUniformValue = _directionLights[i]->specularColour;
			ambientColourUniformValue = _directionLights[i]->ambientColour;

			directionUniformValue = _directionLights[i]->direction;
		}

		_lightUniformBuffer->AddVec4BufferUniform(diffuseColourUniformName, diffuseColourUniformValue);
		_lightUniformBuffer->AddVec4BufferUniform(specularColourUniformName, specularColourUniformValue);
		_lightUniformBuffer->AddVec4BufferUniform(ambientColourUniformName, ambientColourUniformValue);

		_lightUniformBuffer->AddVec3BufferUniform(directionUniformName, directionUniformValue);
	}
	for (int i = 0; i < MAX_LIGHTS; i++)
	{// Add Buffer Uniforms for Spot Lights
		std::string arrayEntryUniformName = "spotLights[" + std::to_string(i) + "]";

		std::string diffuseColourUniformName = arrayEntryUniformName + ".diffuseColour";
		Eigen::Vector4f diffuseColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);
		std::string specularColourUniformName = arrayEntryUniformName + ".specularColour";
		Eigen::Vector4f specularColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);
		std::string ambientColourUniformName = arrayEntryUniformName + ".ambientColour";
		Eigen::Vector4f ambientColourUniformValue(0.0f, 0.0f, 0.0f, 0.0f);

		std::string positionUniformName = arrayEntryUniformName + ".position";
		Eigen::Vector3f positionUniformValue(0.0f, 0.0f, 0.0f);
		std::string directionUniformName = arrayEntryUniformName + ".direction";
		Eigen::Vector3f directionUniformValue(0.0f, 0.0f, 0.0f);
		std::string beamAngleUniformName = arrayEntryUniformName + ".beamAngle";
		float beamAngleUniformValue = 0.0f;

		if (i < _spotLights.size())
		{
			diffuseColourUniformValue = _spotLights[i]->diffuseColour;
			specularColourUniformValue = _spotLights[i]->specularColour;
			ambientColourUniformValue = _spotLights[i]->ambientColour;

			positionUniformValue = _spotLights[i]->position;
			directionUniformValue = _spotLights[i]->direction;

			beamAngleUniformValue = _spotLights[i]->beamAngle;
		}

		_lightUniformBuffer->AddVec4BufferUniform(diffuseColourUniformName, diffuseColourUniformValue);
		_lightUniformBuffer->AddVec4BufferUniform(specularColourUniformName, specularColourUniformValue);
		_lightUniformBuffer->AddVec4BufferUniform(ambientColourUniformName, ambientColourUniformValue);

		_lightUniformBuffer->AddVec3BufferUniform(positionUniformName, positionUniformValue);
		_lightUniformBuffer->AddVec3BufferUniform(directionUniformName, directionUniformValue);

		_lightUniformBuffer->AddFloatBufferUniform(beamAngleUniformName, beamAngleUniformValue);
	}

	// Create Lighting Unifrom Buffer Object
	bool result = _lightUniformBuffer->CreateUniformBufferObject();

	return result;
}

/**
 * Render the Geometry Data onto the Geometry Buffer.
 * 
 */
void Renderer::renderGBuffer()
{
	// Bind Geometry Buffer to Active Frame Buffer
	glBindFramebuffer(GL_FRAMEBUFFER, _GBuffer);

	// Clear the Geometry Buffer
	glClearColor(0.0, 0.0, 0.0, -1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render eeach of the Geometry Bathces
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

	// Load Geometry Buffer Textures
	for (int i = 0; i < 32; i++)
	{
		_GBufferTextures[i]->BindTexture(i);
	}

	// Update Lighting Uniform Buffer Object Data
	{
		_lightUniformBuffer->SetUInt32BufferUniform("numPointLights", (uint32_t)_pointLights.size());
		_lightUniformBuffer->SetUInt32BufferUniform("numDirectionLights", (uint32_t)_pointLights.size());
		_lightUniformBuffer->SetUInt32BufferUniform("numSpotLights", (uint32_t)_pointLights.size());

		for (int i = 0; i < _pointLights.size(); i++)
		{// Update Buffer Uniforms for Point Lights
			std::string arrayEntryUniformName = "pointLights[" + std::to_string(i) + "]";

			std::string diffuseColourUniformName = arrayEntryUniformName + ".diffuseColour";
			std::string specularColourUniformName = arrayEntryUniformName + ".specularColour";
			std::string ambientColourUniformName = arrayEntryUniformName + ".ambientColour";

			std::string positionUniformName = arrayEntryUniformName + ".position";

			_lightUniformBuffer->SetVec4BufferUniform(diffuseColourUniformName, _pointLights[i]->diffuseColour);
			_lightUniformBuffer->SetVec4BufferUniform(specularColourUniformName, _pointLights[i]->specularColour);
			_lightUniformBuffer->SetVec4BufferUniform(ambientColourUniformName, _pointLights[i]->ambientColour);

			_lightUniformBuffer->SetVec3BufferUniform(positionUniformName, _pointLights[i]->position);
		}
		for (int i = 0; i < _directionLights.size(); i++)
		{// Update Buffer Uniforms for Direction Lights
			std::string arrayEntryUniformName = "directionLights[" + std::to_string(i) + "]";

			std::string diffuseColourUniformName = arrayEntryUniformName + ".diffuseColour";
			std::string specularColourUniformName = arrayEntryUniformName + ".specularColour";
			std::string ambientColourUniformName = arrayEntryUniformName + ".ambientColour";

			std::string directionUniformName = arrayEntryUniformName + ".direction";

			_lightUniformBuffer->SetVec4BufferUniform(diffuseColourUniformName, _directionLights[i]->diffuseColour);
			_lightUniformBuffer->SetVec4BufferUniform(specularColourUniformName, _directionLights[i]->specularColour);
			_lightUniformBuffer->SetVec4BufferUniform(ambientColourUniformName, _directionLights[i]->ambientColour);

			_lightUniformBuffer->SetVec3BufferUniform(directionUniformName, _directionLights[i]->direction);
		}
		for (int i = 0; i < _spotLights.size(); i++)
		{// Update Buffer Uniforms for Spot Lights
			std::string arrayEntryUniformName = "spotLights[" + std::to_string(i) + "]";

			std::string diffuseColourUniformName = arrayEntryUniformName + ".diffuseColour";
			std::string specularColourUniformName = arrayEntryUniformName + ".specularColour";
			std::string ambientColourUniformName = arrayEntryUniformName + ".ambientColour";

			std::string positionUniformName = arrayEntryUniformName + ".position";
			std::string directionUniformName = arrayEntryUniformName + ".direction";

			std::string beamAngleUniformName = arrayEntryUniformName + ".beamAngle";

			_lightUniformBuffer->SetVec4BufferUniform(diffuseColourUniformName, _spotLights[i]->diffuseColour);
			_lightUniformBuffer->SetVec4BufferUniform(specularColourUniformName, _spotLights[i]->specularColour);
			_lightUniformBuffer->SetVec4BufferUniform(ambientColourUniformName, _spotLights[i]->ambientColour);

			_lightUniformBuffer->SetVec3BufferUniform(positionUniformName, _spotLights[i]->position);
			_lightUniformBuffer->SetVec3BufferUniform(directionUniformName, _spotLights[i]->direction);

			_lightUniformBuffer->SetFloatBufferUniform(beamAngleUniformName, _spotLights[i]->beamAngle);
		}
	}
	// Load Lighting Uniform Buffer into Slot 0
	_lightUniformBuffer->BindUniformBufferObject(0);

	for (auto lightingShader : _lightingShaders)
	{
		lightingShader->getProgramID(); // Use Shader

		// Draw Quad Mesh
	}

	// TODO: Copy Depth Buffer (from Geometry Buffer) to Frame Buffer
}
}