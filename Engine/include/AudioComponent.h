#pragma once

#include "Component.h"
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