#include "LoggingSystem.h"

#include "Renderer.h"

#include "UniformBuffer.h"
#include "FrameBuffer.h"

#include "Light.h"
#include "RenderBatch.h"

#include <iostream>
#include <utility>
#include <functional>

#include <format>

// Asset Headers
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

#include "MeshLibrary.h"
#include "ShaderLibrary.h"

// Entity Headers
#include "CameraComponent.h"

// Static Function & Parameters
namespace MCK::Rendering {
Renderer* Renderer::k_Instance = nullptr;

AssetType::Mesh* Renderer::k_DisplayScreenMesh = nullptr;
AssetType::Shader* Renderer::k_DisplayScreenShader = nullptr;

UniformBuffer* Renderer::k_DisplayScreenUniforms = nullptr;

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
	// Load Display Screen Mesh
	if (!MeshLibrary::GetMesh(MeshEnum::__MCK_DISPLAY_SCREEN, k_DisplayScreenMesh)) {
		Logger::log("Could not Get Display Screen Mesh", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load Display Screen Shader
	if (!ShaderLibrary::GetShader(ShaderEnum::__MCK_FRAMEBUFFER_DISPLAY, k_DisplayScreenShader)) {
		Logger::log("Could not Get Display Screen Shader", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Display Screen Uniform Buffer
	k_DisplayScreenUniforms = new UniformBuffer();

	// Add Position to Display Screen Uniform Buffer
	if (!k_DisplayScreenUniforms->AddVec3BufferUniform("position", Eigen::Vector3f::Zero())) {
		Logger::log("Could not Add Position to Display Screen Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Front to Display Screen Uniform Buffer
	if (!k_DisplayScreenUniforms->AddVec3BufferUniform("front", Eigen::Vector3f(0.0f, 0.0f, -1.0f))) {
		Logger::log("Could not Add Front to Display Screen Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Up to Display Screen Uniform Buffer
	if (!k_DisplayScreenUniforms->AddVec3BufferUniform("up", Eigen::Vector3f(0.0f, 1.0f, 0.0f))) {
		Logger::log("Could not Add Up to Display Screen Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Add Camera Projection Matrix to Display Screen Uniform Buffer
	if (!k_DisplayScreenUniforms->AddMat4BufferUniform("cameraProjectionMatrix", Eigen::Matrix4f::Identity())) {
		Logger::log("Could not Add Camera Projection Matrix to Display Screen Uniform Buffer", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Display Screen Uniform Buffer Object
	if (!k_DisplayScreenUniforms->CreateUniformBufferObject())
	{
		Logger::log("Could not Create Display Screen Uniform Buffer Object", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	return Instance()->initialiseRenderer(a_ScreenWidth, a_ScreenHeight);;
}
bool Renderer::ResizeRenderer(GLuint screenWidth, GLuint screenHeight)
{
	return Instance()->resizeRenderer(screenWidth, screenHeight);
}
/**  */
void Renderer::ClearRenderer()
{
	if (k_Instance)
	{
		delete k_Instance;
		k_Instance = nullptr;
	}

	k_DisplayScreenMesh = nullptr;
	k_DisplayScreenShader = nullptr;

	if (k_DisplayScreenUniforms)
	{
		delete k_DisplayScreenUniforms;
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
bool Renderer::QueueMeshInstance(const EntitySystem::TransformComponent& a_Transform,
	AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material,
	bool a_HasTransparency, bool a_isAnimated)
{
	bool result = false;
	if (!a_HasTransparency)
	{
		result = Instance()->queueGeometryBatchInstance(a_Transform, a_Mesh, a_Shader, a_Material, a_isAnimated);
	}

	return result;
}

bool Renderer::QueuePointLight(PointLight* a_PointLight)
{
	return Instance()->queuePointLight(a_PointLight);
}
bool Renderer::QueueDirectionLight(DirectionLight* a_DirectionLight)
{
	return Instance()->queueDirectionLight(a_DirectionLight);
}
bool Renderer::QueueSpotLight(SpotLight* a_SpotLight)
{
	return Instance()->queueSpotLight(a_SpotLight);
}

bool Renderer::UseCamera(const EntitySystem::CameraComponent& a_Camera)
{
	return Instance()->useCamera(a_Camera);
}

/**  */
void Renderer::SetCentrePosition(Eigen::Vector3f a_CentrePosition)
{
	Instance()->setCentrePosition(a_CentrePosition);
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
	m_CameraBuffer(nullptr), m_MeshTransformBuffer(nullptr),
	m_CentrePosition(Eigen::Vector3f::Zero()) {}
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
	if (m_CameraBuffer) {
		Logger::log("Cannot Recreate Camera Uniform Buffer", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
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
	m_GeometryBuffer = new FrameBuffer(a_ScreenWidth, a_ScreenHeight);

	// Add Geometry Buffer Colour Attachments
	if (!m_GeometryBuffer->AddUIntColourAttachment())
	{// ID #0 is Reserved for the Lighting Shader ID Map
		resetRenderer();

		Logger::log("Could not Add Geometry Buffer Colour Attachment Texture #0", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	for (int i = 1; i < 31; i++) {
	if (!m_GeometryBuffer->AddFloatColourAttachment())
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
	m_DeferredBuffer = new FrameBuffer(a_ScreenWidth, a_ScreenHeight);

	// Add Output Colour Attachment Texture
	if (!m_DeferredBuffer->AddFloatColourAttachment())
	{
		resetRenderer();

		Logger::log("Could not Add Deferred Buffer Output Colour Attachment Texture", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Add Depth Buffer Texture
	if (!m_DeferredBuffer->AddDepthBufferTexture())
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
bool Renderer::resizeRenderer(GLuint a_ScreenWidth, GLuint a_ScreenHeight)
{
	// Resize Depth Buffer Texture
	m_DepthBufferTexture->ResizeTexture(a_ScreenWidth, a_ScreenHeight);

	// Resize Geometry Buffer
	m_GeometryBuffer->ResizeFramebuffer(a_ScreenWidth, a_ScreenHeight);
	// Resize Deferred Buffer
	m_DeferredBuffer->ResizeFramebuffer(a_ScreenWidth, a_ScreenHeight);

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
 * Render the Geometry Data onto a Frame Buffer.
 *
 * \return Whether the Geometry could be Rendered
 */
bool Renderer::renderGeometry(FrameBuffer* a_Framebuffer, bool a_DepthOnly)
{
	GLuint clearFlags = GL_DEPTH_BUFFER_BIT;
	if (!a_DepthOnly)
		clearFlags |= GL_COLOR_BUFFER_BIT;

	// Use Frame Buffer as Geometry Render Target
	if (!a_Framebuffer->UseFrameBufferObject(Eigen::Vector4f::Zero(), clearFlags)) {
		Logger::log("Could not Use Framebuffer Buffer for Geometry", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Loop Through and Render each of the Geometry Batches
	for (auto geometryBatch : m_GeometryBatches) {
		// Render All Geometry Batch Instances to Textures
		if (!geometryBatch->DrawBatchObjects(m_MeshTransformBuffer, a_DepthOnly)) {
			Logger::log("Could not Draw Geometry Batch Objects", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
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
		if (!unlitShader || !unlitShader->UseShaderProgram()) {
			Logger::log(std::format("Could not Start Unlit Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Draw to Framebuffer Display Mesh
		renderDisplayScreen();
	}

	// Point Light Calculations
	for (size_t i = 0; i < _pointLightShaders.size(); i++)
	{	auto lightShader = _pointLightShaders[i];

		// Start Point Light Shader Program
		if (!lightShader || !lightShader->UseShaderProgram()) {
			Logger::log(std::format("Could not Start Point Light Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Render Point Lighting
		for (size_t j = 0; j < _pointLights.size(); j++)
		{	auto light = _pointLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight(Eigen::Vector3f::Zero())) {
				Logger::log(std::format("Could not Load Point Light #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderDisplayScreen();
		}
	}
	// Direction Light Calculations
	for (size_t i = 0; i < _directionLightShaders.size(); i++)
	{
		auto lightShader = _directionLightShaders[i];

		// Start Direction Light Shader Program
		if (!lightShader || !lightShader->UseShaderProgram()) {
			Logger::log(std::format("Could not Start Direction Light Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Render Direction Lighting
		for (size_t j = 0; j < _directionLights.size(); j++)
		{
			auto light = _directionLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight(Eigen::Vector3f::Zero())) {
				Logger::log(std::format("Could not Load Direction Light #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderDisplayScreen();
		}
	}
	// Spot Light Calculations
	for (size_t i = 0; i < _spotLightShaders.size(); i++)
	{
		auto lightShader = _spotLightShaders[i];

		// Start Spot Light Shader Program
		if (!lightShader || !lightShader->UseShaderProgram()) {
			Logger::log(std::format("Could not Start Spot Light Shader Program #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}

		// Render Spot Lighting
		for (size_t j = 0; j < _spotLights.size(); j++)
		{
			auto light = _spotLights[j];

			// Load Light Uniforms
			if (!light || !light->UseLight(Eigen::Vector3f::Zero())) {
				Logger::log(std::format("Could not Load Spot Light #{}", i), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
				continue;
			}

			// Draw to Framebuffer Display Mesh
			renderDisplayScreen();
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
bool Renderer::queueGeometryBatchInstance(const EntitySystem::TransformComponent& a_Transform, AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material, bool a_isAnimated)
{
	RenderBatch* geometryBatch = nullptr;

	// Find Existing Geometry Batch with Same Mesh & Shader
	for (auto batch : m_GeometryBatches)
	{
		if (batch->Mesh() != a_Mesh)
			continue;

		if (batch->Shader() != a_Shader)
			continue;

		if (batch->IsAnimated() != a_isAnimated)
			continue;

		geometryBatch = batch;
		break;
	}
	// Create New Geometry Batch if no Match Exists
	if (!geometryBatch)
	{
		geometryBatch = new RenderBatch(a_Mesh, a_Shader, a_isAnimated);
		m_GeometryBatches.push_back(geometryBatch);
	}

	// Add Mesh Instance to Geometry Batch
	geometryBatch->AddBatchInstance(a_Transform, a_Material);
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

/**  */
bool Renderer::useCamera(const EntitySystem::CameraComponent& a_Camera)
{
	// Compute Camera ViewProjection Matrix
	Eigen::Matrix4f cameraViewProjectionMatrix = a_Camera.GetProjectionMatrix() * a_Camera.GetCameraViewMatrix();

	// Set Camera Uniform Buffer Values
	if (!m_CameraBuffer->SetVec3BufferUniform("position", a_Camera.Position())) {
		Logger::log("Could not Set Value for Camera Position", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}
	if (!m_CameraBuffer->SetVec3BufferUniform("front", a_Camera.FrontDirection())) {
		Logger::log("Could not Set Value for Camera Front Direction", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}
	if (!m_CameraBuffer->SetVec3BufferUniform("up", a_Camera.UpDirection())) {
		Logger::log("Could not Set Value for Camera Up Direction", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}
	if (!m_CameraBuffer->SetMat4BufferUniform("cameraProjectionMatrix", cameraViewProjectionMatrix)) {
		Logger::log("Could not Set Value for Camera ViewProjection Matrix", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}

/**  */
void Renderer::setCentrePosition(Eigen::Vector3f a_CentrePosition)
{
	m_CentrePosition = a_CentrePosition;
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

	glEnable(GL_DEPTH_TEST);

	// Render Scene to the Geometry Buffer
	if (!m_GeometryBuffer || !renderGeometry(m_GeometryBuffer, false)) {
		Logger::log("Could not Render to Geometry Buffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// TODO:
	// Render Shadow Maps for All Lights
	for (auto light : _directionLights)
	{
		light->BindShadowRendererCamera(m_CentrePosition);
		if (!light->ShadowRenderer() || !renderGeometry(light->ShadowRenderer(), true)) {
			Logger::log("Could not Render Light Shadow Map to Buffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			continue;
		}
	}

	glDisable(GL_DEPTH_TEST);


	// Enable Additive Blending for Overlapping Fragments
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	k_DisplayScreenUniforms->BindUniformBufferObject(0);
	m_MeshTransformBuffer->SetMat4BufferUniform("transformMatrix", Eigen::Matrix4f::Identity());

	// Render to Deferred Buffer
	if (!m_DeferredBuffer || !renderDeferredBuffer()) {
		Logger::log("Could not Render to Deferred Buffer", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Disable Additive Blending for Overlapping Fragments
	glDisable(GL_BLEND);


	// TODO:
	m_CameraBuffer->BindUniformBufferObject(0);
	
	// Render Transparency Buffer


	// Display Frame to Scene
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	k_DisplayScreenUniforms->BindUniformBufferObject(0);
	m_MeshTransformBuffer->SetMat4BufferUniform("transformMatrix", Eigen::Matrix4f::Identity());

	// Load Framebuffer Output Texture
	auto FBOutputTexture = m_DeferredBuffer->GetColourAttachmentTexture(0);
	if (!FBOutputTexture || !FBOutputTexture->BindTexture(0)) {
		Logger::log("Could not Bind Framebuffer Output Texture", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
	}

	// Load Frame Buffer Display Shader
	if (!k_DisplayScreenShader->UseShaderProgram()) {
		Logger::log("Could not Start Framebuffer Display Shader Program", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Draw Framebuffer Texture to Screen
	renderDisplayScreen();

	// Reset Renderer for Next Frame
	resetRendererFrame();

	return true;
}
void Renderer::renderDisplayScreen()
{
	// Bind Display Screen Mesh's VAO to GPU
	if (!k_DisplayScreenMesh->BindVertexArrayObject()) {
		Logger::log("Could not Bind Display Screen Mesh's VAO", Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return;
	}
	//glDrawArrays(GL_TRIANGLES, 0, k_DisplayScreenMesh->NumVertices() * 3);
	glDrawElements(GL_TRIANGLES, k_DisplayScreenMesh->NumIndices(), GL_UNSIGNED_INT, nullptr);
}
}