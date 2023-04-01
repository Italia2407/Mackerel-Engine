#pragma once

#include "Eigen/Eigen.h"
#include "Component.h"

namespace MCK::EntitySystem
{
class TransformComponent : public Component
{
private:
	Eigen::Vector3f m_Position;
	Eigen::Quaternion<float> m_Rotation;
	Eigen::Vector3f m_Scale;

	Eigen::Vector2f m_YZPlaneShear;
	Eigen::Vector2f m_XZPlaneShear;
	Eigen::Vector2f m_XYPlaneShear;

public:
	Eigen::Matrix4f GetTransformationMatrix();

	Eigen::Vector3f& Position() { return m_Position; }
	Eigen::Quaternion<float>& Rotation() { return m_Rotation; }
	Eigen::Vector3f& Scale() { return m_Scale; }

	Eigen::Vector2f& XAxisShear() { return m_YZPlaneShear; }
	Eigen::Vector2f& YAxisShear() { return m_XZPlaneShear; }
	Eigen::Vector2f& ZAxisShear() { return m_XYPlaneShear; }

public:
	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;
	bool Deserialise(json data) override;

	TypeInfoRef GetType() override;
};
}