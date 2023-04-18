#include "AudioChannel.h"

namespace MCK::Audio
{
	bool AudioChannel::IsPlaying()
	{
		bool playing = false;
		AudioChannel::fChannel->isPlaying(&playing);
		return playing;
	}

	void AudioChannel::SetPosition(Eigen::Vector3f position)
	{
		// convert to FMOD vector
		FMOD_VECTOR fVec;
		fVec.x = position[0];
		fVec.y = position[1];
		fVec.z = position[2];

		AudioChannel::fChannel->set3DAttributes(&fVec, NULL);
	}
}
