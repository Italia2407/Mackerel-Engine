#include "Light.h"

#include "Texture.h"
#include "UniformBuffer.h"

namespace MCK::Rendering {
Light::Light(Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	_diffuseColour(diffuseColour), _specularColour(specularColour), _ambientColour(ambientColour), _shadowMap(nullptr), _parametersBuffer(nullptr)
{
	// Initialise & Create Shadow Map Texture
	_shadowMap = new AssetType::Texture();
	_shadowMap->GenerateFloatTexture(1024, 1024);

	// Initialse & Create Parameters Buffer
	_parametersBuffer = new UniformBuffer();
	{
	// Add Light ModelViewProjection
	_parametersBuffer->AddMat4BufferUniform("modelViewProjection", Eigen::Matrix4f::Zero());

	// Add Position & Direction
	_parametersBuffer->AddVec3BufferUniform("position", Eigen::Vector3f::Zero());
	_parametersBuffer->AddVec3BufferUniform("direction", Eigen::Vector3f::Zero());

	// Add Beam Angle
	_parametersBuffer->AddFloatBufferUniform("beamAngle", 0.0f);

	// Add Light Colours
	_parametersBuffer->AddVec4BufferUniform("diffuseColour", Eigen::Vector4f::Zero());
	_parametersBuffer->AddVec4BufferUniform("specularColour", Eigen::Vector4f::Zero());
	_parametersBuffer->AddVec4BufferUniform("ambientColour", Eigen::Vector4f::Zero());
	}
	_parametersBuffer->CreateUniformBufferObject();
}

Eigen::Matrix4f Light::getMVPMatrix()
{
	return Eigen::Matrix4f::Zero();
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

PointLight::PointLight(Eigen::Vector3f position, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _position(position) {}
DirectionLight::DirectionLight(Eigen::Vector3f direction, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), _direction(direction) {}
SpotLight::SpotLight(Eigen::Vector3f position, Eigen::Vector3f direction, float beamAngle, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
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
