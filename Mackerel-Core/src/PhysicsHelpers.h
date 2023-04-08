#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "CollisionComponent.h"
#include "RigidbodyComponent.h"
#include "CreateCollisionShapeInfo.h"
#include "Entity.h"
#include "Component.h"

namespace MCK::Physics::PhysicsHelpers
{
	/**
	 * Gets the corresponding Rigidbody component from a btRigidbody.
	 * 
	 * \param btBody: the rigidbody
	 * \return A pointer to the Rigidbody Component
	 */
	RigidbodyComponent* GetRigidbodyComponent(btRigidBody* btBody);


	/**
	 * Gets the entity that this dynamics object is attached to
	 * 
	 * \param dynamicsObject: the bullet physics object
	 * \return A pointer to the entity
	 */
	MCK::EntitySystem::Entity* GetEntity(btCollisionObject* dynamicsObject);

	/**
	 * Whether a dynamics object is a Rigidbody.
	 * 
	 * \param dynamicsObject: the bullet physics object
	 */
	bool IsRigidbody(btCollisionObject* dynamicsObject);

	// TODO Implement me. Commented so it compiles
	// btCollisionShape* CreateBoxShape(btVector3 halfExtents);

	// btCollisionShape* CreateSphereShape(float radius);

	// btCollisionShape* CreateCapsuleShape(float width, float height);


	// TODO Implement me - Create the shape and assign it to collisionShape
	// void InitialiseCollider(CreateCollisionShapeInfo shapeInfo, btCollisionShape*& collisionShape);
}
