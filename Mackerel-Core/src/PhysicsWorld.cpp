#include "PhysicsWorld.h"

namespace MCK::Physics
{
	void PhysicsWorld::AddRigidbody(entityId id, RigidbodyComponent* rigidbody)
	{
		sceneBodies.emplace(id, rigidbody);
	}

	void PhysicsWorld::RemoveRigidbody(entityId id)
	{
		sceneBodies.erase(id);
	}
}
