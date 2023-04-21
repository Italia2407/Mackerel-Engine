#pragma once
#include "AudioChannel.h"
#include "Sound.h"

#include<map>

namespace MCK::Audio
{
	class AudioEngine
	{
		FMOD::System* audioSystem;

		std::map<unsigned int, Sound> loadedSounds;
		std::map<unsigned int, AudioChannel> channels;

		public:
			const unsigned int channelsPerEmitter = 10;

			/**
			 * Initialises the audio engine.
			 * 
			 */
			void Initialise();

			/**
			 * Resets the engine.
			 * 
			 */
			void Reset();

			/**
			* Releases all resources and shuts down the audio system.
			*
			*/
			void Teardown();

			/**
			 * Updates the audio engine.
			 * 
			 */
			void Update();

			/**
			* Loads sound from file and stores in the 'loadedSounds' map
			*
			*/
			Sound LoadSound(const char* filename, unsigned int soundID, bool s3D, bool looping, bool streamed);
			
			/**
			* Unloads sound from memory and removes it from 'loadedSounds' map
			*
			*/
			void UnloadSound(Sound* sound);

			/**
			 * Plays a sound.
			 * 
			 * \param sound: The sound to play
			 * \param channelID: The channel to play it in
			 */
			void Play(Sound sound, unsigned int channelID);

			/**
			 * Stops a channel.
			 * 
			 * \param id: The id of the channel
			 */
			void Stop(unsigned int id);

			/**
			 * Pauses a channel.
			 *
			 * \param id: The id of the channel
			 */
			void Pause(unsigned int id);

			/**
			 * Resumes a paused channel.
			 *
			 * \param id: The id of the channel
			 */
			void Resume(unsigned int id);

			/**
			 * Sets the position of a channel
			 *
			 * \param id: The id of the channel
			 * \param position: The position of the channel in world space
			 */
			void SetPosition(unsigned int id, Eigen::Vector3f position);

			/**
			 * Whether a channel is playing.
			 * 
			 * \param id: The id of the channel 
			 */
			bool IsPlaying(unsigned int id);

			/**
			 * Whether a channel is paused.
			 *
			 * \param id: The id of the channel
			 */
			bool IsPaused(unsigned int id);

			/**
			 * Whether a channel exists.
			 *
			 * \param id: The id of the channel
			 */
			bool HasChannel(unsigned int id);

			/**
			 * Generates an ID for an audio emitter.
			 * 
			 */
			unsigned int GenerateEmitterID();

			/**
			 * Generates a channel ID for an audio emitter.
			 * 
			 * \param emitterID The ID of the emitter
			 * \param emitterSeed The current seed of the emitter
			 * \return A channel ID
			 */
			unsigned int GenerateChannelID(unsigned int emitterID, 
				unsigned int &emitterSeed);
		private:
			// The current emitter ID to use
			unsigned int curEmitterID = 0;

			/**
			 * Stops a channel without checking if it exists.
			 * 
			 */
			void StopUnsafe(unsigned int id);
	};

}