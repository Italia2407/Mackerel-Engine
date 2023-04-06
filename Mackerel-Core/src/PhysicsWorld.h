#pragma once

#include "RigidbodyComponent.h"
#include "Component.h"
#include "Entity.h"
#include "../bullet3-master/src/btBulletDynamicsCommon.h"

namespace MCK::Physics
{
	class PhysicsWorld
	{
	private:
		std::map<entityId, RigidbodyComponent*> sceneBodies;

		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
	public:

		void InitialiseWorld();

		void AddRigidbody(entityId id, RigidbodyComponent* rigidbody);

		void RemoveRigidbody(entityId id);

		void TeardownWorld();
	};
}

