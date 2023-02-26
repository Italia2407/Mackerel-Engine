#pragma once

#include "Component.h"

namespace MCK::EntitySystem
{
	class TestComponent : public Component
	{
	public:
		void OnCreate();
		void OnUpdate();
		void OnDestroy();
		bool Deserialise(json data);

		TypeInfoRef GetType();
	};
}