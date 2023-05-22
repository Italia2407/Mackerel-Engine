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
	void AudioEmitter::LoadSound()
	{
		// check if a sound has already been loaded, if yes, unload it
		if (loadedSound.sound != nullptr)
			UnloadSound();

		// load the sound by calling LoadSound in the audio engine
		loadedSound = audioEngine->LoadSound(soundFileName.c_str(), emitterID, sound3D, soundLoop, false);
	}

	/**
	 * Function to unload the sound previously loaded by the audio emitter.
	 * 
	 */
	void AudioEmitter::UnloadSound()
	{
		audioEngine->UnloadSound(&loadedSound);
	}

	/**
	 * First stops the current sound, then generates a channel ID and
	 * stores it in the emitter. Then calls play through the audio engine.
	 * 
	 */
	void AudioEmitter::Play()
	{
		Stop();
		currentChannelID = audioEngine->GenerateChannelID(emitterID, emitterSeed);
		audioEngine->Play(loadedSound, currentChannelID);
	}

	/**
	 * Play a sound without stopping or changing the current channel ID.
	 * 
	 */
	void AudioEmitter::PlayOneShot()
	{
		unsigned int channelID = audioEngine->GenerateChannelID(emitterID, emitterSeed);
		audioEngine->Play(loadedSound, channelID);
	}

	/**
	 * Pauses the audio of the current channel.
	 * 
	 */
	void AudioEmitter::Pause()
	{
		audioEngine->Pause(currentChannelID);
	}

	/**
	 * Resumes the audio of the current channel.
	 * 
	 */
	void AudioEmitter::Resume()
	{
		audioEngine->Resume(currentChannelID);
	}

	/**
	 * Stops the audio of the current channel.
	 * 
	 */
	void AudioEmitter::Stop()
	{
		audioEngine->Stop(currentChannelID);
	}

	/**
	 * Checks if the audio in the current channel is paused.
	 * 
	 * \return true if audio is paused, false otherwise
	 */
	bool AudioEmitter::IsPaused()
	{
		return audioEngine->IsPaused(currentChannelID);
	}

	/**
	 * Checks if the audio in the current channel is playing.
	 * 
	 * \return true if audio is playing, false otherwise
	 */
	bool AudioEmitter::IsPlaying()
	{
		return audioEngine->IsPlaying(currentChannelID);
	}

	/**
	 * Setter function to set the filename for the sound to be loaded.
	 * 
	 * \param filename
	 */
	void AudioEmitter::SetSoundFileName(std::string filename)
	{
		soundFileName = filename;
	}

	/**
	 * Setter function to set whether the audio to be loaded 
	 *	should be looped.
	 * 
	 * \param loop
	 */
	void AudioEmitter::SetSoundLoop(bool loop)
	{
		soundLoop = loop;
	}

	/**
	 * Setter function to set whether the audio to be loaded
	 *	is a 3D sound.
	 * 
	 * \param s3D
	 */
	void AudioEmitter::SetSound3D(bool s3D)
	{
		sound3D = s3D;
	}

	/**
	 * When the audio component is created, it sets some data
	 *	and loads a sound based on the settings stored in the emitter.
	 * 
	 */
	void AudioEmitter::OnCreate()
	{
		// get a reference to the audio engine
		audioEngine = &entity->scene->audioEngine;

		// set the emitter ID 
		emitterID = audioEngine->GenerateEmitterID();

		// load the sound with the desired settings
		LoadSound();

		// get a reference to a transform component
		transform = entity->GetComponent<MCK::EntitySystem::TransformComponent>();

		Play();
	}

	/**
	 * Each frame, updates the position of the emitter.
	 * 
	 */
	void AudioEmitter::OnUpdate()
	{
		// set the position of the current channel
		audioEngine->SetChannelPosition(currentChannelID, transform->Position());
	}

	/**
	 * When the component is destroyed, stops the sound and unloads it.
	 * 
	 */
	void AudioEmitter::OnDestroy()
	{
		// stop the current channel
		Stop();

		// unload the sound
		UnloadSound();
	}

	bool AudioEmitter::Deserialise(json data)
	{
		std::string d = data.dump();
		data = data["data"];

		auto it = data.find("filename");
		if (it != data.end())
		{
			soundFileName = data["filename"].get<std::string>();
		}

		int sLoop = data["loop"];
		int s3d = data["3d"];
		soundLoop = (bool)sLoop;
		sound3D = (bool)s3d;

		return true;
	}

}
