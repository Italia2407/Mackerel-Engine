#pragma once
#include "Component.h"
#include "Scene.h"

namespace MCK::EntitySystem
{
	class AudioListener : public Component
	{
	private:
		TransformComponent* transform;
		MCK::Audio::AudioEngine* audioEngine;

	public:
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		TypeInfoRef GetType() override;

		bool Deserialise(json data) override;
	};
}
