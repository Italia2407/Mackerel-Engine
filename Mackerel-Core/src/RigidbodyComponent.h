#pragma once
#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "Eigen/Core.h"
#include "Component.h"

namespace MCK::Physics
{
	// MISSING EVERYTHING - IMPLEMENT ME
	class RigidbodyComponent : public MCK::EntitySystem::Component
	{
	private:
		btRigidBody* rigidbody;

	public:
		void AddForce();
		void AddTorque();
		void SetVelocity();
		void SetPosition();
		Eigen::Vector3f GetVelocity();
		Eigen::Vector3f GetPosition();

		void OnCreate();
		void OnUpdate();
		void OnDestroy();

		TypeInfoRef GetType();
	};
}

