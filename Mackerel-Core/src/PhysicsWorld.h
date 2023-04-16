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

		/**
		 * Casts a ray from the start point to the end point,
		 *	and returns information about the hit entity and if one has been hit.
		 * 
		 * \param start point where the ray starts
		 * \param end point where the ray is being shot to
		 * \return RaycastHit structure, which contains a reference to the hit entity,
		 *	 as well as the hit point, distance and normal, and a bool to say if something was hit
		 */
		RaycastHit Raycast(btVector3 start, btVector3 end);

		/**
		 *	Casts a ray from the start point through the end point, 
		 *	returns pointers to RaycastHit structs of each entity that was hit.
		 * 
		 * \param start point where the ray starts
		 * \param end point where the ray is being shot to
		 * \return number of hits, and a pointer to a RaycastHit struct for each hit
		 */
		RaycastHits RaycastAll(btVector3 start, btVector3 end);

		/**
		 * Applies the simulation transforms to all rigidbodies
		 * 
		 */
		void ApplySimulation(double delta);

		void TeardownWorld();
	};
}

