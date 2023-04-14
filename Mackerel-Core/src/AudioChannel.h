#pragma once
#include "../FMOD/Include/fmod.hpp"
#include "../FMOD/Include/fmod_studio.h"

/** An audio channel is the instance of a sound being played */
struct AudioChannel
{
	unsigned int id;
	FMOD::Channel* fChannel;

	bool isPlaying;
};
