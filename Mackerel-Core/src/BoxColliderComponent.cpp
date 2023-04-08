#include "BoxColliderComponent.h"

#include "Entity.h"

namespace MCK::EntitySystem
{
	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef BoxColliderComponent::GetType()
	{
		return typeid(BoxColliderComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void BoxColliderComponent::OnCreate()
	{
		std::cout << "I was created" << std::endl;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void BoxColliderComponent::OnUpdate()
	{
		std::cout << "I am being updated" << std::endl;
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void BoxColliderComponent::OnDestroy()
	{
		std::cout << "I have been destroyed" << std::endl;
	}

	bool BoxColliderComponent::Deserialise(json data)
	{
		// Get Entity's Box Collider Component Data.
		data = data["data"];

		// Get Box Collider Component's Scale
		Scale().x() = data["scaleX"];
		Scale().y() = data["scaleY"];
		Scale().z() = data["scaleZ"];

		return true;
	}
}