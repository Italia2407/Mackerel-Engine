#include "Light.h"

namespace MCK::Rendering {
Light::Light(Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	diffuseColour(diffuseColour), specularColour(specularColour), ambientColour(ambientColour) {}

GLuint Light::getBaseLightShaderOffsetSize()
{
	GLuint diffuseColourSize = GLuint(sizeof(float) * 4);
	GLuint specularColourSize = GLuint(sizeof(float) * 4);
	GLuint ambientColourSize = GLuint(sizeof(float) * 4);

	return diffuseColourSize + specularColourSize + ambientColourSize;
}

PointLight::PointLight(Eigen::Vector3f position, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), position(position) {}

GLuint PointLight::getPointLightShaderOffsetSize()
{
	GLuint positionSize = GLuint(sizeof(float) * 4);

	return getBaseLightShaderOffsetSize() + positionSize;
}

DirectionLight::DirectionLight(Eigen::Vector3f direction, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), direction(direction) {}

GLuint DirectionLight::getDirectionLightShaderOffsetSize()
{
	GLuint directionSize = GLuint(sizeof(float) * 4);

	return getBaseLightShaderOffsetSize() + directionSize;
}

SpotLight::SpotLight(Eigen::Vector3f position, Eigen::Vector3f direction, float beamAngle, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour) :
	Light(diffuseColour, specularColour, ambientColour), position(position), direction(direction), beamAngle(beamAngle) {}

GLuint SpotLight::getSpotLightShaderOffsetSize()
{
	GLuint positionSize = GLuint(sizeof(float) * 4);
	GLuint directionSize = GLuint(sizeof(float) * 4);

	return getBaseLightShaderOffsetSize() + positionSize + directionSize;
}
}
