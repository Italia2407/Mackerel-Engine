#include "Light.h"

#include "Texture.h"
#include "UniformBuffer.h"

namespace MCK::Rendering {
Light::Light(glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour) :
	_diffuseColour(diffuseColour), _specularColour(specularColour), _ambientColour(ambientColour), _shadowMap(nullptr), _parametersBuffer(nullptr)
{
	// Initialise & Create Shadow Map Texture
	_shadowMap = new AssetType::Texture();
	_shadowMap->GenerateFloatTexture(1024, 1024);

	// Initialse & Create Parameters Buffer
	_parametersBuffer = new UniformBuffer();
	{
	// Add Light ModelViewProjection
	_parametersBuffer->AddMat4BufferUniform("modelViewProjection", glm::mat4(0.0f));

	// Add Position & Direction
	_parametersBuffer->AddVec3BufferUniform("position", glm::vec3(0.0f));
	_parametersBuffer->AddVec3BufferUniform("direction", glm::vec3(0.0f));

	// Add Beam Angle
	_parametersBuffer->AddFloatBufferUniform("beamAngle", 0.0f);

	// Add Light Colours
	_parametersBuffer->AddVec4BufferUniform("diffuseColour", glm::vec4(0.0f));
	_parametersBuffer->AddVec4BufferUniform("specularColour", glm::vec4(0.0f));
	_parametersBuffer->AddVec4BufferUniform("ambientColour", glm::vec4(0.0f));
	}
	_parametersBuffer->CreateUniformBufferObject();
}

glm::mat4 Light::getMVPMatrix()
{
	return glm::mat4(0.0f);
}

bool Light::UseLight()
{
	updateLightingParameters();

	// Bind Parameters Buffer
	_parametersBuffer->BindUniformBufferObject(0);

	// Bind Shadow Map
	_shadowMap->BindTexture(31);

	return true;
}

PointLight::PointLight(glm::vec3 position, glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _position(position) {}
DirectionLight::DirectionLight(glm::vec3 direction, glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _direction(direction) {}
SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, float beamAngle, glm::vec4 diffuseColour, glm::vec4 specularColour, glm::vec4 ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _position(position), _direction(direction), _beamAngle(beamAngle) {}

bool PointLight::updateLightingParameters()
{
	// Set ModelViewProjection
	_parametersBuffer->SetMat4BufferUniform("modelViewProjection", getMVPMatrix());

	// Set Position
	_parametersBuffer->SetVec3BufferUniform("position", _position);

	// Set Colours
	_parametersBuffer->SetVec4BufferUniform("diffuseColour", _diffuseColour);
	_parametersBuffer->SetVec4BufferUniform("specularColour", _specularColour);
	_parametersBuffer->SetVec4BufferUniform("ambientColour", _ambientColour);

	return true;
}
bool DirectionLight::updateLightingParameters()
{
	// Set ModelViewProjection
	_parametersBuffer->SetMat4BufferUniform("modelViewProjection", getMVPMatrix());

	// Set Direction
	_parametersBuffer->SetVec3BufferUniform("direction", _direction);

	// Set Colours
	_parametersBuffer->SetVec4BufferUniform("diffuseColour", _diffuseColour);
	_parametersBuffer->SetVec4BufferUniform("specularColour", _specularColour);
	_parametersBuffer->SetVec4BufferUniform("ambientColour", _ambientColour);

	return true;
}
bool SpotLight::updateLightingParameters()
{
	// Set ModelViewProjection
	_parametersBuffer->SetMat4BufferUniform("modelViewProjection", getMVPMatrix());

	// Set Position & Direction
	_parametersBuffer->SetVec3BufferUniform("position", _position);
	_parametersBuffer->SetVec3BufferUniform("direction", _direction);

	// Set Beam Angle
	_parametersBuffer->SetFloatBufferUniform("beamAngle", _beamAngle);

	// Set Colours
	_parametersBuffer->SetVec4BufferUniform("diffuseColour", _diffuseColour);
	_parametersBuffer->SetVec4BufferUniform("specularColour", _specularColour);
	_parametersBuffer->SetVec4BufferUniform("ambientColour", _ambientColour);

	return true;
}
}
