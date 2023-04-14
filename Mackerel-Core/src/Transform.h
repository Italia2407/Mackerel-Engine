#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>

namespace MCK {
struct Transform
{
public:
	glm::vec3 Position;
	glm::quat Rotation;
	glm::vec3 Scale;

	glm::vec2 YZPlaneShear;
	glm::vec2 XZPlaneShear;
	glm::vec2 XYPlaneShear;

public:
	Transform() :
		Position(glm::vec3(0.0f)), Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)), Scale(glm::vec3(1.0f)),
		YZPlaneShear(glm::vec2(0.0f)), XZPlaneShear(glm::vec2(0.0f)), XYPlaneShear(glm::vec2(0.0f)) {}

	/**
	 * Computes the Transformation Matrix.
	 * 
	 * \return The Trasformation Matrix
	 */
	glm::mat4 Matrix() const
	{
		// Create Translation Matrix
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);

		// Create Rotation Matrix
		glm::mat4 rotationMatrix = glm::toMat4(Rotation);

		// Create Scale Matrix
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);

		// Create Shear Matrix
		glm::mat4 shearMatrix = glm::mat4(1.0f);

		// Set X-Axis Shear
		shearMatrix[0][1] = YZPlaneShear.x;
		shearMatrix[0][2] = YZPlaneShear.y;
		// Set Y-Axis Shear
		shearMatrix[1][0] = XZPlaneShear.x;
		shearMatrix[1][2] = XZPlaneShear.y;
		// Set Z-Axis Shear
		shearMatrix[2][0] = XYPlaneShear.x;
		shearMatrix[2][1] = XYPlaneShear.y;


		glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * scaleMatrix * shearMatrix;
		return transformationMatrix;
	}
};
}