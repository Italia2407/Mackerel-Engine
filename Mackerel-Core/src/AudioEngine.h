#pragma once
#include "AudioChannel.h"
#include "Sound.h"

#include<map>

namespace MCK::Audio
{
	class AudioEngine
	{
		FMOD::System* audioSysten;

		std::map<unsigned int, Sound> loadedSounds;
		std::map<unsigned int, AudioChannel> channels;

		// TODO Implement me - commented so it compiles
		// void Initialise();
		// void Reset();
		// void Teardown();

		// Sound LoadSound(const char* filename, unsigned int soundID, bool s3D, bool looping, bool streamed);
		// void UnloadSound(Sound* sound);
	};

}