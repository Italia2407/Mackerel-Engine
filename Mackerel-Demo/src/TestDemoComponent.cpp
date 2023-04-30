#include "TestDemoComponent.h"

#include "Entity.h"

#include <typeinfo>
#include <iostream>
#include <math.h>
#include "TimeManager.h"

namespace MCK::EntitySystem::Demo
{
	/**
	* Returns the type id of this component
	*
	* \return The name as a C string.
	*/
	TypeInfoRef TestDemoComponent::GetType()
	{
		return typeid(TestDemoComponent);
	}

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void TestDemoComponent::OnCreate()
	{
		std::cout << "Test demo component created" << std::endl;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void TestDemoComponent::OnUpdate()
	{

	}

	/**
	* Invoked before the entity holding this component is destroyed.
	*
	*/
	void TestDemoComponent::OnDestroy()
	{
		std::cout << "I have been destroyed" << std::endl;
	}

	bool TestDemoComponent::Deserialise(json data)
	{
		std::cout << "DESERIALISE " << data.dump() << std::endl;
		return true;
	}

	void TestDemoComponent::Reset(void* component)
	{
		std::cout << "Resetting" << std::endl;
	}
}
