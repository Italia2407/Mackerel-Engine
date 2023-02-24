#pragma once

#include <string>

// Forward Declarations
namespace MCK::EntitySystem {
class Component;
}

namespace MCK::EntitySystem
{
	/**
	 * A factory for creating components and managing them in memory.
	 */
	class ComponentFactory
	{
	public:
		/**
		 * Create a component.
		 * 
		 * \param key: The type of component to create
		 * \return A pointer to the component, nullptr if no key found
		 */
		Component* CreateComponent(std::string key);

		/**
		 * Deallocates a component.
		 * 
		 * \param component The component to deallocate
		 */
		void FreeComponent(Component* component);
	};
}
