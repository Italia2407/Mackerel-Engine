#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

#include <string>
#include <map>
#include <vector>

// Forward Declarations
namespace MCK {
class UniformBuffer;
class FrameBuffer;
}
namespace MCK::AssetType {
class Material;
class Texture;

class Mesh;
class Shader;
}
namespace MCK::Rendering {
class Light;
class PointLight;
class DirectionLight;
class SpotLight;

class RenderBatch;
}
namespace MCK::EntitySystem {
class CameraComponent;
class TransformComponent;
}

namespace MCK::Rendering {
class Renderer
{
private:
	// Singleton Constructor/Destructor
	Renderer();
	~Renderer();

	// Ensure Inability to Copy
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	// Singleton Instance Bookkeeping
	static Renderer* k_Instance;
	static Renderer* Instance();

	static AssetType::Mesh* k_DisplayScreenMesh;
	static AssetType::Shader* k_DisplayScreenShader;
	static UniformBuffer* k_DisplayScreenUniforms;

// Member Variables
private:	
	GLuint m_ShaderProgramID;

	// Permanent Renderer Objects
	FrameBuffer* m_GeometryBuffer;
	FrameBuffer* m_DeferredBuffer;

	AssetType::Texture* m_DepthBufferTexture;

	UniformBuffer* m_CameraBuffer;
	UniformBuffer* m_MeshTransformBuffer;

	std::vector<AssetType::Shader*> m_UnlitShaders;
	std::vector<AssetType::Shader*> _pointLightShaders;
	std::vector<AssetType::Shader*> _directionLightShaders;
	std::vector<AssetType::Shader*> _spotLightShaders;

	// Per Frame Renderer Objects
	std::vector<RenderBatch*> m_GeometryBatches;

	std::vector<PointLight*> _pointLights;
	std::vector<DirectionLight*> _directionLights;
	std::vector<SpotLight*> _spotLights;

	// The Centre Position around which Shadow Mapping is Performed
	Eigen::Vector3f m_CentrePosition;

// Member Methods
private:
	bool initialiseRenderer(GLuint a_ScreenWidth, GLuint a_ScreenHeight);
	bool resizeRenderer(GLuint a_ScreenWidth, GLuint a_ScreenHeight);

	void clearRenderer();
	void resetRenderer();
	void resetRendererFrame();

	bool addUnlitShader(AssetType::Shader* a_Shader);
	bool addPointLightShader(AssetType::Shader* a_Shader);
	bool addDirectionLightShader(AssetType::Shader* a_Shader);
	bool addSpotLightShader(AssetType::Shader* a_Shader);

	bool renderGeometry(FrameBuffer* a_Framebuffer, bool a_DepthOnly);
	bool renderDeferredBuffer();

	// Functions to tell the Renderer what needs to be Rendered
	bool queueGeometryBatchInstance(const EntitySystem::TransformComponent& a_Transform, AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material, bool a_isAnimated = false);

	bool queuePointLight(PointLight* pointLight);
	bool queueDirectionLight(DirectionLight* directionLight);
	bool queueSpotLight(SpotLight* spotLight);

	bool useCamera(const EntitySystem::CameraComponent& a_Camera);

	void setCentrePosition(Eigen::Vector3f a_CentrePosition);

	bool renderFrame();
	void renderDisplayScreen();

public:
	static bool InitialiseRenderer(GLuint screenWidth, GLuint screenHeight);
	static bool ResizeRenderer(GLuint screenWidth, GLuint screenHeight);
	static void ClearRenderer();

	static bool AddUnlitShader(AssetType::Shader* a_Shader);
	static bool AddPointLightShader(AssetType::Shader* a_Shader);
	static bool AddDirectionLightShader(AssetType::Shader* a_Shader);
	static bool AddSpotLightShader(AssetType::Shader* a_Shader);

	static bool QueueMeshInstance(const EntitySystem::TransformComponent& a_Transform,
		AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material,
		bool a_HasTransparency, bool a_isAnimated = false);

	static bool QueuePointLight(PointLight* a_PointLight);
	static bool QueueDirectionLight(DirectionLight* a_DirectionLight);
	static bool QueueSpotLight(SpotLight* a_SpotLight);

	static bool UseCamera(const EntitySystem::CameraComponent& a_Camera);

	static void SetCentrePosition(Eigen::Vector3f a_CentrePosition);

	static bool RenderFrame();

	static void ResetRenderer();
};
}
