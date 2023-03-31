#include "Renderer.h"

#include "UniformBuffer.h"
#include "FrameBuffer.h"

#include "Light.h"
#include "RenderBatch.h"

#include <iostream>
#include <utility>
#include <functional>

// Logging Headers
#include "LoggingSystem.h"
#include <format>

// Asset Headers
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

#include "MeshLibrary.h"
#include "ShaderLibrary.h"

// Static Function & Parameters
namespace MCK::Rendering {
Renderer* Renderer::k_Instance = nullptr;

AssetType::Shader* Renderer::k_FramebufferDisplayShader = nullptr;

AssetType::Mesh* Renderer::k_DisplayScreen = nullptr;

Renderer* Renderer::Instance()
{
	// Create Renderer Instance if it Doesn't Exist Yet
	if (!k_Instance)
	{
		k_Instance = new Renderer();
	}

	return k_Instance;
}

/**
* Initialises the Renderer.
*
* \param a_ScreenWidth: The Target Screen's Width
* \param a_ScreenHeight: The Target Screen's Height
* \return Whether the Renderer was Successfully Initialised
*/
bool Renderer::InitialiseRenderer(GLuint a_ScreenWidth, GLuint a_ScreenHeight)
{
	// Load Framebuffer Display Shader
	if (!ShaderLibrary::GetShader(ShaderEnum::__MCK_FRAMEBUFFER_DISPLAY, k_FramebufferDisplayShader)) {
		Logger::log("Could not Get Framebuffer Display Shader", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load Framebuffer Display Mesh
	if (!MeshLibrary::GetMesh(MeshEnum::__MCK_DISPLAY_SCREEN, k_DisplayScreen)) {
		Logger::log("Could not Get Display Screen Mesh", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	return Instance()->initialiseRenderer(a_ScreenWidth, a_ScreenHeight);;
}
/**  */
void Renderer::ClearRenderer()
{
	if (k_Instance)
	{
		delete k_Instance;
		k_Instance = nullptr;
	}

	k_FramebufferDisplayShader = nullptr;
	k_DisplayScreen = nullptr;
}

/**
 * Add Unlit Deferred Shader to the Renderer.
 * 
 * \param a_Shader: The Unlit Shader to be Added
 * \return Whether the Shader was Successfully Added
 */
bool Renderer::AddUnlitShader(AssetType::Shader* a_Shader)
{
	bool result = Instance()->addUnlitShader(a_Shader);
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
	bool result = Instance()->addPointLightShader(a_Shader);
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
	bool result = Instance()->addDirectionLightShader(a_Shader);
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
	bool result = Instance()->addSpotLightShader(a_Shader);
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
		result = Instance()->queueGeometryBatchInstance(a_Mesh, a_Shader, a_Material, a_Transform);
	}

	return result;
}

/**  */
bool Renderer::RenderFrame()
{
	return Instance()->renderFrame();
}
}

namespace MCK::Rendering {
Renderer::Renderer() :
	m_ShaderProgramID(GL_ZERO),
	m_GeometryBuffer(nullptr), m_DeferredBuffer(nullptr),
	m_DepthBufferTexture(nullptr),
	m_MeshTransformBuffer(nullptr), m_CameraBuffer(nullptr) {}
Renderer::~Renderer()
{
	resetRenderer();
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
		Logger::log("Cannot Recreate Geometry Framebuffer", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}
	if (m_DeferredBuffer) {
		Logger::log("Cannot Recreate Deferred Framebuffer", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}
	if (m_DepthBufferTexture) {
		Logger::log("Cannot Recreate Depth Buffer Texture", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}
	if (m_MeshTransformBuffer) {
		Logger::log("Cannot Recreate Transform Uniform Buffer", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}


	// Create the Depth Buffer Texture
	m_DepthBufferTexture = new AssetType::Texture();

	if (!m_DepthBufferTexture->GenerateDepthTexture(a_ScreenWidth, a_ScreenHeight)) {
		resetRenderer();

		Logger::log("Could not Generate Depth Buffer Texture", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}


	// Create the Geometry Frame Buffer
	m_GeometryBuffer = new FrameBuffer("Geometry Buffer");

	// Add Geometry Buffer Colour Attachments
	if (!m_GeometryBuffer->AddFloatColourAttachment(a_ScreenWidth, a_ScreenHeight))
	{// ID #0 is Reserved for the Lighting Shader ID Map
		resetRenderer();

		Logger::log("Could not Add Geometry Buffer Colour Attachment Texture #0", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	for (int i = 1; i < 31; i++) {
	if (!m_GeometryBuffer->AddFloatColourAttachment(a_ScreenWidth, a_ScreenHeight))
	{// Other ID #s are for General Purpose
		resetRenderer();

		Logger::log(std::format("Could not Add Geometry Buffer Colour Attachment Texture #{}", i), Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}}

	// Assign Depth Buffer Texture Attachment
	if (!m_GeometryBuffer->AssignExternalDepthBufferTexture(m_DepthBufferTexture))
	{
		resetRenderer();

		Logger::log("Could not Assign Depth Buffer Texture to Geometry Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Geometry Frame Buffer Object
	if (!m_GeometryBuffer->CreateFrameBuffer())
	{
		resetRenderer();

		Logger::log("Could not Create Geometry Frame Buffer Object", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}


	// Create the Deferred Frame Buffer
	m_DeferredBuffer = new FrameBuffer("Deferred Buffer");

	// Add Output Colour Attachment Texture
	if (!m_DeferredBuffer->AddFloatColourAttachment(a_ScreenWidth, a_ScreenHeight))
	{
		resetRenderer();

		Logger::log("Could not Add Deferred Buffer Output Colour Attachment Texture", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Add Depth Buffer Texture
	if (!m_DeferredBuffer->AddDepthBufferTexture(a_ScreenWidth, a_ScreenHeight))
	{
		resetRenderer();

		Logger::log("Could not Add Deferred Buffer Depth Buffer Texture", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Deferred Frame Buffer Object
	if (!m_DeferredBuffer->CreateFrameBuffer())
	{
		resetRenderer();

		Logger::log("Could not Create Deferred Frame Buffer Object", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create the Camera Uniform Buffer
	m_CameraBuffer = new UniformBuffer();
	
	// Add Position to Camera Uniform Buffer
	if (!m_CameraBuffer->AddVec3BufferUniform("position", Eigen::Vector3f::Zero())) {
		resetRenderer();

		Logger::log("Could not Add Position to Camera Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Front to Camera Uniform Buffer
	if (!m_CameraBuffer->AddVec3BufferUniform("front", Eigen::Vector3f(0.0f, 0.0f, -1.0f))) {
		resetRenderer();

		Logger::log("Could not Add Front to Camera Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Up to Camera Uniform Buffer
	if (!m_CameraBuffer->AddVec3BufferUniform("up", Eigen::Vector3f(0.0f, 1.0f, 0.0f))) {
		resetRenderer();

		Logger::log("Could not Add Up to Camera Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Camera Projection Matrix to Camera Uniform Buffer
	if (!m_CameraBuffer->AddMat4BufferUniform("cameraProjectionMatrix", Eigen::Matrix4f::Identity())) {
		resetRenderer();

		Logger::log("Could not Add Camera Projection Matrix to Camera Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Camera Uniform Buffer Object
	if (!m_CameraBuffer->CreateUniformBufferObject())
	{
		resetRenderer();

		Logger::log("Could not Create Camera Uniform Buffer Object", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create the Transform Uniform Buffer
	m_MeshTransformBuffer = new UniformBuffer();

	// Add Position to Mesh Transform Uniform Buffer
	if (!m_MeshTransformBuffer->AddVec3BufferUniform("position", Eigen::Vector3f::Zero())) {
		resetRenderer();

		Logger::log("Could not Add Position to Mesh Transform Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Rotation to Mesh Transform Uniform Buffer
	if (!m_MeshTransformBuffer->AddVec3BufferUniform("rotation", Eigen::Vector3f::Zero())) {
		resetRenderer();

		Logger::log("Could not Add Rotation to Mesh Transform Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Scale to Mesh Transform Uniform Buffer
	if (!m_MeshTransformBuffer->AddVec3BufferUniform("scale", Eigen::Vector3f::Ones())) {
		resetRenderer();

		Logger::log("Could not Add Scale to Mesh Transform Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Transform Matrix to Mesh Transform Uniform Buffer
	if (!m_MeshTransformBuffer->AddMat4BufferUniform("transformMatrix", Eigen::Matrix4f::Identity())) {
		resetRenderer();

		Logger::log("Could not Add Transform Matrix to Mesh Transform Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Transform Uniform Buffer Object
	if (!m_MeshTransformBuffer->CreateUniformBufferObject())
	{
		resetRenderer();

		Logger::log("Could not Create Transform Uniform Buffer Object", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
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
	if (m_CameraBuffer) {
		delete m_CameraBuffer;
		m_CameraBuffer = nullptr;
	}
	if (m_MeshTransformBuffer) {
		delete m_MeshTransformBuffer;
		m_MeshTransformBuffer = nullptr;
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
		Logger::log("Cannot Add NULLPTR Unlit Shader", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	m_UnlitShaders.push_back(a_Shader);
	return true;
}
bool Renderer::addPointLightShader(AssetType::Shader* a_Shader)
{
	if (!a_Shader) {
		Logger::log("Cannot Add NULLPTR Point Light Shader", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	_pointLightShaders.push_back(a_Shader);
	return true;
}
bool Renderer::addDirectionLightShader(AssetType::Shader* a_Shader)
{
	if (!a_Shader) {
		Logger::log("Cannot Add NULLPTR Direction Light Shader", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	_directionLightShaders.push_back(a_Shader);
	return true;
}
bool Renderer::addSpotLightShader(AssetType::Shader* a_Shader)
{
	if (!a_Shader) {
		Logger::log("Cannot Add NULLPTR Spot Light Shader", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	_spotLightShaders.push_back(a_Shader);
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
	if (!m_GeometryBuffer->UseFrameBufferObject(Eigen::Vector4f::Zero(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)) {
		Logger::log("Could not Use Geometry Buffer as Framebuffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Loop Through and Render each of the Geometry Batches
	for (auto geometryBatch : m_GeometryBatches) {
	// Render All Geometry Batch Instances to GBuffer Textures
	if (!geometryBatch->DrawBatchObjects(m_MeshTransformBuffer)) {
		Logger::log("Could not Draw Geometry Batch Objects", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
	}}

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
		Logger::log("Could not Use Deferred Buffer as Framebuffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load Geometry Buffer Textures
	for (GLuint i = 0; i < m_GeometryBuffer->GetNumColourAttachments(); i++)
	{	auto GBufferTexture = m_GeometryBuffer->GetColourAttachmentTexture(i);

		if (!GBufferTexture || !GBufferTexture->BindTexture(i)) {
			Logger::log(std::format("Could not Bind Geometry Buffer Texture #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		}
	}

	// Render Unlit Surfaces
	for (size_t i = 0; i < m_UnlitShaders.size(); i++)
	{	auto unlitShader = m_UnlitShaders[i];

		// Start Unlit Shader Program
		if (!unlitShader || !unlitShader->UsePassthroughProgram()) {
			Logger::log(std::format("Could not Start Unlit Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Draw to Framebuffer Display Mesh
		renderDisplayMesh();
	}

	// Point Light Calculations
	for (size_t i = 0; i < _pointLightShaders.size(); i++)
	{	auto lightShader = _pointLightShaders[i];

		// Start Point Light Shader Program
		if (!lightShader || !lightShader->UsePassthroughProgram()) {
			Logger::log(std::format("Could not Start Point Light Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Render Point Lighting
		for (size_t j = 0; j < _pointLights.size(); j++)
		{	auto light = _pointLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight()) {
				Logger::log(std::format("Could not Load Point Light #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderDisplayMesh();
		}
	}
	// Direction Light Calculations
	for (size_t i = 0; i < _directionLightShaders.size(); i++)
	{
		auto lightShader = _directionLightShaders[i];

		// Start Direction Light Shader Program
		if (!lightShader || !lightShader->UsePassthroughProgram()) {
			Logger::log(std::format("Could not Start Direction Light Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Render Direction Lighting
		for (size_t j = 0; j < _directionLights.size(); j++)
		{
			auto light = _directionLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight()) {
				Logger::log(std::format("Could not Load Direction Light #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderDisplayMesh();
		}
	}
	// Spot Light Calculations
	for (size_t i = 0; i < _spotLightShaders.size(); i++)
	{
		auto lightShader = _spotLightShaders[i];

		// Start Spot Light Shader Program
		if (!lightShader || !lightShader->UsePassthroughProgram()) {
			Logger::log(std::format("Could not Start Spot Light Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Render Spot Lighting
		for (size_t j = 0; j < _spotLights.size(); j++)
		{
			auto light = _spotLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight()) {
				Logger::log(std::format("Could not Load Spot Light #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderDisplayMesh();
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
bool Renderer::renderFrame()
{
	// Clear Default Framebuffer Values
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.5f, 0.4f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_CameraBuffer->BindUniformBufferObject(0);
	m_MeshTransformBuffer->BindUniformBufferObject(1);
	std::cout << *m_MeshTransformBuffer->GetVec3BufferUniform("scale") << std::endl;

	// Render Scene to the Geometry Buffer
	if (!m_GeometryBuffer || !renderGBuffer()) {
		Logger::log("Could not Render to Geometry Buffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}


	// TODO:
	// Render Shadow Maps for All Lights


	// Enable Additive Blending for Overlapping Fragments
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	// Render to Deferred Buffer
	if (!m_DeferredBuffer || !renderDeferredBuffer()) {
		Logger::log("Could not Render to Deferred Buffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Disable Additive Blending for Overlapping Fragments
	glDisable(GL_BLEND);


	// TODO:
	// Render Transparency Buffer


	// Display Frame to Scene
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Load Framebuffer Output Texture
	auto FBOutputTexture = m_DeferredBuffer->GetColourAttachmentTexture(0);
	if (!FBOutputTexture || !FBOutputTexture->BindTexture(0)) {
		Logger::log("Could not Bind Framebuffer Output Texture", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
	}

	// Load Frame Buffer Display Shader
	if (!k_FramebufferDisplayShader->UsePassthroughProgram()) {
		Logger::log("Could not Start Framebuffer Display Shader Program", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Draw Framebuffer Texture to Screen
	renderDisplayMesh();


	// Reset Renderer for Next Frame
	resetRendererFrame();

	return true;
}
void Renderer::renderDisplayMesh()
{
	// Bind Display Mesh's VAO to GPU
	if (!k_DisplayScreen->BindVertexArrayObject())
	{
		Logger::log("Could not Bind Display Mesh's VAO", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return;
	}
	glDrawElements(GL_TRIANGLES, k_DisplayScreen->NumIndices(), GL_UNSIGNED_INT, nullptr);
}
}