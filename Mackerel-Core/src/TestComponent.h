#pragma once

#include "Component.h"
#include "TransformComponent.h"

namespace MCK::EntitySystem
{
	class TestComponent : public Component
	{
	private:
		TransformComponent* transform;
		float t = 0;

	public:
		void OnCreate();
		void OnUpdate();
		void OnDestroy();
		bool Deserialise(json data);

		TypeInfoRef GetType();
	};
}