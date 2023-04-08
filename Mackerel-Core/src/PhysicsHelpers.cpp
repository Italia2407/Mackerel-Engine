#include "PhysicsHelpers.h"

namespace MCK::Physics::PhysicsHelpers
{
	RigidbodyComponent* GetRigidbodyComponent(btRigidBody* btBody)
	{
		return static_cast<RigidbodyComponent*>(btBody->getUserPointer());
	}

	MCK::EntitySystem::Component* GetDynamicsComponent(btCollisionObject* dynamicsObject)
	{
		return static_cast<MCK::EntitySystem::Component*>(dynamicsObject->getUserPointer());
	}

	MCK::EntitySystem::Entity* GetEntity(btCollisionObject* dynamicsObject)
	{
		MCK::EntitySystem::Component* correspondingComponent = GetDynamicsComponent(dynamicsObject);
		return correspondingComponent->entity;
	}

	bool IsRigidbody(btCollisionObject* dynamicsObject)
	{
		return dynamicsObject->getInternalType() == btCollisionObject::CollisionObjectTypes::CO_RIGID_BODY;
	}
}

