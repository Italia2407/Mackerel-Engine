#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "Eigen/Core.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CreateCollisionShapeInfo.h"

namespace MCK::Physics
{
	class RigidbodyComponent : public MCK::EntitySystem::Component
	{
	private:
		
		MCK::EntitySystem::TransformComponent* transform;

		float angularFactor = 1;
	public:
		btRigidBody* rigidbody = nullptr;
		btCollisionShape* collisionShape = nullptr;

		void AddCentralForce(const Eigen::Vector3f force);
		void AddTorque(const Eigen::Vector3f torque);
		void AddForce(const Eigen::Vector3f force, const Eigen::Vector3f rel_pos);
		void SetLinearVelocity(const Eigen::Vector3f linearVel);
		void SetAngularVelocity(const Eigen::Vector3f angularVel);
		void SetPosition(const Eigen::Vector3f position);
		Eigen::Vector3f GetLinearVelocity();
		Eigen::Vector3f GetAngularVelocity();
		Eigen::Vector3f GetPosition();
		void ApplyToTransformComponent();
		void SetCollisionShape(CreateCollisionShapeInfo shapeInfo);
		void EnableRotation();
		void DisableRotation();

		void OnCreate();
		void OnUpdate();
		void OnDestroy();

		bool Deserialise(json data) override;

		TypeInfoRef GetType();
	};
}

