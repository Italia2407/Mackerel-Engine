#include "Renderer.h"

#include "UniformBuffer.h"
#include "FrameBuffer.h"

#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"

#include "Light.h"
#include "RenderBatch.h"

#include <iostream>
#include <utility>
#include <functional>

// Static Function & Parameters
namespace MCK::Rendering {
Renderer* Renderer::k_Instance = nullptr;

AssetType::Shader* Renderer::k_PassThroughVertexShader = nullptr;
AssetType::Shader* Renderer::k_FrameBufferDisplayShader = nullptr;

AssetType::Mesh* Renderer::k_FrameBufferDisplayMesh = nullptr;
AssetType::Mesh* Renderer::k_ScreenDisplayMesh = nullptr;

/**
* Initialises the Renderer.
*
* \param a_ScreenWidth: The Target Screen's Width
* \param a_ScreenHeight: The Target Screen's Height
* \return Whether the Renderer was Successfully Initialised
*/
bool Renderer::InitialiseRenderer(GLuint a_ScreenWidth, GLuint a_ScreenHeight)
{
	bool result = getInstance()->initialiseRenderer(a_ScreenWidth, a_ScreenHeight);
	return result;
}
/**  */
void Renderer::ClearRenderer()
{
	if (k_Instance)
	{
		delete k_Instance; k_Instance = nullptr;

		delete k_PassThroughVertexShader; k_PassThroughVertexShader = nullptr;
		delete k_FrameBufferDisplayShader; k_FrameBufferDisplayShader = nullptr;

		delete k_FrameBufferDisplayMesh; k_FrameBufferDisplayMesh = nullptr;
	}
}

/**
 * Add Unlit Deferred Shader to the Renderer.
 * 
 * \param a_Shader: The Unlit Shader to be Added
 * \return Whether the Shader was Successfully Added
 */
bool Renderer::AddUnlitShader(AssetType::Shader* a_Shader)
{
	bool result = getInstance()->addUnlitShader(a_Shader);
	return result;
}
/**
 * Add Point Light Deferred Shader to the Renderer.
 *
 * \param a_Shader: The Point Light Shader to be Added
 * \return Whether the Shader was Successfully Added
 */
bool Renderer::AddPointLightShader(AssetType::Shader* a_Shader)
{
	bool result = getInstance()->addPointLightShader(a_Shader);
	return result;
}
/**
 * Add Direction Light Deferred Shader to the Renderer.
 *
 * \param a_Shader: The Direction Light Shader to be Added
 * \return Whether the Shader was Successfully Added
 */
bool Renderer::AddDirectionLightShader(AssetType::Shader* a_Shader)
{
	bool result = getInstance()->addDirectionLightShader(a_Shader);
	return result;
}
/**
 * Add Spot Light Deferred Shader to the Renderer.
 *
 * \param a_Shader: The Spot Light Shader to be Added
 * \return Whether the Shader was Successfully Added
 */
bool Renderer::AddSpotLightShader(AssetType::Shader* a_Shader)
{
	bool result = getInstance()->addSpotLightShader(a_Shader);
	return result;
}

/**  */
bool Renderer::QueueMeshInstance(Eigen::Matrix4f a_Transform,
	AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material,
	bool a_HasTransparency)
{
	bool result = false;
	if (!a_HasTransparency)
	{
		result = getInstance()->queueGeometryBatchInstance(a_Mesh, a_Shader, a_Material, a_Transform);
	}

	return result;
}

/**  */
bool Renderer::RenderFrame(AssetType::Shader* a_ProjectionShader)
{
	bool result = getInstance()->renderFrame(a_ProjectionShader);
	return result;
}
}

namespace MCK::Rendering {
Renderer::Renderer() :
	m_ShaderProgramID(GL_ZERO),
	m_GeometryBuffer(nullptr), m_DeferredBuffer(nullptr),
	m_DepthBufferTexture(nullptr),
	m_TransformBuffer(nullptr) {}
Renderer::~Renderer()
{
	resetRenderer();
}

Renderer* Renderer::getInstance()
{
	// Create new Renderer Instance if it Doesn't Exist Yet
	if (!k_Instance)
	{
		k_Instance = new Renderer();

		// Load Static Shaders for the Renderer
		k_PassThroughVertexShader = new AssetType::Shader();
		k_PassThroughVertexShader->LoadShaderFromSource("../Mackerel-Core/res/Shaders/vert/passthrough.vert", GL_VERTEX_SHADER);

		k_FrameBufferDisplayShader = new AssetType::Shader();
		k_FrameBufferDisplayShader->LoadShaderFromSource("../Mackerel-Core/res/Shaders/static/FBDisplayer.glsl", GL_FRAGMENT_SHADER);

		// Load Display Meshes for the Renderer
		k_FrameBufferDisplayMesh = new AssetType::Mesh();
		k_FrameBufferDisplayMesh->LoadDisplayMesh();

		k_ScreenDisplayMesh = new AssetType::Mesh();
		k_ScreenDisplayMesh->LoadSmallDisplayMesh();
	}

	return k_Instance;
}

/**
 * Initialises the Renderer Instance.
 * 
 * \param screenWidth: The Target Screen's Width
 * \param screenHeight: The Target SCreen's Height
 * \return Whether the Renderer Instance was Successfully Initialised
 */
bool Renderer::initialiseRenderer(GLuint a_ScreenWidth, GLuint a_ScreenHeight)
{
	// Ensure Renderer Buffer Objects are not Recreated
	if (m_GeometryBuffer) {
		std::cout << "ERROR: Cannot Recreate Geometry Framebuffer" << std::endl;
		return false;
	}
	if (m_DeferredBuffer) {
		std::cout << "ERROR: Cannot Recreate Deferred Framebuffer" << std::endl;
		return false;
	}
	if (m_DepthBufferTexture) {
		std::cout << "ERROR: Cannot Recreate Depth Buffer Texture" << std::endl;
		return false;
	}
	if (m_TransformBuffer) {
		std::cout << "ERROR: Cannot Recreate Transform Uniform Buffer" << std::endl;
		return false;
	}


	// Create the Depth Buffer Texture
	m_DepthBufferTexture = new AssetType::Texture();

	if (!m_DepthBufferTexture->GenerateDepthTexture(a_ScreenWidth, a_ScreenHeight)) {
		resetRenderer();

		std::cout << "ERROR: Could not Generate Depth Buffer Texture" << std::endl;
		return false;
	}


	// Create the Geometry Frame Buffer
	m_GeometryBuffer = new FrameBuffer("Geometry Buffer");

	// Add Geometry Buffer Colour Attachments
	if (!m_GeometryBuffer->AddFloatColourAttachment(a_ScreenWidth, a_ScreenHeight))
	{// ID #0 is Reserved for the Lighting Shader ID Map
		resetRenderer();

		std::cout << "ERROR: Could not Add Geometry Buffer Colour Attachment Texture #0" << std::endl;
		return false;
	}
	for (int i = 1; i < 31; i++) {
	if (!m_GeometryBuffer->AddFloatColourAttachment(a_ScreenWidth, a_ScreenHeight))
	{// Other ID #s are for General Purpose
		resetRenderer();

		std::cout << "ERROR: Could not Add Geometry Buffer Colour Attachment Texture #" << i << std::endl;
		return false;
	}}

	// Assign Depth Buffer Texture Attachment
	if (!m_GeometryBuffer->AssignExternalDepthBufferTexture(m_DepthBufferTexture))
	{
		resetRenderer();

		std::cout << "ERROR: Could not Assign Depth Buffer Texture to Geometry Buffer" << std::endl;
		return false;
	}

	// Create Geometry Frame Buffer Object
	if (!m_GeometryBuffer->CreateFrameBuffer())
	{
		resetRenderer();

		std::cout << "ERROR: Could not Create Geometry Frame Buffer Object" << std::endl;
		return false;
	}


	// Create the Deferred Frame Buffer
	m_DeferredBuffer = new FrameBuffer("Deferred Buffer");

	// Add Output Colour Attachment Texture
	if (!m_DeferredBuffer->AddFloatColourAttachment(a_ScreenWidth, a_ScreenHeight))
	{
		resetRenderer();

		std::cout << "ERROR: Could not Add Deferred Buffer Output Colour Attachment Texture" << std::endl;
		return false;
	}

	// Add Depth Buffer Texture
	if (!m_DeferredBuffer->AddDepthBufferTexture(a_ScreenWidth, a_ScreenHeight))
	{
		resetRenderer();

		std::cout << "ERROR: Could not Add Deferred Buffer Depth Buffer Texture" << std::endl;
		return false;
	}

	// Create Deferred Frame Buffer Object
	if (!m_DeferredBuffer->CreateFrameBuffer())
	{
		resetRenderer();

		std::cout << "ERROR: Could not Create Deferred Frame Buffer Object" << std::endl;
		return false;
	}


	// Create the Transform Uniform Buffer
	m_TransformBuffer = new UniformBuffer();

	// Add Transform Uniform to Uniform Buffer
	if (!m_TransformBuffer->AddMat4BufferUniform("transform", Eigen::Matrix4f::Identity()))
	{
		resetRenderer();

		std::cout << "ERROR: Could not Add Transform Uniform to Transform Uniform Buffer" << std::endl;
		return false;
	}

	// Create Transform Uniform Buffer Object
	if (!m_TransformBuffer->CreateUniformBufferObject())
	{
		resetRenderer();

		std::cout << "ERROR: Could not Create Transform Uniform Buffer Object" << std::endl;
		return false;
	}


	return true;
}
/**
 * Resets the Renderer & Deletes all Objects.
 * 
 */
void Renderer::resetRenderer()
{
	// Delete Renderer Buffer Objects
	if (m_GeometryBuffer) {
		delete m_GeometryBuffer;
		m_GeometryBuffer = nullptr;
	}
	if (m_DeferredBuffer) {
		delete m_DeferredBuffer;
		m_DeferredBuffer = nullptr;
	}
	if (m_DepthBufferTexture) {
		delete m_DepthBufferTexture;
		m_DepthBufferTexture = nullptr;
	}
	if (m_TransformBuffer) {
		delete m_TransformBuffer;
		m_TransformBuffer = nullptr;
	}

	// Clear all Per Frame Objects
	resetRendererFrame();

	// Clear all Lighting Shaders
	m_UnlitShaders.clear();

	_pointLightShaders.clear();
	_directionLightShaders.clear();
	_spotLightShaders.clear();
}
/**
 * Reset Renderer for the Next Frame.
 * 
 */
void Renderer::resetRendererFrame()
{
	// Delete & Clear all Render Batches
	for (auto geometryBatch : m_GeometryBatches) {
	if (geometryBatch) {
		delete geometryBatch;
	}}
	m_GeometryBatches.clear();

	// Clear all Scene Lights
	_pointLights.clear();
	_directionLights.clear();
	_spotLights.clear();
}

bool Renderer::addUnlitShader(AssetType::Shader* a_Shader)
{
	if (!a_Shader) {
		std::cout << "ERROR: Cannot Add NULLPTR Unlit Shader" << std::endl;
		return false;
	}

	m_UnlitShaders.push_back(a_Shader);
	return true;
}
bool Renderer::addPointLightShader(AssetType::Shader* a_Shader)
{
	if (!a_Shader) {
		std::cout << "ERROR: Cannot Add NULLPTR Point Light Shader" << std::endl;
		return false;
	}

	_pointLightShaders.push_back(a_Shader);
	return true;
}
bool Renderer::addDirectionLightShader(AssetType::Shader* a_Shader)
{
	if (!a_Shader) {
		std::cout << "ERROR: Cannot Add NULLPTR Direction Light Shader" << std::endl;
		return false;
	}

	_directionLightShaders.push_back(a_Shader);
	return true;
}
bool Renderer::addSpotLightShader(AssetType::Shader* a_Shader)
{
	if (!a_Shader) {
		std::cout << "ERROR: Cannot Add NULLPTR Spot Light Shader" << std::endl;
		return false;
	}

	_spotLightShaders.push_back(a_Shader);
	return true;
}

bool Renderer::startShaderProgram(GLuint a_VertShader, GLuint a_FragShader)
{
	// Delete Previous Shader Program
	if (m_ShaderProgramID != GL_ZERO)
	{
		glUseProgram(GL_ZERO);
		glDeleteShader(m_ShaderProgramID);
	}

	// Create new Shader Program
	m_ShaderProgramID = glCreateProgram();

	// Attach & Link Shaders to Shader Program
	glAttachShader(m_ShaderProgramID, a_VertShader);
	glAttachShader(m_ShaderProgramID, a_FragShader);

	glLinkProgram(m_ShaderProgramID);

	// Validate Shader Program
	glValidateProgram(m_ShaderProgramID);

	GLint validationStatus;  glGetProgramiv(m_ShaderProgramID, GL_VALIDATE_STATUS, &validationStatus);
	if (validationStatus == GL_FALSE)
	{
		std::cout << "ERROR: Could not Validate Shader Program" << std::endl;

		return false;
	}

	// Use Shader Program
	glUseProgram(m_ShaderProgramID);

	return true;
}

/**
 * Render the Geometry Data onto the Geometry Buffer.
 * 
 * \return Whether the GBuffer could be Rendered
 */
bool Renderer::renderGBuffer(AssetType::Shader* a_ProjectionShader)
{
	// Use Geometry Frame Buffer as Render Target
	if (!m_GeometryBuffer->UseFrameBufferObject(Eigen::Vector4f(1.0f, 0.4f, 0.3f, 1.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)) {
		std::cout << "ERROR: Could not Use Geometry Buffer as Framebuffer" << std::endl;
		return false;
	}

	// Loop Through and Render each of the Geometry Batches
	for (auto geometryBatch : m_GeometryBatches)
	{
		// Create & Load Geometry Batch Shader Program
		if (!startShaderProgram(a_ProjectionShader->ShaderID(), geometryBatch->Shader()->ShaderID())) {
			std::cout << "ERROR: Could not Start Shader Program" << std::endl;
			continue;
		}

		// Render All Geometry Batch Instances to GBuffer Textures
		if (!geometryBatch->DrawBatchObjects(m_TransformBuffer)) {
			std::cout << "ERROR: Could not Draw Geometry Batch Objects" << std::endl;
		}
	}

	return true;
}
/**
 * Render the Scene without Transparent Objects.
 * 
 * \return 
 */
bool Renderer::renderDeferredBuffer()
{
	// Use Deferred Frame Buffer as Render Target
	if (!m_DeferredBuffer->UseFrameBufferObject(Eigen::Vector4f::Zero(), GL_COLOR_BUFFER_BIT)) {
		std::cout << "ERROR: Could not Use Deferred Buffer as Framebuffer" << std::endl;
		return false;
	}

	// Load Geometry Buffer Textures
	for (GLuint i = 0; i < m_GeometryBuffer->GetNumColourAttachments(); i++)
	{	auto GBufferTexture = m_GeometryBuffer->GetColourAttachmentTexture(i);

		if (!GBufferTexture || !GBufferTexture->BindTexture(i)) {
			std::cout << "ERROR: Could not Bind Geometry Buffer Texture #" << i << std::endl;
		}
	}

	// Render Unlit Surfaces
	for (size_t i = 0; i < m_UnlitShaders.size(); i++)
	{	auto unlitShader = m_UnlitShaders[i];

		// Create & Load Unlit Shader Program
		if (!unlitShader || !startShaderProgram(k_PassThroughVertexShader->ShaderID(), unlitShader->ShaderID())) {
			std::cout << "ERROR: Could not Start Unlit Shader Program #" << i << std::endl;
			continue;
		}

		// Draw to Framebuffer Display Mesh
		renderFramebufferDisplayMesh();
	}

	// Point Light Calculations
	for (size_t i = 0; i < _pointLightShaders.size(); i++)
	{	auto lightShader = _pointLightShaders[i];

		// Create & Load Point Light Shader Program
		if (!lightShader || !startShaderProgram(k_PassThroughVertexShader->ShaderID(), lightShader->ShaderID())) {
			std::cout << "ERROR: Could not Start Point Light Shader Program #" << i << std::endl;
			continue;
		}

		// Render Point Lighting
		for (size_t j = 0; j < _pointLights.size(); j++)
		{	auto light = _pointLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight()) {
				std::cout << "ERROR: Could not Load Point Light #" << j << std::endl;
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderFramebufferDisplayMesh();
		}
	}
	// Direction Light Calculations
	for (size_t i = 0; i < _directionLightShaders.size(); i++)
	{
		auto lightShader = _directionLightShaders[i];

		// Create & Load Direction Light Shader Program
		if (!lightShader || !startShaderProgram(k_PassThroughVertexShader->ShaderID(), lightShader->ShaderID())) {
			std::cout << "ERROR: Could not Start Direction Light Shader Program #" << i << std::endl;
			continue;
		}

		// Render Direction Lighting
		for (size_t j = 0; j < _directionLights.size(); j++)
		{
			auto light = _directionLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight()) {
				std::cout << "ERROR: Could not Load Direction Light #" << j << std::endl;
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderFramebufferDisplayMesh();
		}
	}
	// Spot Light Calculations
	for (size_t i = 0; i < _spotLightShaders.size(); i++)
	{
		auto lightShader = _spotLightShaders[i];

		// Create & Load Spot Light Shader Program
		if (!lightShader || !startShaderProgram(k_PassThroughVertexShader->ShaderID(), lightShader->ShaderID())) {
			std::cout << "ERROR: Could not Start Spot Light Shader Program #" << i << std::endl;
			continue;
		}

		// Render Spot Lighting
		for (size_t j = 0; j < _spotLights.size(); j++)
		{
			auto light = _spotLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight()) {
				std::cout << "ERROR: Could not Load Spot Light #" << j << std::endl;
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderFramebufferDisplayMesh();
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
		m_GeometryBatches.push_back(geometryBatch);
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
bool Renderer::renderFrame(AssetType::Shader* a_ProjectionShader)
{
	// Clear Default Framebuffer Values
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.5f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Render Scene to the Geometry Buffer
	if (!m_GeometryBuffer || !renderGBuffer(a_ProjectionShader)) {
		std::cout << "ERROR: Could not Render to Geometry Buffer" << std::endl;
		return false;
	}


	// TODO:
	// Render Shadow Maps for All Lights


	// Enable Additive Blending for Overlapping Fragments
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Render to Deferred Buffer
	if (!m_DeferredBuffer || !renderDeferredBuffer()) {
		std::cout << "ERROR: Could not Render to Deferred Buffer" << std::endl;
		return false;
	}

	// Disable Additive Blending for Overlapping Fragments
	glDisable(GL_BLEND);


	// TODO:
	// Render Transparency Buffer


	// Display Frame to Scene
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Load Frame Buffer Display Shader & Output Texture 
	if (!startShaderProgram(k_PassThroughVertexShader->ShaderID(), k_FrameBufferDisplayShader->ShaderID())) {
		std::cout << "ERROR: Could not Start Framebuffer Display Shader Program" << std::endl;
		return false;
	}

	auto FBOutputTexture = m_DeferredBuffer->GetColourAttachmentTexture(0);
	if (!FBOutputTexture || !FBOutputTexture->BindTexture(0)) {
		std::cout << "ERROR: Could not Bind Framebuffer Output Texture" << std::endl;
	}


	// Draw to Screen Display Mesh
	if (!k_ScreenDisplayMesh->BindVertexArrayObject()) {
		std::cout << "ERROR: Could not Bind Screen Display Mesh's VAO" << std::endl;
		return false;
	}
	glDrawElements(GL_TRIANGLES, k_ScreenDisplayMesh->NumIndices(), GL_UNSIGNED_INT, nullptr);


	// Reset Renderer for Next Frame
	resetRendererFrame();

	return true;
}
void Renderer::renderFramebufferDisplayMesh()
{
	if (!k_FrameBufferDisplayMesh->BindVertexArrayObject())
	{
		std::cout << "ERROR: Could not Bind Framebuffer Display Mesh's VAO" << std::endl;
		return;
	}
	glDrawElements(GL_TRIANGLES, k_FrameBufferDisplayMesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
}
}