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
		if (RayCallback.hasHit())
		{
			cast.distance = RayCallback.m_hitPointWorld - start;
			cast.point = RayCallback.m_hitPointWorld;
			cast.normal = RayCallback.m_hitNormalWorld;
			cast.hit = true;

			cast.hitEntity = PhysicsHelpers::GetEntity(RayCallback.m_collisionObject);
		}

		return cast;
	}

	RaycastHits PhysicsWorld::RaycastAll(btVector3 start, btVector3 end)
	{
		btCollisionWorld::AllHitsRayResultCallback RayCallback(start, end);

		dynamicsWorld->rayTest(start, end, RayCallback);

		RaycastHits casts{};
		casts.num_hits = 0;

		for (int i = 0; i < RayCallback.m_collisionObjects.size(); i++)
		{
			RaycastHit cast{};
			cast.distance = RayCallback.m_hitPointWorld[i] - start;
			cast.hit = true;
			cast.hitEntity = PhysicsHelpers::GetEntity(RayCallback.m_collisionObjects[i]);
			cast.normal = RayCallback.m_hitNormalWorld[i];
			cast.point = RayCallback.m_hitPointWorld[i];

			casts.num_hits++;
			casts.hits.emplace_back(cast);
		}

		return casts;
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
