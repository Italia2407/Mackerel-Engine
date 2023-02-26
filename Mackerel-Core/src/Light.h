#pragma once

#include <glad/glad.h>
#include <Eigen/Eigen.h>

namespace MCK::Rendering {
struct Light
{
	Light(Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

	Eigen::Vector4f diffuseColour;
	Eigen::Vector4f specularColour;
	Eigen::Vector4f ambientColour;

	static GLuint getBaseLightShaderOffsetSize();
};

struct PointLight : Light
{
	PointLight(Eigen::Vector3f position, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

	Eigen::Vector3f position;

	static GLuint getPointLightShaderOffsetSize();
};
struct DirectionLight : Light
{
	DirectionLight(Eigen::Vector3f direction, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

	Eigen::Vector3f direction;

	static GLuint getDirectionLightShaderOffsetSize();
};
struct SpotLight : Light
{
	SpotLight(Eigen::Vector3f position, Eigen::Vector3f direction, float beamAngle, Eigen::Vector4f diffuseColour, Eigen::Vector4f specularColour, Eigen::Vector4f ambientColour);

	Eigen::Vector3f position;
	Eigen::Vector3f direction;

	float beamAngle;

	static GLuint getSpotLightShaderOffsetSize();
};
}
