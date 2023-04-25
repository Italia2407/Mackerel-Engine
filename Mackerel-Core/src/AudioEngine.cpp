#include "AudioEngine.h"
#include "LoggingSystem.h"

namespace MCK::Audio
{
    void AudioEngine::Initialise()
    {
        FMOD_RESULT result;

        result = FMOD::System_Create(&audioSystem); // Create the main system object.
        if (result != FMOD_OK)
        {
            std::ostringstream output;
            output << "Unable to intitialise audio system" << std::endl;
            std::string outputStr = output.str();

            MCK::Logger::log(outputStr, MCK::Logger::LogLevel::Error, std::source_location::current());
        }

        result = audioSystem->init(512, FMOD_INIT_NORMAL, 0); // Initialize FMOD with 512 virtual channels.
        if (result != FMOD_OK)
        {
            std::ostringstream output;
            output << "Unable to intitialise audio system" << std::endl;
            std::string outputStr = output.str();

            MCK::Logger::log(outputStr, MCK::Logger::LogLevel::Error, std::source_location::current());
        }

        // Initialise the audio listener
        audioSystem->set3DNumListeners(1);

        FMOD_VECTOR zero = FMOD_VECTOR(0, 0, 0);
        FMOD_VECTOR forward = FMOD_VECTOR(0, 0, 1);
        FMOD_VECTOR up = FMOD_VECTOR(0, 1, 0);

        audioSystem->set3DListenerAttributes(0, &zero, &zero, &forward, &up);
    }

    /**
    * 
    *
    */
    void AudioEngine::Reset()
    {
    }

    /**
    * Releases all resources and shuts down the audio system.
    *
    */
    void AudioEngine::Teardown()
    {
        Reset();

        for (auto it = loadedSounds.begin(); it != loadedSounds.end(); ++it)
        {
            UnloadSound(&(it->second));
            it = loadedSounds.erase(it);
        }

        audioSystem->release();
    }

    void AudioEngine::Update()
    {
        std::vector<unsigned int> stoppedChannelIDs;

        // Find all stopped channels and add them to stoppedChannelIDs
        for (auto it = channels.begin(); it != channels.end(); it++)
        {
            unsigned int id = it->first;
            AudioChannel channel = it->second;
            
            // Find is paused/is playing
            bool isPaused = false;
            channel.fChannel->getPaused(&isPaused);

            bool isPlaying = false;
            channel.fChannel->isPlaying(&isPlaying);

            // Sound finished - add to remove list
            if (!isPlaying && !isPaused) 
            {
                stoppedChannelIDs.push_back(id);
            }
        }

        // Stop all dead sounds
        for (unsigned int i = 0; i < stoppedChannelIDs.size(); ++i)
        {
            channels[stoppedChannelIDs[i]].fChannel->stop();
            channels.erase(stoppedChannelIDs[i]);
        }
        
        // Update
        audioSystem->update();
    }

    /**
    * Loads sound from file and stores in the 'loadedSounds' map
    *
    */
    Sound AudioEngine::LoadSound(const char* filename, unsigned int soundID, bool s3D, bool looping, bool streamed)
    {
        // checks sound doesn't already exist
        auto found = loadedSounds.find(soundID);
        if (found != loadedSounds.end())
            return found->second;

        FMOD_MODE mode = FMOD_DEFAULT;
        mode |= s3D ? FMOD_3D : FMOD_2D;
        mode |= looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
        mode |= streamed ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

        FMOD::Sound* newSound = nullptr;
        FMOD_RESULT result = audioSystem->createSound(filename, mode, nullptr, &newSound);

        if (result == FMOD_OK && newSound)
        {
            Sound sound = { soundID, newSound, s3D, looping, streamed };
            loadedSounds[soundID] = sound;
            return sound;
        }
        else
        {
            std::ostringstream output;
            output << "SoundID: " << soundID << " could not be loaded";
            std::string outputStr = output.str();

            MCK::Logger::log(outputStr, MCK::Logger::LogLevel::Error, std::source_location::current());

            Sound emptySound = { 0, nullptr, false, false, false };
            return emptySound;
        }
    }

    /**
    * Unloads sound from memory and removes it from 'loadedSounds' map
    *
    */
    void AudioEngine::UnloadSound(Sound* sound)
    {
        if (sound)
        {
            auto it = loadedSounds.find(sound->id);
            if (it != loadedSounds.end())
            {
                sound->sound->release();
                loadedSounds.erase(it);
            }
        }
    }

    void AudioEngine::Play(Sound sound, unsigned int channelID)
    {
        // Is there already a sound with this ID?
        auto found = channels.find(channelID);
        if (found != channels.end())
        {
            // Stop that sound
            StopUnsafe(channelID);
        }

        // Play the sound
        FMOD::Channel* fmodChannel = nullptr;
        audioSystem->playSound(sound.sound, nullptr, false, &fmodChannel);

        // Construct channel
        AudioChannel channel{};
        channel.id = channelID;
        channel.fChannel = fmodChannel;

        // Add this sound
        channels[channel.id] = channel;
    }

    void AudioEngine::Stop(unsigned int id)
    {
        auto found = channels.find(id);
        if (found != channels.end())
        {
            // Stop this sound
            StopUnsafe(id);
        }
    }

    void AudioEngine::StopUnsafe(unsigned int id)
    {
        channels[id].fChannel->stop();
        loadedSounds.erase(id);
    }

    void AudioEngine::Pause(unsigned int id)
    {
        if (channels.count(id))
        {
            channels[id].fChannel->setPaused(true);
        }
    }
    
    void AudioEngine::Resume(unsigned int id)
    {
        if (channels.count(id))
        {
            channels[id].fChannel->setPaused(false);
        }
    }

    void AudioEngine::SetPosition(unsigned int id, Eigen::Vector3f position)
    {
        if (channels.count(id))
        {
            // convert to FMOD vector
            FMOD_VECTOR fVec;
            fVec.x = position[0];
            fVec.y = position[1];
            fVec.z = position[2];

            channels[id].fChannel->set3DAttributes(&fVec, NULL);
        }

    }

    bool AudioEngine::IsPlaying(unsigned int id)
    {
        if (channels.count(id))
        {
            bool playing = false;
            channels[id].fChannel->isPlaying(&playing);
            return playing;
        }
        return false;
    }

    bool AudioEngine::IsPaused(unsigned int id)
    {
        if (channels.count(id))
        {
            bool paused = false;
            channels[id].fChannel->getPaused(&paused);
            return paused;
        }
        return false;
    }

    bool AudioEngine::HasChannel(unsigned int id)
    {
        return bool(channels.count(id));
    }

    unsigned int AudioEngine::GenerateEmitterID()
    {
        return ++curEmitterID;
    }

    unsigned int AudioEngine::GenerateChannelID(unsigned int emitterID, unsigned int& emitterSeed)
    {
        ++emitterSeed;
        return emitterID * channelsPerEmitter + (emitterSeed % channelsPerEmitter);
    }

    void AudioEngine::UpdateTransform(Eigen::Vector3f position, Eigen::Quaternionf rotation)
    {
        // rotated forward direction
        Eigen::Vector3f forward_rotated = rotation * Eigen::Vector3f(0, 0, 1);

        // calculate rotated up direction
        Eigen::Vector3f up_rotated = (forward_rotated.cross(Eigen::Vector3f(0, 1, 0))).normalized();

        // convert to FMOD
        FMOD_VECTOR pos = FMOD_VECTOR(position[0], position[1], position[2]);

        FMOD_VECTOR forward = FMOD_VECTOR(forward_rotated[0], forward_rotated[1], forward_rotated[2]);
        FMOD_VECTOR up = FMOD_VECTOR(up_rotated[0], up_rotated[1], up_rotated[2]);

        // velocity remains zero
        FMOD_VECTOR zero = FMOD_VECTOR(0, 0, 0);

        audioSystem->set3DListenerAttributes(0, &pos, &zero, &forward, &up);
    }
}
