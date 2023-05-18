#include "TestComponent.h"

#include "Entity.h"

#include <typeinfo>
#include <iostream>
#include <math.h>
#include "TimeManager.h"

namespace MCK::EntitySystem
{

	/**
	 * Invoked when the entity holding the component is created.
	 *
	 */
	void TestComponent::OnCreate() 
	{
		transform = entity->GetComponent<TransformComponent>();
		transform->Position().z() = 3;
	}

	/**
	* Invoked when the component is updated each frame.
	*
	*/
	void TestComponent::OnUpdate() 
	{
		double delta = MCK::TimeManager::getFrameTime();
		float speed = 1.f * static_cast<float>(delta);

		std::cout << "I am being updated. Pos " << transform->Position().x();
		std::cout << "Input: " << input.x() << ", " << input.y() << std::endl;

		transform->Position().x() += speed * input.x();
		transform->Position().z() += speed * input.y();
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
