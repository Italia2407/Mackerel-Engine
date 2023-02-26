#include "ComponentFactory.h"

#include "Component.h"
#include "TestComponent.h"

namespace MCK::EntitySystem
{
	const std::string TESTCOMPONENT_KEY = "TestComponent";

	/**
	 * Create a component.
	 *
	 * \param key: The type of component to create
	 * \return A pointer to the component, nullptr if no key found
	 */
	Component* ComponentFactory::CreateComponent(std::string key)
	{
		if (key == TESTCOMPONENT_KEY)
		{
			return new TestComponent();
		}

		return nullptr;
	}

	/**
	 * Deallocates a component.
	 *
	 * \param component The component to deallocate
	 */
	void ComponentFactory::FreeComponent(Component* component)
	{
		delete component;
	}
}