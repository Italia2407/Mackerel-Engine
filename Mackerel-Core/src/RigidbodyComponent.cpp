#include "RigidbodyComponent.h"

#include <typeinfo>
#include <iostream>

namespace MCK::Physics
{
	/**
	*
	*
	* 
	*/
	void RigidbodyComponent::AddForce()
	{

	}

	/**
	*
	*
	*
	*/
	void RigidbodyComponent::AddTorque()
	{

	}

	/**
	*
	*
	*
	*/
	void RigidbodyComponent::SetVelocity()
	{

	}

	/**
	*
	*
	*
	*/
	void RigidbodyComponent::SetPosition()
	{

	}

	/**
	*
	*
	*
	*/
	Eigen::Vector3f RigidbodyComponent::GetVelocity()
	{
		return Eigen::Vector3f(0.0f, 0.0f, 0.0f);
	}

	/**
	*
	*
	*
	*/
	Eigen::Vector3f RigidbodyComponent::GetPosition()
	{
		return Eigen::Vector3f();
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
	 *
	 */
	void RigidbodyComponent::OnCreate()
	{
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

