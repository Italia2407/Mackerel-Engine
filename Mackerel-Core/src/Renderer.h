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

	// Do Not Copy!
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;

	// Singleton Instance Bookkeeping
	static Renderer* _instance;
	static Renderer* getInstance()
	{
		// Create New Renderer Instance if not Existing Yet
		if (!_instance)
			_instance = new Renderer();

		return _instance;
	}

	GLuint m_ShaderProgramID;

	FrameBuffer* m_GeometryBuffer;
	FrameBuffer* m_LightingBuffer;

	AssetType::Texture* m_DepthBufferTexture;

	std::vector<RenderBatch*> m_GeometryBatches;

	// Light & Shadow Rendering
	std::vector<AssetType::Shader*> _pointLightShaders;
	std::vector<AssetType::Shader*> _directionLightShaders;
	std::vector<AssetType::Shader*> _spotLightShaders;

	std::vector<PointLight*> _pointLights;
	std::vector<DirectionLight*> _directionLights;
	std::vector<SpotLight*> _spotLights;

private:
	bool initialiseRenderer(GLuint screenWidth, GLuint screenHeight);

	bool attachShader(AssetType::Shader* a_Shader);

	bool renderGBuffer();
	bool renderLightingBuffer();

	// Functions to tell the Renderer what needs to be Rendered
	bool queueGeometryBatchInstance(AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material, Eigen::Matrix4f a_Transform);

	bool queuePointLight(PointLight* pointLight);
	bool queueDirectionLight(DirectionLight* directionLight);
	bool queueSpotLight(SpotLight* spotLight);

	bool renderFrame();

public:
	/**
	 * Initialises the Renderer.
	 * 
	 * \param screenWidth: The Target Screen's Width
	 * \param screenHeight: The Target Screen's Height
	 * \return Whether the Renderer was Successfully Initialised
	 */
	static bool InitialiseRenderer(GLuint screenWidth, GLuint screenHeight)
	{
		bool result = getInstance()->initialiseRenderer(screenWidth, screenHeight);
		return result;
	}
	static void ClearRenderer()
	{
		if (_instance)
			delete _instance;
	}

	/**  */
	static bool QueueMeshInstance(Eigen::Matrix4f a_Transform, AssetType::Mesh* a_Mesh, AssetType::Shader* a_Shader, AssetType::Material* a_Material, bool a_HasTransparency)
	{
		bool result = false;
		if (!a_HasTransparency)
		{
			result = getInstance()->queueGeometryBatchInstance(a_Mesh, a_Shader, a_Material, a_Transform);
		}

		return result;
	}

	static bool RenderFrame()
	{
		bool result = getInstance()->renderFrame();
		return result;
	}
};
}
