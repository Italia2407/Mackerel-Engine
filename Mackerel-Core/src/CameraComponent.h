#pragma once
#include "Component.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace MCK::EntitySystem {
class CameraComponent : public Component
{
public:
	CameraComponent(float a_AspectRatio);
	CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane);
	CameraComponent(float a_AspectRatio, glm::vec3 a_Position, glm::vec3 a_FrontDirection, glm::vec3 a_UpDirection);
	CameraComponent(float a_AspectRatio, float a_FarPlane, float a_NearPlane, glm::vec3 a_Position, glm::vec3 a_FrontDirection, glm::vec3 a_UpDirection);

protected:
	glm::vec3 m_Position;
	glm::vec3 m_FrontDirection;
	glm::vec3 m_UpDirection;

	float m_AspectRatio;

	float m_FarPlane;
	float m_NearPlane;

public:
	glm::vec3& Position() { return m_Position; }
	const glm::vec3& Position() const { return m_Position; }
	glm::vec3& FrontDirection() { return m_FrontDirection; }
	const glm::vec3& FrontDirection() const { return m_FrontDirection; }
	glm::vec3& UpDirection() { return m_UpDirection; }
	const glm::vec3& UpDirection() const { return m_UpDirection; }

	float& AspectRatio() { return m_AspectRatio; }
	const float& AspectRatio() const { return m_AspectRatio; }

	float& FarPlane() { return m_FarPlane; }
	const float& FarPlane() const { return m_FarPlane; }
	float& NearPlane() { return m_NearPlane; }
	const float& NearPlane() const { return m_NearPlane; }

	glm::mat4 GetCameraViewMatrix() const;
	virtual glm::mat4 GetProjectionMatrix() const = 0;
};
}
