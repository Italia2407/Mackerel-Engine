#include "InputComponent.h"

#include "Entity.h"

namespace MCK::EntitySystem
{
	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef InputComponent::GetType()
	{
		return typeid(InputComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void InputComponent::OnCreate()
	{
		std::cout << "I was created" << std::endl;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void InputComponent::OnUpdate()
	{
		std::cout << "I am being updated" << std::endl;
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void InputComponent::OnDestroy()
	{
		std::cout << "I have been destroyed" << std::endl;
	}

	bool InputComponent::Deserialise(json data)
	{
		// Get Entity's Box Collider Component Data.
		data = data["data"];

		// Get Box Collider Component's Scale
		Direction().x() = data["directionX"];
		Direction().y() = data["directionY"];

		return true;
	}
}