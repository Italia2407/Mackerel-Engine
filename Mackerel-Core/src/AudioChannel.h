#pragma once
#include "../FMOD/Include/fmod.hpp"
#include "../FMOD/Include/fmod_studio.hpp"
#include "../ext/Eigen 3.4.0/Eigen/Eigen.h"

namespace MCK::Audio
{
	/** An audio channel is the instance of a sound being played */
	struct AudioChannel
	{
		unsigned int id;
		FMOD::Channel* fChannel;

		bool IsPlaying();

		void SetPosition(Eigen::Vector3f position);
	};
}

