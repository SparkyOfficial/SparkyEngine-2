#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

#ifdef ENABLE_AUDIO
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace Sparky {
    
    // Audio effect types
    enum class AudioEffectType {
        REVERB,
        ECHO,
        FLANGER,
        CHORUS
    };
    
    // Audio source properties
    struct AudioSourceProperties {
        float minDistance = 1.0f;
        float maxDistance = 100.0f;
        float rolloffFactor = 1.0f;
        float coneInnerAngle = 360.0f;
        float coneOuterAngle = 360.0f;
        float coneOuterGain = 0.0f;
        bool enableDoppler = true;
        float dopplerFactor = 1.0f;
    };
    
    class AudioEngine {
    public:
        static AudioEngine& getInstance();
        
        bool initialize();
        void cleanup();
        
        // Sound management
#ifdef ENABLE_AUDIO
        bool loadSound(const std::string& name, const std::string& filepath);
        ALuint playSound(const std::string& name, bool loop = false);
        void stopSound(ALuint source);
#else
        bool loadSound(const std::string& name, const std::string& filepath) { return false; }
        unsigned int playSound(const std::string& name, bool loop = false) { return 0; }
        void stopSound(unsigned int source) {}
#endif
        
        // Listener controls
        void setListenerPosition(const glm::vec3& position);
        void setListenerOrientation(const glm::vec3& forward, const glm::vec3& up);
        void setListenerVelocity(const glm::vec3& velocity);
        const glm::vec3& getListenerPosition() const { return listenerPosition; }
        const glm::vec3& getListenerOrientation() const { return listenerOrientation; }
        
        // Source controls
#ifdef ENABLE_AUDIO
        void setSoundPosition(ALuint source, const glm::vec3& position);
        void setSoundVelocity(ALuint source, const glm::vec3& velocity);
        void setSoundVolume(ALuint source, float volume);
        void setSoundPitch(ALuint source, float pitch);
        bool isSoundPlaying(ALuint source);
#else
        void setSoundPosition(unsigned int source, const glm::vec3& position) {}
        void setSoundVelocity(unsigned int source, const glm::vec3& velocity) {}
        void setSoundVolume(unsigned int source, float volume) {}
        void setSoundPitch(unsigned int source, float pitch) {}
        bool isSoundPlaying(unsigned int source) { return false; }
#endif
        
        // Advanced audio positioning and effects
#ifdef ENABLE_AUDIO
        void setSoundProperties(ALuint source, const AudioSourceProperties& properties);
        void setSoundDistanceModel(ALuint source, int model = AL_INVERSE_DISTANCE_CLAMPED);
        void setSoundAttenuation(ALuint source, float minDistance, float maxDistance, float rolloffFactor);
        void setSoundCone(ALuint source, float innerAngle, float outerAngle, float outerGain);
        void setSoundDoppler(ALuint source, bool enable, float factor = 1.0f);
#else
        void setSoundProperties(unsigned int source, const AudioSourceProperties& properties) {}
        void setSoundDistanceModel(unsigned int source, int model = 0) {}
        void setSoundAttenuation(unsigned int source, float minDistance, float maxDistance, float rolloffFactor) {}
        void setSoundCone(unsigned int source, float innerAngle, float outerAngle, float outerGain) {}
        void setSoundDoppler(unsigned int source, bool enable, float factor = 1.0f) {}
#endif
        
        // Audio effects
#ifdef ENABLE_AUDIO
        bool createAudioEffect(AudioEffectType type, const std::string& name);
        void applyAudioEffect(ALuint source, const std::string& effectName);
        void removeAudioEffect(ALuint source, const std::string& effectName);
#else
        bool createAudioEffect(AudioEffectType type, const std::string& name) { return false; }
        void applyAudioEffect(unsigned int source, const std::string& effectName) {}
        void removeAudioEffect(unsigned int source, const std::string& effectName) {}
#endif
        
        // Preset sounds for common game events
        void playGunshotSound(const glm::vec3& position);
        void playExplosionSound(const glm::vec3& position);
        void playFootstepSound(const glm::vec3& position);
        void playBackgroundMusic(const std::string& filepath);
        
        // Utility functions
        float calculateDistanceAttenuation(const glm::vec3& sourcePos, const glm::vec3& listenerPos, 
                                        float minDistance, float maxDistance, float rolloffFactor);

    private:
        AudioEngine();
        ~AudioEngine();
        
#ifdef ENABLE_AUDIO
        ALCdevice* device;
        ALCcontext* context;
        
        std::unordered_map<std::string, ALuint> soundBuffers;
        std::vector<ALuint> soundSources;
        std::unordered_map<std::string, ALuint> audioEffects;
#else
        // Dummy variables when audio is disabled
        void* device;
        void* context;
        
        std::unordered_map<std::string, unsigned int> soundBuffers;
        std::vector<unsigned int> soundSources;
        std::unordered_map<std::string, unsigned int> audioEffects;
#endif
        
        glm::vec3 listenerPosition;
        glm::vec3 listenerOrientation;
        glm::vec3 listenerVelocity;
    };
}