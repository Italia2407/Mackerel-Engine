#pragma once
#include "CameraComponent.h"

#include <glad/glad.h>

namespace MCK::EntitySystem {
class OrthographicCamera : public CameraComponent
{
public:
	OrthographicCamera(float a_AspectRatio);
	OrthographicCamera(float a_AspectRatio, float a_Right, float a_Left, float a_Top, float a_Bottom, float a_Far, float a_Near);
	OrthographicCamera(float a_AspectRatio, float a_Right, float a_Left, float a_Top, float a_Bottom, float a_Far, float a_Near,
		Eigen::Vector3f a_Position, Eigen::Vector3f a_FrontDirection, Eigen::Vector3f a_UpDirection);

private:
	float m_Right;
	float m_Left;
	float m_Top;
	float m_Bottom;

public:
	float& Right() { return m_Right; }
	const float& Right() const { return m_Right; }
	float& Left() { return m_Left; }
	const float& Left() const { return m_Left; }
	float& Top() { return m_Top; }
	const float& Top() const { return m_Top; }
	float& Bottom() { return m_Bottom; }
	const float& Bottom() const { return m_Bottom; }

	Eigen::Matrix4f GetProjectionMatrix() const override;

	bool Deserialise(json data) override;

	TypeInfoRef GetType() override;
};
}
