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

	static AssetType::Shader* k_FramebufferDisplayShader;

	static AssetType::Mesh* k_DisplayScreen;

// Member Variables
private:	
	GLuint m_ShaderProgramID;

	// Permanent Renderer Objects
	FrameBuffer* m_GeometryBuffer;
	FrameBuffer* m_DeferredBuffer;

	AssetType::Texture* m_DepthBufferTexture;

	UniformBuffer* m_TransformBuffer;

	std::vector<AssetType::Shader*> m_UnlitShaders;
	std::vector<AssetType::Shader*> _pointLightShaders;
	std::vector<AssetType::Shader*> _directionLightShaders;
	std::vector<AssetType::Shader*> _spotLightShaders;

	// Per Frame Renderer Objects
	std::vector<RenderBatch*> m_GeometryBatches;

	std::vector<PointLight*> _pointLights;
	std::vector<DirectionLight*> _directionLights;
	std::vector<SpotLight*> _spotLights;

// Member Methods
private:
	bool initialiseRenderer(GLuint a_ScreenWidth, GLuint a_ScreenHeight);
	void resetRenderer();
	void resetRendererFrame();

	bool addUnlitShader(AssetType::Shader* a_Shader);
	bool addPointLightShader(AssetType::Shader* a_Shader);
	bool addDirectionLightShader(AssetType::Shader* a_Shader);
	bool addSpotLightShader(AssetType::Shader* a_Shader);

	bool renderGBuffer();
	bool renderDeferredBuffer();

	// Functions to tell the Renderer what needs to be Rendered
	bool queueGeometryBatchInstance(AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material, Eigen::Matrix4f a_Transform);

	bool queuePointLight(PointLight* pointLight);
	bool queueDirectionLight(DirectionLight* directionLight);
	bool queueSpotLight(SpotLight* spotLight);

	bool renderFrame();
	void renderDisplayMesh();

public:
	static bool InitialiseRenderer(GLuint screenWidth, GLuint screenHeight);
	static void ClearRenderer();

	static bool AddUnlitShader(AssetType::Shader* a_Shader);
	static bool AddPointLightShader(AssetType::Shader* a_Shader);
	static bool AddDirectionLightShader(AssetType::Shader* a_Shader);
	static bool AddSpotLightShader(AssetType::Shader* a_Shader);

	static bool QueueMeshInstance(Eigen::Matrix4f a_Transform,
		AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material,
		bool a_HasTransparency);

	static bool QueuePointLight(PointLight* a_PointLight);
	static bool QueueDirectionLight(DirectionLight* a_DirectionLight);
	static bool QueueSpotLight(SpotLight* a_SpotLight);

	static bool RenderFrame();
};
}
