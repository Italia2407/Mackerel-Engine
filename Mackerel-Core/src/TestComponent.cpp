#include "TestComponent.h"
#include "Entity.h"
#include <typeinfo>
#include <iostream>

namespace MCK::EntitySystem
{
	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef TestComponent::GetType()
	{ 
		return typeid(TestComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void TestComponent::OnCreate() 
	{
		std::cout << "I was created" << std::endl;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void TestComponent::OnUpdate() 
	{
		std::cout << "I am being updated" << std::endl;
	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void TestComponent::OnDestroy()
	{
		std::cout << "I have been destroyed" << std::endl;
	}

	bool TestComponent::Deserialise(json data)
	{
		return true;
	}
}
