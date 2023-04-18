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

        for (auto it = loadedSounds.begin(); it != loadedSounds.end(); )
        {
            UnloadSound(&(it->second));
            it = loadedSounds.erase(it);
        }

        audioSystem->release();
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
}
