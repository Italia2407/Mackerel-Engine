#include "PhysicsWorld.h"

namespace MCK::Physics
{
	void PhysicsWorld::InitialiseWorld()
	{
		collisionConfiguration = new
			btDefaultCollisionConfiguration();
		
		dispatcher = new btCollisionDispatcher(collisionConfiguration);

		overlappingPairCache = new btDbvtBroadphase();

		solver = new btSequentialImpulseConstraintSolver;

		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
			overlappingPairCache, solver, collisionConfiguration);

		dynamicsWorld -> setGravity(btVector3(0, -10, 0));
	}

	void PhysicsWorld::AddRigidbody(entityId id, RigidbodyComponent* rigidbody)
	{
		sceneBodies.emplace(id, rigidbody);
	}

	void PhysicsWorld::RemoveRigidbody(entityId id)
	{
		sceneBodies.erase(id);
	}

	void PhysicsWorld::TeardownWorld()
	{
		delete collisionConfiguration;
		delete dispatcher;
		delete overlappingPairCache;
		delete solver;
		delete dynamicsWorld;
	}
}
