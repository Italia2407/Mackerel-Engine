#pragma once
#include "CameraComponent.h"

namespace MCK::EntitySystem {
class ProjectionCamera : public CameraComponent
{
public:
	ProjectionCamera(float a_AspectRatio);
	ProjectionCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane);
	ProjectionCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane,
		glm::vec3 a_Position, glm::vec3 a_FrontDirection, glm::vec3 a_UpDirection);

private:
	float m_FOVAngle;

public:
	float& FOVAngle() { return m_FOVAngle; }
	const float& FOVAngle() const { return m_FOVAngle; }

	glm::mat4 GetProjectionMatrix() const override;

	bool Deserialise(json data) override;

	TypeInfoRef GetType() override;
};
}