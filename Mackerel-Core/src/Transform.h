#pragma once

#include "Eigen/Eigen.h"

#include <iostream>

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
	Transform() :
		Position(Eigen::Vector3f::Zero()), Rotation(Eigen::Quaternion<float>::Identity()), Scale(Eigen::Vector3f::Ones()),
		YZPlaneShear(Eigen::Vector2f::Zero()), XZPlaneShear(Eigen::Vector2f::Zero()), XYPlaneShear(Eigen::Vector2f::Zero()) {}

	/**
	 * Computes the Transformation Matrix.
	 * 
	 * \return The Trasformation Matrix
	 */
	Eigen::Matrix4f Matrix() const
	{
		// Create Translation Matrix
		Eigen::Matrix4f translationMatrix = Eigen::Affine3f(Eigen::Translation3f(Position)).matrix();

		// Create Rotation Matrix
		Eigen::Matrix3f rotationMatrix3D = Rotation.toRotationMatrix();
		Eigen::Matrix4f rotationMatrix = Eigen::Matrix4f::Identity();
		rotationMatrix.block(0, 0, 3, 3) = rotationMatrix3D;

		// Create Scale Matrix
		Eigen::Matrix4f scaleMatrix = Eigen::Matrix4f::Identity();

		// Create Shear Matrix
		Eigen::Matrix4f shearMatrix = Eigen::Matrix4f::Identity();

		// Set X-Axis Shear
		shearMatrix(0, 1) = YZPlaneShear.x();
		shearMatrix(0, 2) = YZPlaneShear.y();
		// Set Y-Axis Shear
		shearMatrix(1, 0) = XZPlaneShear.x();
		shearMatrix(1, 2) = XZPlaneShear.y();
		// Set Z-Axis Shear
		shearMatrix(2, 0) = XYPlaneShear.x();
		shearMatrix(2, 1) = XYPlaneShear.y();


		Eigen::Matrix4f transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix * shearMatrix;
		return transformationMatrix;
	}
};
}