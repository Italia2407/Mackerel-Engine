#pragma once

#include "Eigen/Eigen.h"
#include "Component.h"

#include "Transform.h"

namespace MCK::EntitySystem
{
class TransformComponent : public Component<TransformComponent>
{
private:
	Transform m_Transform;

public:
	inline Eigen::Vector3f& Position() { return m_Transform.Position; }
	inline Eigen::Quaternion<float>& Rotation() { return m_Transform.Rotation; }
	inline Eigen::Vector3f& Scale() { return m_Transform.Scale; }

	inline Eigen::Vector2f& YZPlaneShear() { return m_Transform.YZPlaneShear; }
	inline Eigen::Vector2f& XZPlaneShear() { return m_Transform.XZPlaneShear; }
	inline Eigen::Vector2f& XYPlaneShear() { return m_Transform.XYPlaneShear; }

public:
	Eigen::Matrix4f GetTransformationMatrix() const;

	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;
	bool Deserialise(json data) override;
};
}