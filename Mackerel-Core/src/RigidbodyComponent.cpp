#include "RigidbodyComponent.h"
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
		rigidbody->applyForce(btVector3(force.x(), force.y(), force.z()), btVector3(rel_pos.x(), rel_pos.y(), rel_pos.z()));
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
	void RigidbodyComponent::applyToTransformComponent(MCK::EntitySystem::TransformComponent& transformComponent)
	{
		btTransform rbTransform = rigidbody->getCenterOfMassTransform();
		btVector3 pos = rigidbody->getCenterOfMassPosition();
		btQuaternion rot = rbTransform.getRotation();
		
		transformComponent.Position() = Eigen::Vector3f(pos.x(), pos.y(), pos.z());
		transformComponent.Rotation() = Eigen::Quaternion(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
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
		MCK::EntitySystem::TransformComponent* transform = entity->GetComponent<MCK::EntitySystem::TransformComponent>();
		std::cout << "I was created" << std::endl;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void RigidbodyComponent::OnUpdate()
	{
		std::cout << "I am being updated" << std::endl;
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void RigidbodyComponent::OnDestroy()
	{
		std::cout << "I have been destroyed" << std::endl;
	}
}

