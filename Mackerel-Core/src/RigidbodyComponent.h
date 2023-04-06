#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "Eigen/Core.h"
#include "Component.h"
#include "TransformComponent.h"

namespace MCK::Physics
{
	class RigidbodyComponent : public MCK::EntitySystem::Component
	{
	private:
		btRigidBody* rigidbody;

	public:
		void AddCentralForce(const Eigen::Vector3f force);
		void AddTorque(const Eigen::Vector3f torque);
		void AddForce(const Eigen::Vector3f force, const Eigen::Vector3f rel_pos);
		void SetLinearVelocity(const Eigen::Vector3f linearVel);
		void SetAngularVelocity(const Eigen::Vector3f angularVel);
		void SetPosition(const Eigen::Vector3f position);
		Eigen::Vector3f GetLinearVelocity();
		Eigen::Vector3f GetAngularVelocity();
		Eigen::Vector3f GetPosition();
		void applyToTransformComponent(MCK::EntitySystem::TransformComponent& transformComponent);

		void OnCreate();
		void OnUpdate();
		void OnDestroy();

		TypeInfoRef GetType();
	};
}

