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

	FrameBuffer* _geometryBuffer;
	FrameBuffer* _lightingBuffer;

	AssetType::Texture* _depthBufferTexture;

	GLuint _GBuffer;
	GLuint _frameBuffer;

	//std::vector<RenderBatch*> _geometryBatches;
	std::map<std::pair<AssetType::Mesh*, AssetType::Shader*>, RenderBatch*> _geometryBatches;

	AssetType::Texture* _GBufferTextures[31];
	AssetType::Texture* _depthTexture;

	// Light & Shadow Rendering
	std::vector<AssetType::Shader*> _pointLightShaders;
	std::vector<AssetType::Shader*> _directionLightShaders;
	std::vector<AssetType::Shader*> _spotLightShaders;

	std::vector<PointLight*> _pointLights;
	std::vector<DirectionLight*> _directionLights;
	std::vector<SpotLight*> _spotLights;

private:
	bool initialiseRenderer(GLuint screenWidth, GLuint screenHeight);

	bool createGBuffer(GLuint screenWidth, GLuint screenHeight);
	bool createLightingBuffer(GLuint screenWidth, GLuint screenHeight);

	void renderGBuffer();
	void renderLightingBuffer();
	void renderShadowMap(Light* light);

	// Functions to tell the Renderer what needs to be Rendered

	bool queuePointLight(PointLight* pointLight);
	bool queueDirectionLight(DirectionLight* directionLight);
	bool queueSpotLight(SpotLight* spotLight);

	bool queueRenderBatchInstance(AssetType::Mesh* mesh, AssetType::Shader* shader, AssetType::Material* material,
		Eigen::Vector3f position, Eigen::Quaternion<float> rotation, Eigen::Vector3f scale);

	void renderFrame();

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
};
}
