#pragma once
#include "Component.h"

#include "Eigen/Eigen.h"

namespace MCK::EntitySystem {
class CameraComponent : public Component
{
public:
	CameraComponent(float a_AspectRatio);
	CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane);
	CameraComponent(float a_AspectRatio, Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection);
	CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane, Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection);

protected:
	Eigen::Vector3f m_Position;
	Eigen::Vector3f m_FrontDirection;
	Eigen::Vector3f m_UpDirection;

	float m_AspectRatio;

	float m_FarPlane;
	float m_NearPlane;

public:
	Eigen::Vector3f& Position() { return m_Position; }
	const Eigen::Vector3f& Position() const { return m_Position; }
	Eigen::Vector3f& FrontDirection() { return m_FrontDirection; }
	const Eigen::Vector3f& FrontDirection() const { return m_FrontDirection; }
	Eigen::Vector3f& UpDirection() { return m_UpDirection; }
	const Eigen::Vector3f& UpDirection() const { return m_UpDirection; }

	float& AspectRatio() { return m_AspectRatio; }
	const float& AspectRatio() const { return m_AspectRatio; }

	float& FarPlane() { return m_FarPlane; }
	const float& FarPlane() const { return m_FarPlane; }
	float& NearPlane() { return m_NearPlane; }
	const float& NearPlane() const { return m_NearPlane; }

	Eigen::Matrix4f GetCameraViewMatrix() const;
	virtual Eigen::Matrix4f GetProjectionMatrix() const = 0;
};
}
