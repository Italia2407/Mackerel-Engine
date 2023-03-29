#include "TransformComponent.h"

#include "Entity.h"

#include <typeinfo>
#include <iostream>

namespace MCK::EntitySystem
{
	/**
	* Returns the transformation matrix of this entity
	* Takes the transformation of parent entities into account
	* TODO Implement me
	* 
	*/
	Eigen::Matrix4f TransformComponent::GetTransformationMatrix()
	{
		Eigen::Matrix4f mat;
		mat.Identity();

		return mat;
	}

	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef TransformComponent::GetType()
	{
		return typeid(TransformComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void TransformComponent::OnCreate()
	{
		std::cout << "I was created" << std::endl;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void TransformComponent::OnUpdate()
	{
		std::cout << "I am being updated" << std::endl;
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void TransformComponent::OnDestroy()
	{
		std::cout << "I have been destroyed" << std::endl;
	}

	bool TransformComponent::Deserialise(json data)
	{
		std::cout << "DESERIALISE " << data.dump() << std::endl;
		return true;
	}
}