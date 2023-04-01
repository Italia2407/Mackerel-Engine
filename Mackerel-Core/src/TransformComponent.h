#pragma once

#include "Eigen/Eigen.h"
#include "Component.h"

#include "Transform.h"

namespace MCK::EntitySystem
{
class TransformComponent : public Component
{
private:
	Transform m_Transform;

public:
	Eigen::Vector3f& Position() { return m_Transform.Position; }
	Eigen::Quaternion<float>& Rotation() { return m_Transform.Rotation; }
	Eigen::Vector3f& Scale() { return m_Transform.Scale; }

	Eigen::Vector2f& YZPlaneShear() { return m_Transform.YZPlaneShear; }
	Eigen::Vector2f& XZPlaneShear() { return m_Transform.XZPlaneShear; }
	Eigen::Vector2f& XYPlaneShear() { return m_Transform.XYPlaneShear; }

public:
	Eigen::Matrix4f GetTransformationMatrix() const;

	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;
	bool Deserialise(json data) override;

	TypeInfoRef GetType() override;
};
}