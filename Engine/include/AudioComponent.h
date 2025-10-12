#pragma once

#include "Component.h"
#include "AudioEngine.h" // Include AudioEngine for AudioSourceProperties
#include <string>
#include <unordered_map>

#ifdef ENABLE_AUDIO
#include <AL/al.h>
#endif

namespace Sparky {
    
    class AudioComponent : public Component {
    public:
        AudioComponent();
        virtual ~AudioComponent();
        
        void update(float deltaTime) override;
        void render() override;
        
        // Audio playback methods
#ifdef ENABLE_AUDIO
        bool loadSound(const std::string& name, const std::string& filepath);
        ALuint playSound(const std::string& name, bool loop = false);
        void stopSound(const std::string& name);
        void stopSound(ALuint source);
#else
        bool loadSound(const std::string& name, const std::string& filepath) { return false; }
        unsigned int playSound(const std::string& name, bool loop = false) { return 0; }
        void stopSound(const std::string& name) {}
        void stopSound(unsigned int source) {}
#endif
        
        // Sound control methods
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
        
        // Preset sounds
        void playGunshot();
        void playExplosion();
        void playFootstep();
        
        // Getters
#ifdef ENABLE_AUDIO
        const std::unordered_map<std::string, ALuint>& getSoundSources() const { return soundSources; }
#else
        const std::unordered_map<std::string, unsigned int>& getSoundSources() const { 
            static std::unordered_map<std::string, unsigned int> empty;
            return empty;
        }
#endif

    private:
#ifdef ENABLE_AUDIO
        std::unordered_map<std::string, ALuint> soundSources;
#else
        std::unordered_map<std::string, unsigned int> soundSources;
#endif
        bool followOwner; // Whether sounds should follow the owner's position
    };
}