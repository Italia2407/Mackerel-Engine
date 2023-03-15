#include "Renderer.h"

#include "UniformBuffer.h"
#include "FrameBuffer.h"

#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

#include "Light.h"
#include "RenderBatch.h"

#include <utility>
#include <functional>

#define MAX_LIGHTS (32)

namespace MCK::Rendering {
Renderer::Renderer() :
	_GBuffer(0), _frameBuffer(0), _depthTexture(nullptr)
{
	// Initialise Geometry & Lighting Frame Buffers
	_geometryBuffer = new FrameBuffer();
	_lightingBuffer = new FrameBuffer();

	// Initialise Geometry Buffer Textures
	for (int i = 0; i < 31; i++)
	{
		_GBufferTextures[i] = new AssetType::Texture();
	}

	// Initialise the Depth Buffer Texture
	_depthTexture = new AssetType::Texture();
}
Renderer::~Renderer()
{
	// Delete all Geometry Buffer Textures
	for (int i = 0; i < 32; i++)
	{
		if (_GBufferTextures[i])
			delete _GBufferTextures[i];
	}
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
	// Create the Depth Buffer Texture
	_depthTexture->GenerateFloatTexture(screenWidth, screenHeight);

	// Create the Geometry Frame Buffer
	if (_geometryBuffer->IsCreated())
	{// Geometry Frame Buffer was Already Created
		return false;
	}
	if (!createGBuffer(screenWidth, screenHeight))
	{// Geometry Frame Buffer could not be Created
		return false;
	}

	// Create the Lighting Frame Buffer
	if (_lightingBuffer->IsCreated())
	{// Lighting Frame Buffer was Already Created
		return false;
	}
	if (!createLightingBuffer(screenWidth, screenHeight))
	{// Lighting Frame Buffer could not be Created
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
	// Add Colour Attachment Textures
	_geometryBuffer->AddUIntColourAttachment(screenWidth, screenHeight);
	for (int i = 1; i < 31; i++)
		_geometryBuffer->AddFloatColourAttachment(screenWidth, screenHeight);

	// Assign Depth Buffer Texture
	_geometryBuffer->AssignDepthBufferTexture(_depthBufferTexture);

	// Create Geometry Buffer Object
	bool result = _geometryBuffer->CreateFrameBuffer();
	return result;
}
/**
 * Creates the Lighting Frame Buffer.
 * 
 * \param screenWidth: The Lighting Frame Buffer's Width
 * \param screenHeight: The Lighting Frame Buffer's Height
 * \return Whether the Lighting Frame Buffer was Successfully Created
 */
bool Renderer::createLightingBuffer(GLuint screenWidth, GLuint screenHeight)
{
	// Add Colour Attachment Texture
	_lightingBuffer->AddFloatColourAttachment(screenWidth, screenHeight);

	// Assign Depth Buffer Texture
	_lightingBuffer->AssignDepthBufferTexture(_depthBufferTexture);

	// Create Geometry Buffer Object
	bool result = _lightingBuffer->CreateFrameBuffer();
	return result;
}

/**
 * Render the Geometry Data onto the Geometry Buffer.
 * 
 */
void Renderer::renderGBuffer()
{
	// Use Geometry Frame Buffer
	_geometryBuffer->UseFrameBufferObject(Eigen::Vector4f::Zero(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render each of the Geometry Bathces
	for (auto [geometryBatchKey, geometryBatch] : _geometryBatches)
	{
		// Render to G Buffer Textures
		geometryBatch->LoadMeshShader();
		geometryBatch->DrawBatchObjects();
	}
}
/** Render the Scene without Transparent Objects */
void Renderer::renderLightingBuffer()
{
	// Use Lighting Frame Buffer
	_lightingBuffer->UseFrameBufferObject(Eigen::Vector4f::Zero(), GL_COLOR_BUFFER_BIT);

	// Load Geometry Buffer Textures
	for (GLuint i = 0; i < _geometryBuffer->GetNumColourAttachments(); i++)
	{
		_geometryBuffer->GetColourAttachmentTexture(i)->BindTexture(i);
	}

	// Point Light Calculations
	for (auto pointLightShader : _pointLightShaders)
	{
		pointLightShader->getProgramID(); // Use Shader

		for (auto pointLight : _pointLights)
		{
			pointLight->UseLight();

			// Render Lighitng
			//DrawQuad();
		}
	}
	// Direction Light Calculations
	for (auto directionLightShader : _directionLightShaders)
	{
		directionLightShader->getProgramID(); // Use Shader

		for (auto directionLight : _directionLights)
		{
			directionLight->UseLight();

			// Render Lighitng
			//DrawQuad();
		}
	}
	// Spot Light Calculations
	for (auto spotLightShader : _spotLightShaders)
	{
		spotLightShader->getProgramID(); // Use Shader

		for (auto spotLight : _spotLights)
		{
			spotLight->UseLight();

			// Render Lighitng
			//DrawQuad();
		}
	}

	// TODO: Copy Depth Buffer (from Geometry Buffer) to Frame Buffer
}
void Renderer::renderShadowMap(Light* light)
{

}

/**
 * Queue Mesh Instance to be Rendered for the Frame.
 * 
 * \param mesh: The Instance Mesh
 * \param shader: The Instance Shader
 * \param material: The Instance Material
 * \param position: The Instance Position
 * \param rotation: The Instance Rotation
 * \param scale: The Instance Scale
 * \return Whether the Instance was Successfully Added
 */
bool Renderer::queueRenderBatchInstance(AssetType::Mesh* mesh, AssetType::Shader* shader, AssetType::Material* material,
	Eigen::Vector3f position, Eigen::Quaternion<float> rotation, Eigen::Vector3f scale)
{
	// Geometry Batch to which the Instance will be Added
	RenderBatch* geometryBatch = nullptr;

	auto batchKey = std::make_pair(mesh, shader);
	if (!_geometryBatches.contains(batchKey))
	{// No Geometry Batch with Mesh & Shader Pair Currently Exists
		geometryBatch = new RenderBatch(mesh, shader);
		_geometryBatches[batchKey] = geometryBatch;
	}
	else
	{// Geometry Batch with Mesh & Shader Pair Currently Exists
		geometryBatch = _geometryBatches[batchKey];
	}

	if (!geometryBatch)
	{// No Geometry Batch could be Found or Created
		return false;
	}

	geometryBatch->AddBatchInstance(material, position, rotation, scale);
	return true;
}

bool Renderer::queuePointLight(PointLight* pointLight)
{
	_pointLights.push_back(pointLight);
	return true;
}
bool Renderer::queueDirectionLight(DirectionLight* directionLight)
{
	_directionLights.push_back(directionLight);
	return true;
}
bool Renderer::queueSpotLight(SpotLight* spotLight)
{
	_spotLights.push_back(spotLight);
	return true;
}

/**
 * Render the Current Frame. 
 */
void Renderer::renderFrame()
{
	// Render Scene Geometry to the Geometry Buffer
	renderGBuffer();

	// Render Shadow Maps for all Lights
	for (auto pointLight : _pointLights)
	{
		renderShadowMap(pointLight);
	}
	for (auto directionLight : _directionLights)
	{
		renderShadowMap(directionLight);
	}
	for (auto spotLight : _spotLights)
	{
		renderShadowMap(spotLight);
	}

	// Render Lighting Buffer
	renderLightingBuffer();


	// TODO: Show Frame to Screen

	// Clear Geometry Batches
	for (auto [geometryBatchKey, geometryBatch] : _geometryBatches)
	{
		if (geometryBatch)
			delete geometryBatch;
	}
	_geometryBatches.clear();

	// Clear Lights
	_pointLights.clear();
	_directionLights.clear();
	_spotLights.clear();
}
}