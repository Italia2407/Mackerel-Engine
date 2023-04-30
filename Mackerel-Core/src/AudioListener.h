#pragma once
#include "Component.h"
#include "Scene.h"

namespace MCK::EntitySystem
{
	class AudioListener : public Component<AudioListener>
	{
	private:
		TransformComponent* transform;
		MCK::Audio::AudioEngine* audioEngine;

	public:
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;

		bool Deserialise(json data) override;
	};
}
