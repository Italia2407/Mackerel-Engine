#pragma once
#include "ComponentBase.h"
#include "CreateCollisionShapeInfo.h"

namespace MCK::EntitySystem
{
	template<typename T>
	class Component : public ComponentBase
	{
	public:
		bool IsType() { return typeid(T) == GetType(); }

		static void Reset(void* componentLoc)
		{

			//T* componentPtr = static_cast<T*>(componentLoc);
			//componentPtr = new T();
			new (componentLoc) T();
		}

		TypeInfoRef GetType() override
		{
			return typeid(T);
		}
	};
}

