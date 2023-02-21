#pragma once

#include <Eigen/Eigen.h>

struct Light
{
	Light(Eigen::Vector4f colour);

	Eigen::Vector4f diffuseColour;
	Eigen::Vector4f specularColour;
	Eigen::Vector4f ambientColour;
};

struct PointLight : Light
{
	PointLight(Eigen::Vector3f position, Eigen::Vector4f colour);

	Eigen::Vector3f position;
};

struct DirectionLight : Light
{
	DirectionLight(Eigen::Vector3f direction, Eigen::Vector4f colour);

	Eigen::Vector3f direction;
};

struct SpotLight : Light
{
	SpotLight(Eigen::Vector3f position, Eigen::Vector3f direction, Eigen::Vector4f colour);

	Eigen::Vector3f position;
	Eigen::Vector3f direction;
};

