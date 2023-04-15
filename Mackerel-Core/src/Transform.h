#pragma once

#include "Eigen/Eigen.h"

namespace MCK {
struct Transform
{
public:
	Eigen::Vector3f Position;
	Eigen::Quaternion<float> Rotation;
	Eigen::Vector3f Scale;

	Eigen::Vector2f YZPlaneShear;
	Eigen::Vector2f XZPlaneShear;
	Eigen::Vector2f XYPlaneShear;

public:
	Transform();

	Eigen::Matrix4f Matrix() const;
};
}