#include "../bullet3-master/src/btBulletDynamicsCommon.h"
#include "CollisionComponent.h"
#include "RigidbodyComponent.h"
#include "CreateCollisionShapeInfo.h"
#include "Entity.h"
#include "Component.h"

namespace MCK::Physics::PhysicsHelpers
{
	/**
	 * Returns the corresponding dynamics component. Either a RigidbodyComponent or a CollisionComponent
	 */
	MCK::EntitySystem::ComponentBase* GetDynamicsComponent(const btCollisionObject* dynamicsObject);

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
	MCK::EntitySystem::Entity* GetEntity(const btCollisionObject* dynamicsObject);

	/**
	 * Whether a dynamics object is a Rigidbody.
	 * 
	 * \param dynamicsObject: the bullet physics object
	 */
	bool IsRigidbody(const btCollisionObject* dynamicsObject);

	/**
	* Creates a btBoxShape entity and returns a pointer to it as a btCollisionShape.
	* 
	* \param halExtents: a btVector3 of half the x. y and z lengths of the box.
	* \return a pointer to the btCollisionShape.
	*/
	btCollisionShape* CreateBoxShape(btVector3 halfExtents);

	/**
	* Creates a btSphereShape entity and returns a pointer to it as a btCollisionShape.
	*
	* \param radius: the radius of the sphere.
	* \return a pointer to the btCollisionShape.
	*/
	btCollisionShape* CreateSphereShape(float radius);

	/**
	* Creates a btCapsuleShape entity and returns a pointer to it as a btCollisionShape.
	*
	* \param width: the width of the capsule.
	* \param height: the height of the capsule.
	* \return a pointer to the btCollisionShape.
	*/
	btCollisionShape* CreateCapsuleShape(float width, float height);

	/**
	* Creates a collider using the data in the first parameter and other helper functions.
	* Assigns the pointer returned from those functions to the second parameter.
	* 
	* \param shapeInfo: struct containing the type of the collision shape and its dimensions.
	* \param collisionShape: a pointer to be used for the new collisionShape.
	*/
	void InitialiseCollider(CreateCollisionShapeInfo shapeInfo, btCollisionShape*& collisionShape);
}
