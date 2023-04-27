#pragma once
#include "LoggingSystem.h"
#include "Component.h"
#include "Scene.h"
#include "Sound.h"
 
namespace MCK::EntitySystem
{
	class AudioEmitter : public Component
	{
	private:
		unsigned int emitterSeed = 0;
		unsigned int emitterID = 0;
		unsigned int currentChannelID = 0;

		const char* soundFileName = "";
		bool soundLoop = false;
		bool sound3D = true;

		TransformComponent* transform;
		MCK::Audio::AudioEngine* audioEngine;
		MCK::Audio::Sound loadedSound;

	public:
		void LoadSound();
		void UnloadSound();
		void Play();
		void PlayOneShot();

		void Pause();
		void Resume();
		void Stop();
		bool IsPaused();
		bool IsPlaying();

		void SetSoundFileName(const char* filename);
		void SetSoundLoop(bool loop);
		void SetSound3D(bool s3D);


	public:
		void OnCreate() override;
		void OnUpdate() override;
		void OnDestroy() override;
		bool Deserialise(json data) override;

		TypeInfoRef GetType() override;
	};
}