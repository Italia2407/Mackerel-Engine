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
	m_ShaderProgramID(GL_ZERO)
{
	// Initialise Geometry & Lighting Frame Buffers
	m_GeometryBuffer = new FrameBuffer();
	m_LightingBuffer = new FrameBuffer();

	// Initialise Depth Buffer Texture
	m_DepthBufferTexture = new AssetType::Texture();
}
Renderer::~Renderer()
{
	// Delete Geometry & Lighting Frame Buffers
	if (m_GeometryBuffer)
		delete m_GeometryBuffer;
	if (m_LightingBuffer)
		delete m_LightingBuffer;

	// Delete Depth Buffer Texture
	if (m_DepthBufferTexture)
		delete m_DepthBufferTexture;
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
	// Create the Shader Program
	m_ShaderProgramID = glCreateProgram();

	// Create the Depth Buffer Texture
	m_DepthBufferTexture->GenerateFloatTexture(screenWidth, screenHeight);

	// Create the Geometry Frame Buffer
	if (m_GeometryBuffer->IsCreated())
	{// Geometry Frame Buffer was Already Created
		return false;
	}

	// Add Colour Attachment Textures
	// Texture 0 is The Lighting Shader ID Map
	m_GeometryBuffer->AddUIntColourAttachment(screenWidth, screenHeight);

	for (int i = 1; i < 31; i++)
		m_GeometryBuffer->AddFloatColourAttachment(screenWidth, screenHeight);
	
	// Assign Depth Buffer Texture
	m_GeometryBuffer->AssignExternalDepthBufferTexture(m_DepthBufferTexture);

	// Create Geometry Buffer Object
	if (!m_GeometryBuffer->CreateFrameBuffer())
	{// Geometry Buffer could not be Created
		return false;
	}


	// Create the Lighting Frame Buffer
	if (m_LightingBuffer->IsCreated())
	{// Lighting Frame Buffer was Already Created
		return false;
	}

	// Add Colour Attachment Texture & Depth Buffer Texture
	m_LightingBuffer->AddFloatColourAttachment(screenWidth, screenHeight);
	m_LightingBuffer->AddDepthBufferTexture(screenWidth, screenHeight);

	// Create Lighting Frame Buffer Object
	if (!m_LightingBuffer->CreateFrameBuffer())
	{// Lighitng Frame Buffer could not be Created
		return false;
	}

	return true;
}

bool Renderer::attachShader(AssetType::Shader* a_Shader)
{
	// Get Currently Attached Shaders
	GLsizei shaderCount = 2;
	GLuint shaderIDs[2];
	glGetAttachedShaders(m_ShaderProgramID, shaderCount, &shaderCount, shaderIDs);
	
	// Detach all Shaders with Same Type as Incoming Shader
	for (auto shaderID : shaderIDs)
	{
		GLint shaderType;
		glGetShaderiv(shaderID, GL_SHADER_TYPE, &shaderType);

		if (shaderType == a_Shader->ShaderType())
		{
			glDetachShader(m_ShaderProgramID, shaderID);
		}
	}

	// Attach Incoming Shader to Shader Program
	glAttachShader(m_ShaderProgramID, a_Shader->ShaderID());

	return true;
}

/**
 * Render the Geometry Data onto the Geometry Buffer.
 * 
 * \return Whether the GBuffer could be Rendered
 */
bool Renderer::renderGBuffer()
{
	// Use Geometry Frame Buffer as Render Target
	m_GeometryBuffer->UseFrameBufferObject(Eigen::Vector4f::Zero(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto geometryBatch : m_GeometryBatches)
	{
		// Attach Geometry Batch Shader to Shader Program
		if (!attachShader(geometryBatch->Shader()))
		{
			return false;
		}

		// Render All Batch Instances to GBuffer Textures
		geometryBatch->DrawBatchObjects();
	}

	return true;
}
/**
 * Render the Scene without Transparent Objects.
 * 
 * \return 
 */
bool Renderer::renderLightingBuffer()
{
	// Use Lighting Frame Buffer as Render Target
	m_LightingBuffer->UseFrameBufferObject(Eigen::Vector4f::Zero(), GL_COLOR_BUFFER_BIT);

	// Load Geometry Buffer Textures
	for (GLuint i = 0; i < m_GeometryBuffer->GetNumColourAttachments(); i++)
	{
		auto GBufferTexture = m_GeometryBuffer->GetColourAttachmentTexture(i);
		GBufferTexture->BindTexture(i);
	}

	// Point Light Calculations
	for (auto lightShader : _pointLightShaders)
	{
		// Attach Point Light Shader
		attachShader(lightShader);

		// Render Point Lighting
		for (auto light : _pointLights)
		{
			light->UseLight();

			//DrawQuad();
		}
	}
	// Direction Light Calculations
	for (auto lightShader : _directionLightShaders)
	{
		// Attach Direction Light Shader
		attachShader(lightShader);

		// Render Direction Lighting
		for (auto light : _directionLights)
		{
			light->UseLight();

			//DrawQuad();
		}
	}
	// Spot Light Calculations
	for (auto lightShader : _spotLightShaders)
	{
		// Attach Spot Light Shader
		attachShader(lightShader);

		// Render Spot Lighting
		for (auto light : _spotLights)
		{
			light->UseLight();

			//DrawQuad();
		}
	}

	return true;
}

/**
 * Queue Mesh Instance to be Rendered for the Frame.
 * 
 * \param a_Mesh: The Geometry Instance's Mesh
 * \param a_Shader: The Geometry Instace's Shader
 * \param a_Material: The Geometry Instance's Material
 * \param a_Transform: The Geometry Instance's Transform Matrix
 * \return Whether the Geometry Instance was Successfully Added
 */
bool Renderer::queueGeometryBatchInstance(AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material, Eigen::Matrix4f a_Transform)
{
	RenderBatch* geometryBatch = nullptr;

	// Find Existing Geometry Batch with Same Mesh & Shader
	for (auto batch : m_GeometryBatches)
	{
		if (batch->Mesh() != a_Mesh)
			continue;

		if (batch->Shader() != a_Shader)
			continue;

		geometryBatch = batch;
		break;
	}
	// Create New Geometry Batch if no Match Exists
	if (!geometryBatch)
	{
		geometryBatch = new RenderBatch(a_Mesh, a_Shader);
	}

	// Add Mesh Instance to Geometry Batch
	geometryBatch->AddBatchInstance(a_Material, a_Transform);
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
 * 
 * \return Whether the Frame Could be Rendered 
 */
bool Renderer::renderFrame()
{
	// Render Scene to the Geometry Buffer
	if (!renderGBuffer())
	{
		return false;
	}

	// TODO:
	// Render Shadow Map for Lights

	// Render to Lighting Buffer
	// TODO:
	// Enable Additive Blending of Overlapping Fragments
	// Load Passthrough Vertex Shader

	if (!renderLightingBuffer())
	{
		return false;
	}

	// TODO:
	// Disable Additive Blending of Overlapping Fragments
	// Load Standard Vertex Shader

	// TODO:
	// Render Transparency Buffer

	// TODO:
	// Display Frame to Scene

	// Clear Geometry Batches
	for (auto geometryBatch : m_GeometryBatches) {
		if (geometryBatch)
			delete geometryBatch;
	}
	m_GeometryBatches.clear();

	// Clear Scene Lights
	_pointLights.clear();
	_directionLights.clear();
	_spotLights.clear();

	return true;
}
}