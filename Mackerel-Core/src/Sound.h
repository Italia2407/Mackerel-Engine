#pragma once
#include "../FMOD/Include/fmod.hpp"
#include "../FMOD/Include/fmod_studio.h"

namespace MCK::Audio
{
	struct Sound
	{
		unsigned int id = 0;
		FMOD::Sound* sound = nullptr;
		bool s3D = false;
		bool looping = false;
		bool streamed = false;
	};
}