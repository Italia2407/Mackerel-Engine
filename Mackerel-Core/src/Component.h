#pragma once
#include "ComponentBase.h"

namespace MCK::EntitySystem
{
	template<typename T>
	class Component : public ComponentBase
	{
	public:
		bool IsType() { return typeid(T) == GetType(); }

		static void Reset(void* componentLoc)
		{
			T* componentPtr = static_cast<T*>(componentLoc);
			*componentPtr = T();
		}

		TypeInfoRef GetType() override
		{
			return typeid(T);
		}
	};
}

