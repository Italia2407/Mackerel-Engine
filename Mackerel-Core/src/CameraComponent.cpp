#include "CameraComponent.h"

#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"

#include <math.h>

namespace MCK::EntitySystem {
CameraComponent::CameraComponent(float a_AspectRatio) :
	m_Position(Eigen::Vector3f::Zero()), m_FrontDirection(Eigen::Vector3f(0.0f, 0.0f, 1.0f)), m_UpDirection(Eigen::Vector3f(0.0f, 1.0f, 0.0f)),
	m_AspectRatio(a_AspectRatio), m_FarPlane(10.0f), m_NearPlane(0.0001f) {}
CameraComponent::CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane) :
	m_Position(Eigen::Vector3f::Zero()), m_FrontDirection(Eigen::Vector3f(0.0f, 0.0f, 1.0f)), m_UpDirection(Eigen::Vector3f(0.0f, 1.0f, 0.0f)),
	m_AspectRatio(a_AspectRatio), m_FarPlane(a_FarPlane), m_NearPlane(a_NearPlane) {}
CameraComponent::CameraComponent(float a_AspectRatio, Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection) :
	m_Position(a_Position), m_FrontDirection(a_FrontDirection), m_UpDirection(a_UpDirection),
	m_AspectRatio(a_AspectRatio), m_FarPlane(10.0f), m_NearPlane(0.0001f) {}
CameraComponent::CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane, Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection) :
	m_Position(a_Position), m_FrontDirection(a_FrontDirection), m_UpDirection(a_UpDirection),
	m_AspectRatio(a_AspectRatio), m_FarPlane(a_FarPlane), m_NearPlane(a_NearPlane) {}

/**
 * Calculates the View Matrix for the Camera.
 *
 * \return The Camera's View Matrix
 */
Eigen::Matrix4f CameraComponent::GetCameraViewMatrix() const
{
	// Calculate the Camera Space Axes Directions
	Eigen::Vector3f cameraZAxis = Eigen::Vector3f(m_FrontDirection.x(), m_FrontDirection.y(), -m_FrontDirection.z()).normalized();
	Eigen::Vector3f cameraXAxis = cameraZAxis.cross(Eigen::Vector3f(m_UpDirection.x(), m_UpDirection.y(), -m_UpDirection.z())).normalized();
	Eigen::Vector3f cameraYAxis = cameraXAxis.cross(cameraZAxis).normalized();

	// Create the Camera View Matrix
	Eigen::Matrix4f cameraViewMatrix = Eigen::Matrix4f::Identity(); {
		cameraViewMatrix.coeffRef(0, 0) = cameraXAxis.x();
		cameraViewMatrix.coeffRef(0, 1) = cameraXAxis.y();
		cameraViewMatrix.coeffRef(0, 2) = cameraXAxis.z();
		cameraViewMatrix.coeffRef(0, 3) = cameraXAxis.dot(Eigen::Vector3f(-m_Position.x(), -m_Position.y(), m_Position.z()));

		cameraViewMatrix.coeffRef(1, 0) = cameraYAxis.x();
		cameraViewMatrix.coeffRef(1, 1) = cameraYAxis.y();
		cameraViewMatrix.coeffRef(1, 2) = cameraYAxis.z();
		cameraViewMatrix.coeffRef(1, 3) = cameraYAxis.dot(Eigen::Vector3f(-m_Position.x(), -m_Position.y(), m_Position.z()));

		cameraViewMatrix.coeffRef(2, 0) = cameraZAxis.x();
		cameraViewMatrix.coeffRef(2, 1) = cameraZAxis.y();
		cameraViewMatrix.coeffRef(2, 2) = cameraZAxis.z();
		cameraViewMatrix.coeffRef(2, 3) = cameraZAxis.dot(Eigen::Vector3f(-m_Position.x(), -m_Position.y(), m_Position.z()));
	}

	return cameraViewMatrix;
}
}

namespace MCK::EntitySystem {
OrthographicCamera::OrthographicCamera(float a_AspectRatio) :
	CameraComponent(a_AspectRatio),
	m_Right(1.0f), m_Left(-1.0f), m_Top(1.0f), m_Bottom(-1.0f) {}
OrthographicCamera::OrthographicCamera(float a_AspectRatio, float a_Right, float a_Left, float a_Top, float a_Bottom, float a_Far, float a_Near) :
	CameraComponent(a_AspectRatio, a_Far, a_Near),
	m_Right(a_Right), m_Left(a_Left), m_Top(a_Top), m_Bottom(a_Bottom) {}
OrthographicCamera::OrthographicCamera(
		float a_AspectRatio, float a_Right, float a_Left, float a_Top, float a_Bottom, float a_Far, float a_Near,
		Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection) :
	CameraComponent(a_AspectRatio, a_Far, a_Near, a_Position, a_FrontDirection, a_UpDirection),
	m_Right(a_Right), m_Left(a_Left), m_Top(a_Top), m_Bottom(a_Bottom) {}

Eigen::Matrix4f OrthographicCamera::GetProjectionMatrix() const
{
	Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Identity(); {
		if (m_AspectRatio > 1.0f)
			projectionMatrix.coeffRef(0, 0) = 2.0f / ((m_Right - m_Left) * m_AspectRatio);
		else
			projectionMatrix.coeffRef(0, 0) = 2.0f / (m_Right - m_Left);
		projectionMatrix.coeffRef(0, 3) = -(m_Right + m_Left) / (m_Right - m_Left);

		if (m_AspectRatio < 1.0f)
			projectionMatrix.coeffRef(1, 1) = (2.0f * m_AspectRatio) / (m_Top - m_Bottom);
		else
			projectionMatrix.coeffRef(1, 1) = 2.0f / (m_Top - m_Bottom);
		projectionMatrix.coeffRef(1, 3) = -(m_Top + m_Bottom) / (m_Top - m_Bottom);

		projectionMatrix.coeffRef(2, 2) = 1.0f / (m_FarPlane - m_NearPlane);
		projectionMatrix.coeffRef(2, 3) = -(m_NearPlane) / (m_FarPlane - m_NearPlane);
	}

	return projectionMatrix;
}

bool OrthographicCamera::Deserialise(json data)
{
	return false;
}
TypeInfoRef OrthographicCamera::GetType()
{
	return typeid(OrthographicCamera);
}
}

namespace MCK::EntitySystem {
PerspectiveCamera::PerspectiveCamera(float a_AspectRatio) :
	CameraComponent(a_AspectRatio), m_FOVAngle(45.0f) {}
PerspectiveCamera::PerspectiveCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane) :
	CameraComponent(a_AspectRatio, a_FarPlane, a_NearPlane), m_FOVAngle(a_FOVAngle) {}
PerspectiveCamera::PerspectiveCamera(
	float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane,
	Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection) :
	CameraComponent(a_AspectRatio, a_FarPlane, a_NearPlane, a_Position, a_FrontDirection, a_UpDirection), m_FOVAngle(a_FOVAngle) {}

Eigen::Matrix4f PerspectiveCamera::GetProjectionMatrix() const
{
	float PI = std::acos(0.0f) * 2.0f;
	float FOVAngleTan = std::tan((m_FOVAngle * (PI / 180.0f)) / 2.0f);

	Eigen::Matrix4f projectionMatrix = Eigen::Matrix4f::Zero(); {
		if (m_AspectRatio > 1.0f)
			projectionMatrix.coeffRef(0, 0) = 1.0f / (FOVAngleTan * m_AspectRatio);
		else
			projectionMatrix.coeffRef(0, 0) = 1.0f / FOVAngleTan;

		if (m_AspectRatio < 1.0f)
			projectionMatrix.coeffRef(1, 1) = m_AspectRatio / FOVAngleTan;
		else
			projectionMatrix.coeffRef(1, 1) = 1.0f / FOVAngleTan;

		projectionMatrix.coeffRef(2, 2) = m_FarPlane / (m_FarPlane - m_NearPlane);
		projectionMatrix.coeffRef(2, 3) = -(m_FarPlane * m_NearPlane) / (m_FarPlane - m_NearPlane);

		projectionMatrix.coeffRef(3, 2) = 1.0f;
	}

	return projectionMatrix;
}

bool PerspectiveCamera::Deserialise(json data)
{
	return false;
}
TypeInfoRef PerspectiveCamera::GetType()
{
	return typeid(PerspectiveCamera);
}
}