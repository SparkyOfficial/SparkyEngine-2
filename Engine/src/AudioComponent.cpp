#include "../include/AudioComponent.h"
#include "../include/AudioEngine.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"

namespace Sparky {
    
    AudioComponent::AudioComponent() : Component(), followOwner(true) {
        SPARKY_LOG_DEBUG("AudioComponent created");
    }
    
    AudioComponent::~AudioComponent() {
        SPARKY_LOG_DEBUG("AudioComponent destroyed");
        
        // Stop all playing sounds
        for (auto& pair : soundSources) {
            AudioEngine::getInstance().stopSound(pair.second);
        }
        soundSources.clear();
    }
    
    void AudioComponent::update(float deltaTime) {
        // Update sound positions to follow owner if needed
        if (followOwner && owner) {
            glm::vec3 ownerPosition = owner->getPosition();
            for (auto& pair : soundSources) {
                AudioEngine::getInstance().setSoundPosition(pair.second, ownerPosition);
            }
        }
    }
    
    void AudioComponent::render() {
        // Audio components don't render anything
    }
    
    bool AudioComponent::loadSound(const std::string& name, const std::string& filepath) {
        return AudioEngine::getInstance().loadSound(name, filepath);
    }
    
    ALuint AudioComponent::playSound(const std::string& name, bool loop) {
        // Play the sound
        ALuint source = AudioEngine::getInstance().playSound(name, loop);
        
        if (source != 0) {
            // Store the source
            soundSources[name] = source;
            
            // If following owner, set the sound position to the owner's position
            if (followOwner && owner) {
                AudioEngine::getInstance().setSoundPosition(source, owner->getPosition());
            }
        }
        
        return source;
    }
    
    void AudioComponent::stopSound(const std::string& name) {
        auto it = soundSources.find(name);
        if (it != soundSources.end()) {
            AudioEngine::getInstance().stopSound(it->second);
            soundSources.erase(it);
        }
    }
    
    void AudioComponent::stopSound(ALuint source) {
        AudioEngine::getInstance().stopSound(source);
        
        // Remove from our map
        for (auto it = soundSources.begin(); it != soundSources.end(); ++it) {
            if (it->second == source) {
                soundSources.erase(it);
                break;
            }
        }
    }
    
    void AudioComponent::setSoundPosition(ALuint source, const glm::vec3& position) {
        AudioEngine::getInstance().setSoundPosition(source, position);
    }
    
    void AudioComponent::setSoundVelocity(ALuint source, const glm::vec3& velocity) {
        AudioEngine::getInstance().setSoundVelocity(source, velocity);
    }
    
    void AudioComponent::setSoundVolume(ALuint source, float volume) {
        AudioEngine::getInstance().setSoundVolume(source, volume);
    }
    
    void AudioComponent::setSoundPitch(ALuint source, float pitch) {
        AudioEngine::getInstance().setSoundPitch(source, pitch);
    }
    
    bool AudioComponent::isSoundPlaying(ALuint source) {
        return AudioEngine::getInstance().isSoundPlaying(source);
    }
    
    void AudioComponent::setSoundProperties(ALuint source, const AudioSourceProperties& properties) {
        AudioEngine::getInstance().setSoundProperties(source, properties);
    }
    
    void AudioComponent::setSoundDistanceModel(ALuint source, int model) {
        AudioEngine::getInstance().setSoundDistanceModel(source, model);
    }
    
    void AudioComponent::setSoundAttenuation(ALuint source, float minDistance, float maxDistance, float rolloffFactor) {
        AudioEngine::getInstance().setSoundAttenuation(source, minDistance, maxDistance, rolloffFactor);
    }
    
    void AudioComponent::setSoundCone(ALuint source, float innerAngle, float outerAngle, float outerGain) {
        AudioEngine::getInstance().setSoundCone(source, innerAngle, outerAngle, outerGain);
    }
    
    void AudioComponent::setSoundDoppler(ALuint source, bool enable, float factor) {
        AudioEngine::getInstance().setSoundDoppler(source, enable, factor);
    }
    
    bool AudioComponent::createAudioEffect(AudioEffectType type, const std::string& name) {
        return AudioEngine::getInstance().createAudioEffect(type, name);
    }
    
    void AudioComponent::applyAudioEffect(ALuint source, const std::string& effectName) {
        AudioEngine::getInstance().applyAudioEffect(source, effectName);
    }
    
    void AudioComponent::removeAudioEffect(ALuint source, const std::string& effectName) {
        AudioEngine::getInstance().removeAudioEffect(source, effectName);
    }
    
    void AudioComponent::playGunshot() {
        // Play a gunshot sound at the owner's position
        if (owner) {
            AudioEngine::getInstance().playGunshotSound(owner->getPosition());
        }
    }
    
    void AudioComponent::playExplosion() {
        // Play an explosion sound at the owner's position
        if (owner) {
            AudioEngine::getInstance().playExplosionSound(owner->getPosition());
        }
    }
    
    void AudioComponent::playFootstep() {
        // Play a footstep sound at the owner's position
        if (owner) {
            AudioEngine::getInstance().playFootstepSound(owner->getPosition());
        }
    }
}