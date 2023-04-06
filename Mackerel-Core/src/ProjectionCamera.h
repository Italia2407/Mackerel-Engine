#pragma once
#include "CameraComponent.h"

namespace MCK::EntitySystem {
class ProjectionCamera : public CameraComponent
{
public:
	ProjectionCamera(float a_AspectRatio);
	ProjectionCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane);
	ProjectionCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane, Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection);

private:
	float m_FOVAngle;

public:
	float& FOVAngle() { return m_FOVAngle; }
	const float& FOVAngle() const { return m_FOVAngle; }

	Eigen::Matrix4f GetProjectionMatrix() const override;

	bool Deserialise(json data) override;

	TypeInfoRef GetType() override;
};
}