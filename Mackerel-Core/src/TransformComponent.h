#pragma once
#include "Component.h"
#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace MCK::EntitySystem
{
class TransformComponent : public Component
{
private:
	Transform m_Transform;

public:
	glm::vec3& Position() { return m_Transform.Position; }
	glm::quat& Rotation() { return m_Transform.Rotation; }
	glm::vec3& Scale() { return m_Transform.Scale; }

	glm::vec2& YZPlaneShear() { return m_Transform.YZPlaneShear; }
	glm::vec2& XZPlaneShear() { return m_Transform.XZPlaneShear; }
	glm::vec2& XYPlaneShear() { return m_Transform.XYPlaneShear; }

public:
	glm::mat4 GetTransformationMatrix() const;

	void OnCreate() override;
	void OnUpdate() override;
	void OnDestroy() override;
	bool Deserialise(json data) override;

	TypeInfoRef GetType() override;
};
}