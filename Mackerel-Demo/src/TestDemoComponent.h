#pragma once

#include "Component.h"
#include "TransformComponent.h"

namespace MCK::EntitySystem::Demo
{
	class TestDemoComponent : public ComponentBase
	{
	private:
		float t = 0;

	public:

		void OnCreate();
		void OnUpdate();
		void OnDestroy();
		bool Deserialise(json data);

		TypeInfoRef GetType();

		static void Reset(void* component);
	};
}