#include "CollisionComponent.h"
#include "Scene.h"
#include "Entity.h"
#include "PhysicsHelpers.h"

#include <typeinfo>
#include <iostream>

namespace MCK::Physics
{

    /**
    * Returns the type id of this component
    *
    * \return The type id
    */
    TypeInfoRef CollisionComponent::GetType()
    {
        return typeid(RigidbodyComponent);
    }

	void CollisionComponent::UpdateColliderTransform()
	{
		btTransform transformation{};

		btVector3 pos(
			static_cast<btScalar>(transform->Position().x()),
			static_cast<btScalar>(transform->Position().y()),
			static_cast<btScalar>(transform->Position().z())
		);

		btQuaternion rot(
			static_cast<btScalar>(transform->Rotation().x()),
			static_cast<btScalar>(transform->Rotation().y()),
			static_cast<btScalar>(transform->Rotation().z()),
			static_cast<btScalar>(transform->Rotation().w())
		);

		transformation.setOrigin(pos);
		transformation.setRotation(rot);

		collider->setWorldTransform(transformation);
	}

	/**
	 * Sets the collision shape of the collision component.
	 *
	 * \param shapeInfo The details of the shape
	 */
	void CollisionComponent::SetCollisionShape(CreateCollisionShapeInfo shapeInfo)
	{
		if (collisionShape != nullptr)
		{
			delete collisionShape;
		}

		PhysicsHelpers::InitialiseCollider(shapeInfo, collisionShape);

		if (collider != nullptr)
		{
			collider->setCollisionShape(collisionShape);
		}
	}

	/**
	 * Invoked when the entity holding the component is created.
	 * The transform component of the entity rigidbody is part of is retrieved
	 * so it can be set by rigidbody's properties.
	 *
	 */
	void CollisionComponent::OnCreate()
	{
		transform = entity->GetComponent<MCK::EntitySystem::TransformComponent>();

		// Collision shape
		if(collisionShape == nullptr)
			collisionShape = new btBoxShape(btVector3({ 7,7,7 }));

		collider = new btCollisionObject();
		collider->setCollisionShape(collisionShape);
		collider->setUserPointer(static_cast<void*>(this));
		collider->setCollisionFlags(collider->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

		if(isTrigger)
			collider->setCollisionFlags(collider->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

		UpdateColliderTransform();

		// Add rigidbody
		entity->scene->physicsWorld.AddCollider(entity->id, this);
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void CollisionComponent::OnUpdate()
	{
		//std::cout << "Pos y: " << transform->Position().y() << std::endl;
		UpdateColliderTransform();
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void CollisionComponent::OnDestroy()
	{
		entity->scene->physicsWorld.RemoveCollider(entity->id);
		delete collisionShape;
	}

	bool CollisionComponent::Deserialise(json data)
	{
		return true;
	}
}