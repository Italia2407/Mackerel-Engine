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
		dynamicsWorld->addRigidBody(rigidbody->rigidbody);
	}

	void PhysicsWorld::RemoveRigidbody(entityId id)
	{
		RigidbodyComponent* rigidbody = sceneBodies[id];
		dynamicsWorld->removeRigidBody(rigidbody->rigidbody);

		sceneBodies.erase(id);
	}

	void PhysicsWorld::AddCollider(entityId id, CollisionComponent* collision)
	{
		sceneColliders.emplace(id, collision);
		dynamicsWorld->addCollisionObject(collision->collider);
	}

	void PhysicsWorld::RemoveCollider(entityId id)
	{
		CollisionComponent* collision = sceneColliders[id];
		dynamicsWorld->removeCollisionObject(collision->collider);

		sceneColliders.erase(id);
	}

	RaycastHit PhysicsWorld::Raycast(btVector3 start, btVector3 end)
	{
		RaycastHit cast{};
		cast.hit = false;

		btCollisionWorld::ClosestRayResultCallback RayCallback(start, end);

		dynamicsWorld->rayTest(start, end, RayCallback);
		
		// set the raycast hit struct information
		cast.distance = end - start;
		cast.point = RayCallback.m_hitPointWorld;
		cast.normal = RayCallback.m_hitNormalWorld;
		if (RayCallback.hasHit())
			cast.hit = true;

		cast.hitEntity = PhysicsHelpers::GetEntity(RayCallback.m_collisionObject);

		return cast;
	}

	void PhysicsWorld::ApplySimulation(double delta)
	{
		// Step
		dynamicsWorld->applyGravity();
		dynamicsWorld->stepSimulation(static_cast<btScalar>(delta));

		for (const auto& body : sceneBodies)
		{
			RigidbodyComponent* rb = body.second;

			rb->ApplyToTransformComponent();
		}
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
