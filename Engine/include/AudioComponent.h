#pragma once

#include "Component.h"
#include "AudioEngine.h" // Include AudioEngine for AudioSourceProperties
#include <string>
#include <unordered_map>
#include <AL/al.h>

namespace Sparky {
    
    class AudioComponent : public Component {
    public:
        AudioComponent();
        virtual ~AudioComponent();
        
        void update(float deltaTime) override;
        void render() override;
        
        // Audio playback methods
        bool loadSound(const std::string& name, const std::string& filepath);
        ALuint playSound(const std::string& name, bool loop = false);
        void stopSound(const std::string& name);
        void stopSound(ALuint source);
        
        // Sound control methods
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
        
        // Preset sounds
        void playGunshot();
        void playExplosion();
        void playFootstep();
        
        // Getters
        const std::unordered_map<std::string, ALuint>& getSoundSources() const { return soundSources; }

    private:
        std::unordered_map<std::string, ALuint> soundSources;
        bool followOwner; // Whether sounds should follow the owner's position
    };
}