#pragma once
#include "CameraComponent.h"

namespace MCK::EntitySystem {
class PerspectiveCamera : public CameraComponent
{
public:
	PerspectiveCamera();
	PerspectiveCamera(float a_AspectRatio);
	PerspectiveCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane);
	PerspectiveCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane, Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection);

private:
	float m_FOVAngle;

public:
	float& FOVAngle() { return m_FOVAngle; }
	const float& FOVAngle() const { return m_FOVAngle; }

	Eigen::Matrix4f GetProjectionMatrix() const override;

	virtual TypeInfoRef GetType() override;
	bool Deserialise(json data) override;
	static void Reset(void* componentLoc);
};
}