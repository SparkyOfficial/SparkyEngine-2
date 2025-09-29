#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
#include <AL/al.h>
#include <AL/alc.h>

namespace Sparky {
    
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
        const glm::vec3& getListenerPosition() const { return listenerPosition; }
        const glm::vec3& getListenerOrientation() const { return listenerOrientation; }
        
        // Source controls
        void setSoundPosition(ALuint source, const glm::vec3& position);
        void setSoundVelocity(ALuint source, const glm::vec3& velocity);
        void setSoundVolume(ALuint source, float volume);
        void setSoundPitch(ALuint source, float pitch);
        bool isSoundPlaying(ALuint source);
        
        // Preset sounds for common game events
        void playGunshotSound(const glm::vec3& position);
        void playExplosionSound(const glm::vec3& position);
        void playFootstepSound(const glm::vec3& position);
        void playBackgroundMusic(const std::string& filepath);

    private:
        AudioEngine();
        ~AudioEngine();
        
        ALCdevice* device;
        ALCcontext* context;
        
        std::unordered_map<std::string, ALuint> soundBuffers;
        std::vector<ALuint> soundSources;
        
        glm::vec3 listenerPosition;
        glm::vec3 listenerOrientation;
    };
}