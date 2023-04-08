#pragma once

#include "RigidbodyComponent.h"
#include "CollisionComponent.h"
#include "Component.h"
#include "Entity.h"
#include "../bullet3-master/src/btBulletDynamicsCommon.h"

namespace MCK::Physics
{
	class PhysicsWorld
	{
	private:
		std::map<entityId, RigidbodyComponent*> sceneBodies;
		std::map<entityId, CollisionComponent*> sceneColliders;

		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
	public:

		void InitialiseWorld();

		void AddRigidbody(entityId id, RigidbodyComponent* rigidbody);

		void RemoveRigidbody(entityId id);

		void AddCollider(entityId id, CollisionComponent* rigidbody);

		void RemoveCollider(entityId id);

		/**
		 * Applies the simulation transforms to all rigidbodies
		 * 
		 */
		void ApplySimulation(double delta);

		void TeardownWorld();
	};
}

