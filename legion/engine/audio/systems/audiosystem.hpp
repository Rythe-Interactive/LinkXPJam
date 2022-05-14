#pragma once
#include <audio/components/audio_source.hpp>
#include <audio/components/audio_listener.hpp>

#define AL_ALEXT_PROTOTYPES
#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)

#if !defined(DOXY_EXCLUDE)
#include <AL/alext.h>
#endif

#include <audio/data/importers/audio_importers.hpp>

/**
 * @file audiosystem.hpp
 */

namespace legion::audio
{
    inline void openal_error()
    {
        ALenum error;
        while ((error = alGetError()) != AL_NO_ERROR)
        {
            std::string errMsg = std::to_string(error);
            switch (error)
            {
            case AL_INVALID_NAME:
                errMsg = "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                break;
            case AL_INVALID_ENUM:
                errMsg = "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                break;
            case AL_INVALID_VALUE:
                errMsg = "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                break;
            case AL_INVALID_OPERATION:
                errMsg = "AL_INVALID_OPERATION: the requested operation is not valid";
                break;
            case AL_OUT_OF_MEMORY:
                errMsg = "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                break;
            default:
                errMsg = "UNKNOWN AL ERROR: " + std::to_string(error);
            }
            legion::log::warn("ERROR: OpenAl error: {}", errMsg);
        }
    }

    /**@class AudioSystem
     * @brief This is a system that handles audio components.
     */
    class AudioSystem final : public System<AudioSystem>
    {
    public:
        /**@brief Function for initializing the system.
         */
        void setup();

        void onEngineExit(events::exit& event);

        /**
        * @brief Function callback for audio_source component creation.
        * @brief Initiallizes sound source and sound file
        */
        void onAudioSourceComponentCreate(events::component_creation<audio_source>& event);

        /**
        * @brief Function callback for audio_source component destruction.
        */
        // NOTE TO SELF
        // Make sure this releases the audio file and source
        void onAudioSourceComponentDestroy(events::component_destruction<audio_source>& event);

        /**
        * @brief Function callback for audio_listener component creation.
        * @brief Initiallizes sound listener
        */
        void onAudioListenerComponentCreate(events::component_creation<audio_listener>& event);

        /**
        * @brief Function callback for audio_listener component destruction.
        */
        void onAudioListenerComponentDestroy(events::component_destruction<audio_listener>& event);

        void update(time::span deltatime);

        static void setDistanceModel(ALenum distanceModel);

        static async::spinlock contextLock;
        static ALCcontext* alcContext;
    private:
        void initSource(audio_source& source);

        /**
        * @brief Function to print information about openal.
        * @brief Information that will be printed includes:
        * @brief Vendor, Version, Renderer, OpenALExtensions,
        * @brief ALC Extentions and device sample rate
        */
        void queryInformation();
        void setListener(position p, rotation r);
        ecs::entity m_listenerEnt;

        float maxSourceDistance2 = 1000;
        position m_listenerPosition;
        std::vector<ALuint> m_sourceIds;
        static ALCint frequency, refresh, sync, monoSources, stereoSources, maxAux;

        static ALCdevice* alDevice;
        static unsigned int sourceCount;
        static unsigned int listenerCount;
    };
}
