#include "PhysicsWorld.h"
#include "CollisionData.h"
#include "RigidbodyComponent.h"

#define DEBUG_WIN

#ifdef DEBUG_WIN

#endif // DEBUG_WIN

namespace MCK::Physics
{
	/**
	 * Bullet Collision Callback
	 */
	bool OnCollision(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);

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

		gContactAddedCallback = OnCollision;

#ifdef DEBUG_WIN


#endif // DEBUG_WIN

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

	bool OnCollision(btManifoldPoint& cp, 
		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, 
		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
	{
		bool rigidbodyStatus[2];
		rigidbodyStatus[0] = PhysicsHelpers::IsRigidbody(colObj0Wrap->getCollisionObject());
		rigidbodyStatus[1] = PhysicsHelpers::IsRigidbody(colObj1Wrap->getCollisionObject());

		if (rigidbodyStatus[0] || rigidbodyStatus[1])
		{
			const btCollisionObject* btCollisionObjects[2];
			btCollisionObjects[0] = colObj0Wrap->getCollisionObject();
			btCollisionObjects[1] = colObj1Wrap->getCollisionObject();

			EntitySystem::Entity* entities[2];
			entities[0] = PhysicsHelpers::GetEntity(colObj0Wrap->getCollisionObject());
			entities[1] = PhysicsHelpers::GetEntity(colObj1Wrap->getCollisionObject());

			for (int i = 0; i < 2; ++i)
			{
				int other = 1 - i;

				CollisionData colData{};
				colData.collidedEntity = entities[other];

				EntitySystem::ComponentBase* dynamicsComponent = PhysicsHelpers::GetDynamicsComponent(btCollisionObjects[i]);
				
				if (rigidbodyStatus[i])
				{
					RigidbodyComponent* comp = static_cast<RigidbodyComponent*>(dynamicsComponent);
					comp->onCollisionHandler.Invoke(colData);
				}
				else
				{
					CollisionComponent* comp = static_cast<CollisionComponent*>(dynamicsComponent);
					comp->onCollisionHandler.Invoke(colData);
				}
			}
		}
		return true;
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
		dynamicsWorld->stepSimulation(static_cast<btScalar>(delta), 0);

		for (const auto& body : sceneBodies)
		{
			RigidbodyComponent* rb = body.second;

			rb->ApplyToTransformComponent();
		}
	}

	void PhysicsWorld::TeardownWorld()
	{
		delete dynamicsWorld;
		delete collisionConfiguration;
		delete dispatcher;
		delete overlappingPairCache;
		delete solver;
	}
}
