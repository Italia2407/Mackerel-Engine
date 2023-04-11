#pragma once

#include "RigidbodyComponent.h"
#include "CollisionComponent.h"
#include "RaycastHitInfo.h"
#include "PhysicsHelpers.h"
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

		RaycastHit Raycast(btVector3 start, btVector3 end);

		/**
		 * Applies the simulation transforms to all rigidbodies
		 * 
		 */
		void ApplySimulation(double delta);

		void TeardownWorld();
	};
}

