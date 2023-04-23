#include "AudioEmitter.h"

namespace MCK::EntitySystem
{

	/**
	 * Function to load a sound using the audio engine.
	 * Uses the settings stored in the audio emitter, to load it.
	 * Sound ID and Emitter ID are the same.
	 * For now there are no streamed sounds, so that is set to false.
	 * The function also stores the loaded sound in the emitter.
	 * 
	 */
	void AudioComponent::LoadSound()
	{
		// check if a sound has already been loaded, if yes, unload it
		if (loadedSound.sound != nullptr)
			UnloadSound();

		// load the sound by calling LoadSound in the audio engine
		loadedSound = audioEngine->LoadSound(soundFileName, emitterID, sound3D, soundLoop, false);		
	}

	/**
	 * Function to unload the sound previously loaded by the audio emitter.
	 * 
	 */
	void AudioComponent::UnloadSound()
	{
		audioEngine->UnloadSound(&loadedSound);
	}

	/**
	 * First stops the current sound, then generates a channel ID and
	 * stores it in the emitter. Then calls play through the audio engine.
	 * 
	 */
	void AudioComponent::Play()
	{
		Stop();
		currentChannelID = audioEngine->GenerateChannelID(emitterID, emitterSeed);
		audioEngine->Play(loadedSound, currentChannelID);
	}

	/**
	 * Play a sound without stopping or changing the current channel ID.
	 * 
	 */
	void AudioComponent::PlayOneShot()
	{
		unsigned int channelID = audioEngine->GenerateChannelID(emitterID, emitterSeed);
		audioEngine->Play(loadedSound, channelID);
	}

	void AudioComponent::Pause()
	{
		audioEngine->Pause(currentChannelID);
	}

	void AudioComponent::Resume()
	{
		audioEngine->Resume(currentChannelID);
	}

	void AudioComponent::Stop()
	{
		audioEngine->Stop(currentChannelID);
	}

	bool AudioComponent::IsPaused()
	{
		return audioEngine->IsPaused(currentChannelID);
	}

	bool AudioComponent::IsPlaying()
	{
		return audioEngine->IsPlaying(currentChannelID);
	}

	void AudioComponent::SetSoundFileName(const char* filename)
	{
		soundFileName = filename;
	}

	void AudioComponent::SetSoundLoop(bool loop)
	{
		soundLoop = loop;
	}

	void AudioComponent::SetSound3D(bool s3D)
	{
		sound3D = s3D;
	}

	void AudioComponent::OnCreate()
	{
		// get a reference to the audio engine
		//audioEngine = 

		// set the emitter ID 
		emitterID = audioEngine->GenerateEmitterID();

		// load the sound with the desired settings
		LoadSound();

		// get a reference to a transform component
		transform = entity->GetComponent<MCK::EntitySystem::TransformComponent>();
	}

	void AudioComponent::OnUpdate()
	{
		// set the position of the current channel
		audioEngine->SetPosition(emitterID, transform->Position());
	}

	void AudioComponent::OnDestroy()
	{
		// stop the current channel
		Stop();

		// unload the sound
		UnloadSound();
	}

}
