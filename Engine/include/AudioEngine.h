#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <AL/al.h>
#include <AL/alc.h>

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
        bool loadSound(const std::string& name, const std::string& filepath);
        ALuint playSound(const std::string& name, bool loop = false);
        void stopSound(ALuint source);
        
        // Listener controls
        void setListenerPosition(const glm::vec3& position);
        void setListenerOrientation(const glm::vec3& forward, const glm::vec3& up);
        void setListenerVelocity(const glm::vec3& velocity);
        const glm::vec3& getListenerPosition() const { return listenerPosition; }
        const glm::vec3& getListenerOrientation() const { return listenerOrientation; }
        
        // Source controls
        void setSoundPosition(ALuint source, const glm::vec3& position);
        void setSoundVelocity(ALuint source, const glm::vec3& velocity);
        void setSoundVolume(ALuint source, float volume);
        void setSoundPitch(ALuint source, float pitch);
        bool isSoundPlaying(ALuint source);
        
        // Advanced audio positioning and effects
        void setSoundProperties(ALuint source, const AudioSourceProperties& properties);
        void setSoundDistanceModel(ALuint source, int model = AL_INVERSE_DISTANCE_CLAMPED);
        void setSoundAttenuation(ALuint source, float minDistance, float maxDistance, float rolloffFactor);
        void setSoundCone(ALuint source, float innerAngle, float outerAngle, float outerGain);
        void setSoundDoppler(ALuint source, bool enable, float factor = 1.0f);
        
        // Audio effects
        bool createAudioEffect(AudioEffectType type, const std::string& name);
        void applyAudioEffect(ALuint source, const std::string& effectName);
        void removeAudioEffect(ALuint source, const std::string& effectName);
        
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
        
        ALCdevice* device;
        ALCcontext* context;
        
        std::unordered_map<std::string, ALuint> soundBuffers;
        std::vector<ALuint> soundSources;
        std::unordered_map<std::string, ALuint> audioEffects;
        
        glm::vec3 listenerPosition;
        glm::vec3 listenerOrientation;
        glm::vec3 listenerVelocity;
    };
}