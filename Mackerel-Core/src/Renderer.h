#pragma once

#include <glad/glad.h>

#include <string>
#include <map>
#include <vector>

#include "RenderBatch.h"

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

	std::vector<RenderBatch*> _geometryBatches;

	std::map<std::string, AssetType::Texture*> _GBufferTextures;
	std::vector<AssetType::Material*> _lightingMaterials;

private:
	bool addGBufferTexture(std::string name);
	bool addLightingMaterial(AssetType::Shader* lightingShader);

	void bindGBuffer(GLuint screenWidth, GLuint screenHeight);

	void renderGBuffer();
	void renderFrameBuffer();
public:
	static void BindGBuffer(GLuint screenWidth, GLuint screenHeight)
	{
		getInstance()->bindGBuffer(screenWidth, screenHeight);
	}

	static void ClearRenderer()
	{
		if (_instance)
			delete _instance;
	}
};
}
