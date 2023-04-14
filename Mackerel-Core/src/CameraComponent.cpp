#include "CameraComponent.h"

#include "OrthographicCamera.h"
#include "ProjectionCamera.h"

#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MCK::EntitySystem {
CameraComponent::CameraComponent(float a_AspectRatio) :
	m_Position(glm::vec3(0.0f)), m_FrontDirection(glm::vec3(0.0f, 0.0f, -1.0f)), m_UpDirection(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_AspectRatio(a_AspectRatio), m_FarPlane(1.0f), m_NearPlane(0.0f) {}
CameraComponent::CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane) :
	m_Position(glm::vec3(0.0f)), m_FrontDirection(glm::vec3(0.0f, 0.0f, -1.0f)), m_UpDirection(glm::vec3(0.0f, 1.0f, 0.0f)),
	m_AspectRatio(a_AspectRatio), m_FarPlane(a_FarPlane), m_NearPlane(a_NearPlane) {}
CameraComponent::CameraComponent(float a_AspectRatio, glm::vec3 a_Position, glm::vec3 a_FrontDirection, glm::vec3 a_UpDirection) :
	m_Position(a_Position), m_FrontDirection(a_FrontDirection), m_UpDirection(a_UpDirection),
	m_AspectRatio(a_AspectRatio), m_FarPlane(1.0f), m_NearPlane(0.0f) {}
CameraComponent::CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane, glm::vec3 a_Position, glm::vec3 a_FrontDirection, glm::vec3 a_UpDirection) :
	m_Position(a_Position), m_FrontDirection(a_FrontDirection), m_UpDirection(a_UpDirection),
	m_AspectRatio(a_AspectRatio), m_FarPlane(a_FarPlane), m_NearPlane(a_NearPlane) {}

/**
 * Calculates the View Matrix for the Camera.
 * 
 * \return The Camera's View Matrix
 */
glm::mat4 CameraComponent::GetCameraViewMatrix() const
{
	glm::mat4 cameraViewMatrix = glm::lookAt(m_Position, m_Position + m_FrontDirection, m_UpDirection);
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
		glm::vec3 a_Position, glm::vec3 a_FrontDirection, glm::vec3 a_UpDirection) :
	CameraComponent(a_AspectRatio, a_Far, a_Near, a_Position, a_FrontDirection, a_UpDirection),
	m_Right(a_Right), m_Left(a_Left), m_Top(a_Top), m_Bottom(a_Bottom) {}

glm::mat4 OrthographicCamera::GetProjectionMatrix() const
{
	glm::mat4 projectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane);
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
ProjectionCamera::ProjectionCamera(float a_AspectRatio) :
	CameraComponent(a_AspectRatio), m_FOVAngle(45.0f) {}
ProjectionCamera::ProjectionCamera(float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane) :
	CameraComponent(a_AspectRatio, a_FarPlane, a_NearPlane), m_FOVAngle(a_FOVAngle) {}
ProjectionCamera::ProjectionCamera(
	float a_AspectRatio, float a_FOVAngle, float a_FarPlane, float a_NearPlane,
	glm::vec3 a_Position, glm::vec3 a_FrontDirection, glm::vec3 a_UpDirection) :
	CameraComponent(a_AspectRatio, a_FarPlane, a_NearPlane, a_Position, a_FrontDirection, a_UpDirection), m_FOVAngle(a_FOVAngle) {}

glm::mat4 ProjectionCamera::GetProjectionMatrix() const
{
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(m_FOVAngle), m_AspectRatio, m_NearPlane, m_FarPlane);
	return projectionMatrix;
}

bool ProjectionCamera::Deserialise(json data)
{
	return false;
}
TypeInfoRef ProjectionCamera::GetType()
{
	return typeid(ProjectionCamera);
}
}