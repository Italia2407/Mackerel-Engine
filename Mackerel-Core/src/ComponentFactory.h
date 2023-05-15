#pragma once

#include "RegisteredComponentData.h"
#include <string>
#include <vector>
#include "BlockAllocator.h"

// Forward Declarations
namespace MCK::EntitySystem {
class ComponentBase;
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
		ComponentBase* CreateComponent(std::string key);

		/**
		 * Deallocates a component.
		 * 
		 * \param component The component to deallocate
		 */
		void FreeComponent(ComponentBase* component);
	};

	class ComponentManager
	{
	private:
		ComponentManager();
		~ComponentManager();

		// we can have one input manager, as a treat
		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(const ComponentManager&) = delete;

		// singleton bookkeeping
		static ComponentManager* instance;
		static ComponentManager* Instance()
		{
			if (!instance)
				instance = new ComponentManager;
			return instance;
		}

		std::vector<RegisteredComponentData> componentRegistry;

		ComponentBase* privAllocateComponent(size_t componentSize, void (*resetFunction)(void*));
		void privDeallocateComponent(ComponentBase* componentPtr);
		void privRegisterComponent(std::string jsonKey, size_t componentSize, void (*resetFunction)(void*));
		ComponentBase* privAllocateComponent(std::string jsonKey);

		MemoryManagement::BlockAllocator componentAllocator;

	public:

		static inline ComponentBase* AllocateComponent(size_t componentSize, void (*resetFunction)(void*))
		{
			return Instance()->privAllocateComponent(componentSize, resetFunction);
		}

		static inline void DeallocateComponent(ComponentBase* componentPtr)
		{
			Instance()->privDeallocateComponent(componentPtr);
		}

		static inline void RegisterComponent(std::string jsonKey, size_t componentSize, void (*resetFunction)(void*))
		{
			Instance()->privRegisterComponent(jsonKey, componentSize, resetFunction);
		}

		static inline ComponentBase* AllocateComponent(std::string jsonKey)
		{
			return Instance()->privAllocateComponent(jsonKey);
		}

		//template<typename T> static void Reset(void* componentLocation)
		//{
		//	T* compPtr = static_cast<T*>(componentLocation);
		//	*compPtr = T();
		//}
	};
}
