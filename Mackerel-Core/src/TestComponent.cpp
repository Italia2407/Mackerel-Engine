#include "TestComponent.h"

#include "Entity.h"

#include <typeinfo>
#include <iostream>
#include <math.h>

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
		transform = entity->GetComponent<TransformComponent>();
		transform->Position().z() = 10;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void TestComponent::OnUpdate() 
	{
		std::cout << "I am being updated. Pos " << transform->Position() << std::endl;
		t += 0.01f;
		transform->Position().y() = sinf(t);
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
		std::cout << "DESERIALISE " << data.dump() << std::endl;
		return true;
	}
}
