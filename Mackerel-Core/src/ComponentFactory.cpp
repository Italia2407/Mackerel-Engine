#include "ComponentFactory.h"

#include "Component.h"
#include "TestComponent.h"

namespace MCK::EntitySystem
{
	// --- Old system ---
	const std::string TESTCOMPONENT_KEY = "TestComponent";
	const std::string TRANSFORM_COMP_KEY = "TransformComponent";

	/**
	 * Create a component.
	 *
	 * \param key: The type of component to create
	 * \return A pointer to the component, nullptr if no key found
	 */
	ComponentBase* ComponentFactory::CreateComponent(std::string key)
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
	void ComponentFactory::FreeComponent(ComponentBase* component)
	{
		delete component;
	}

	// --- New system ---

	ComponentManager* ComponentManager::instance = nullptr;

	ComponentManager::ComponentManager()
	{
	}

	ComponentManager::~ComponentManager()
	{
	}

	ComponentBase* ComponentManager::privAllocateComponent(size_t componentSize, void (*resetFunction)(void*))
	{
		void* componentPtr = malloc(componentSize);
		(*resetFunction)(componentPtr);

		return static_cast<ComponentBase*>(componentPtr);
	}

	void ComponentManager::privDeallocateComponent(ComponentBase* componentPtr)
	{
		free(componentPtr);
	}

	void ComponentManager::privRegisterComponent(std::string jsonKey, size_t componentSize, void (*resetFunction)(void*))
	{
		RegisteredComponentData compData{};
		compData.componentSize = componentSize;
		compData.jsonKey = jsonKey;
		compData.resetFunction = resetFunction;

		componentRegistry.push_back(compData);
	}

	ComponentBase* ComponentManager::privAllocateComponent(std::string jsonKey)
	{
		// Find the component registry
		for (int i = 0; i < componentRegistry.size(); ++i)
		{
			if (jsonKey.length() == componentRegistry[i].jsonKey.length() &&
				jsonKey.compare(componentRegistry[i].jsonKey) == 0)
			{
				// Component found
				return AllocateComponent(componentRegistry[i].componentSize,
					componentRegistry[i].resetFunction);
			}
		}

		
		return nullptr;
	}

}