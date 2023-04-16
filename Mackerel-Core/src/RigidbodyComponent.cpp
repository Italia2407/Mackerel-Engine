#include "RigidbodyComponent.h"
#include "Scene.h"
#include "Entity.h"

#include <typeinfo>
#include <iostream>

namespace MCK::Physics
{
	/**
	* Apply a force, given as a vector, to the rigid body through the centre of mass.
	* No torque is calculated as a result.
	* 
	*/
	void RigidbodyComponent::AddCentralForce(const Eigen::Vector3f force)
	{
		rigidbody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
	}

	/**
	* Apply torque, given as a vector, to the rigid body.
	*
	*/
	void RigidbodyComponent::AddTorque(const Eigen::Vector3f torque)
	{
		rigidbody->applyTorque(btVector3(torque.x(), torque.y(), torque.z()));
	}

	/**
	* Apply a force, given as a vector, to the rigid body through a point relative to the centre of mass.
	* Torque is calculated automatically.
	* 
	*/
	void RigidbodyComponent::AddForce(const Eigen::Vector3f force, const Eigen::Vector3f rel_pos)
	{
		rigidbody->applyCentralForce(btVector3(force.x(), force.y(), force.z()));
		//rigidbody->applyForce(btVector3(force.x(), force.y(), force.z()), btVector3(rel_pos.x(), rel_pos.y(), rel_pos.z()));
	}

	/**
	* Set linear velocity for the rigidbody.
	*
	*/
	void RigidbodyComponent::SetLinearVelocity(const Eigen::Vector3f linearVel)
	{
		rigidbody->setLinearVelocity(btVector3(linearVel.x(), linearVel.y(), linearVel.z()));
	}

	/**
	* Set angular velocity for the rigidbody.
	*
	*/
	void RigidbodyComponent::SetAngularVelocity(const Eigen::Vector3f angularVel)
	{
		rigidbody->setLinearVelocity(btVector3(angularVel.x(), angularVel.y(), angularVel.z()));
	}

	/**
	* Set the position of the rigidbody.
	* There is no one built-in function to do this so can't guarantee it works without testing.
	*
	*/
	void RigidbodyComponent::SetPosition(const Eigen::Vector3f position)
	{
		btTransform transform = rigidbody->getCenterOfMassTransform();
		transform.setOrigin(btVector3(position.x(), position.y(), position.z()));
		rigidbody->setCenterOfMassTransform(transform);
	}

	/**
	* Get linear velocity for the rigidbody.
	*
	* \return The linear velocity as an Eigen 3-vector.
	*/
	Eigen::Vector3f RigidbodyComponent::GetLinearVelocity()
	{
		btVector3 linearVel = rigidbody->getLinearVelocity();
		return Eigen::Vector3f(linearVel.x(), linearVel.y(), linearVel.z());
	}

	/**
	* Get angular velocity for the rigidbody.
	*
	* \return The angular velocity as an Eigen 3-vector.
	*/
	Eigen::Vector3f RigidbodyComponent::GetAngularVelocity()
	{
		btVector3 angularVel = rigidbody->getAngularVelocity();
		return Eigen::Vector3f(angularVel.x(), angularVel.y(), angularVel.z());
	}

	/**
	* Returns the position of the rigidbody.
	* Since the rigodbody is not a single point, the centre of mass is returned.
	*
	* \return The position of the rigidbody's centre of mass as an Eigen 3-vector.
	*/
	Eigen::Vector3f RigidbodyComponent::GetPosition()
	{
		btVector3 pos = rigidbody->getCenterOfMassPosition();
		return Eigen::Vector3f(pos.x(), pos.y(), pos.z());
	}

	/**
	* Applies the transformation of the given TransformComponent to the rigidbody.
	*
	*/
	void RigidbodyComponent::ApplyToTransformComponent()
	{
		btTransform rbTransform = rigidbody->getCenterOfMassTransform();
		btVector3 pos = rigidbody->getCenterOfMassPosition();
		btQuaternion rot = rbTransform.getRotation();
		
		transform->Position() = Eigen::Vector3f(pos.x(), pos.y(), pos.z());
		transform->Rotation() = Eigen::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
	}

    /**
    * Returns the type id of this component
    *
    * \return The name as a C string.
    */
    TypeInfoRef RigidbodyComponent::GetType()
    {
        return typeid(RigidbodyComponent);
    }

	/**
	 * Invoked when the entity holding the component is created.
	 * The transform component of the entity rigidbody is part of is retrieved 
	 * so it can be set by rigidbody's properties.
	 *
	 */
	void RigidbodyComponent::OnCreate()
	{
		transform = entity->GetComponent<MCK::EntitySystem::TransformComponent>();
		
		// Calculate the initial transformation
		btTransform initialTransformation{};

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

		initialTransformation.setOrigin(pos);
		initialTransformation.setRotation(rot);

		// Collision shape
		collisionShape = new btBoxShape(btVector3({ 1,1,1 }));

		// Construct the rigidbody
		btRigidBody::btRigidBodyConstructionInfo info
		(
			1, // mass
			nullptr, // Motion state
			collisionShape // Collision
		);
		info.m_startWorldTransform = initialTransformation;

		rigidbody = new btRigidBody(info);
		rigidbody->setUserPointer(static_cast<void*>(this));
		//rigidbody->setLinearFactor()

		// Add rigidbody
		entity->scene->physicsWorld.AddRigidbody(entity->id, this);
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void RigidbodyComponent::OnUpdate()
	{
		std::cout << "Pos y: " << transform->Position().y() << std::endl;
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void RigidbodyComponent::OnDestroy()
	{
		entity->scene->physicsWorld.RemoveRigidbody(entity->id);
		delete collisionShape;
		delete rigidbody;
	}

	bool RigidbodyComponent::Deserialise(json data)
	{
		return true;
	}
}

