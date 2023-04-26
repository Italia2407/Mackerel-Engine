#include "PhysicsHelpers.h"

namespace MCK::Physics::PhysicsHelpers
{
	RigidbodyComponent* GetRigidbodyComponent(btRigidBody* btBody)
	{
		return static_cast<RigidbodyComponent*>(btBody->getUserPointer());
	}

	MCK::EntitySystem::Component* GetDynamicsComponent(const btCollisionObject* dynamicsObject)
	{
		return static_cast<MCK::EntitySystem::Component*>(dynamicsObject->getUserPointer());
	}

	MCK::EntitySystem::Entity* GetEntity(const btCollisionObject* dynamicsObject)
	{
		MCK::EntitySystem::Component* correspondingComponent = GetDynamicsComponent(dynamicsObject);
		return correspondingComponent->entity;
	}

	bool IsRigidbody(const btCollisionObject* dynamicsObject)
	{
		return dynamicsObject->getInternalType() == btCollisionObject::CollisionObjectTypes::CO_RIGID_BODY;
	}

	btCollisionShape* CreateBoxShape(btVector3 halfExtents)
	{
		btBoxShape* box = new btBoxShape(halfExtents);
		return static_cast<btCollisionShape*>(box);
	}

	btCollisionShape* CreateSphereShape(float radius)
	{
		btSphereShape* sphere = new btSphereShape(radius);
		return static_cast<btCollisionShape*>(sphere);
	}

	btCollisionShape* CreateCapsuleShape(float width, float height)
	{
		btCapsuleShape* capsule = new btCapsuleShape(width, height);
		return static_cast<btCollisionShape*>(capsule);
	}

	void InitialiseCollider(CreateCollisionShapeInfo shapeInfo, btCollisionShape*& collisionShape)
	{
		switch (shapeInfo.colliderType)
		{
		case (Box):
			collisionShape = CreateBoxShape(btVector3(shapeInfo.width, shapeInfo.height, shapeInfo.depth));
			break;
		case (Sphere):
			collisionShape = CreateSphereShape(shapeInfo.radius);
			break;
		case (Capsule):
			collisionShape = CreateCapsuleShape(shapeInfo.width, shapeInfo.height);
			break;
		};
	}
}

