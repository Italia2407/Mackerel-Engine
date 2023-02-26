#pragma once

#include <glad/glad.h>

#include <string>
#include <map>
#include <vector>

// Forward Declarations
namespace MCK {
class UniformBuffer;
}
namespace MCK::AssetType {
class Texture;
class Shader;
}
namespace MCK::Rendering {
struct PointLight;
struct DirectionLight;
struct SpotLight;

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

	GLuint _GBuffer;
	GLuint _frameBuffer;

	UniformBuffer* _lightUniformBuffer;

	std::vector<RenderBatch*> _geometryBatches;

	AssetType::Texture* _GBufferTextures[32];
	std::vector<AssetType::Shader*> _lightingShaders;
	
	std::vector<PointLight*> _pointLights;
	std::vector<DirectionLight*> _directionLights;
	std::vector<SpotLight*> _spotLights;

private:
	bool initialiseRenderer(GLuint screenWidth, GLuint screenHeight);

	bool createGBuffer(GLuint screenWidth, GLuint screenHeight);
	bool createLightingUniformBuffer();

	void renderGBuffer();
	void renderFrameBuffer();

	//bool updatePointLight(GLuint pointLightID, PointLight* pointLight);
	bool attachPointLight(PointLight* pointLight);
	bool attachDirectionLight(DirectionLight* directionLight);
	bool attachSpotLight(SpotLight* spotLight);

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
