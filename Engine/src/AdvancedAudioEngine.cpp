/*
 * AdvancedAudioEngine.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Advanced audio engine implementation with enhanced 3D audio features
 */

#include "../include/AdvancedAudioEngine.h"
#include "../include/Logger.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

namespace Sparky {
    
    AdvancedAudioEngine& AdvancedAudioEngine::getInstance() {
        static AdvancedAudioEngine instance;
        return instance;
    }
    
    AdvancedAudioEngine::AdvancedAudioEngine() : AudioEngine() {
        SPARKY_LOG_DEBUG("AdvancedAudioEngine created");
    }
    
    AdvancedAudioEngine::~AdvancedAudioEngine() {
        cleanup();
        SPARKY_LOG_DEBUG("AdvancedAudioEngine destroyed");
    }
    
    bool AdvancedAudioEngine::initialize() {
        SPARKY_LOG_INFO("Initializing AdvancedAudioEngine");
        
        // Initialize base AudioEngine
        if (!AudioEngine::initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize base AudioEngine");
            return false;
        }
        
#ifdef ENABLE_AUDIO
        try {
            // Check for EFX extension support
            if (alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
                SPARKY_LOG_INFO("EFX extensions supported");
            } else {
                SPARKY_LOG_WARNING("EFX extensions not supported");
            }
            
            // Create default environment
            AudioEnvironment defaultEnv;
            defaultEnv.name = "default";
            createAudioEnvironment(defaultEnv);
            setActiveEnvironment("default");
            
            // Set advanced listener properties
            advancedListenerProperties = AdvancedAudioListenerProperties();
            setAdvancedListenerProperties(advancedListenerProperties);
            
            SPARKY_LOG_INFO("AdvancedAudioEngine initialized successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Exception during AdvancedAudioEngine initialization: " + std::string(e.what()));
            return false;
        }
#else
        SPARKY_LOG_INFO("Audio disabled, AdvancedAudioEngine initialized with dummy implementation");
        return true;
#endif
    }
    
    void AdvancedAudioEngine::cleanup() {
        SPARKY_LOG_INFO("Cleaning up AdvancedAudioEngine");
        
#ifdef ENABLE_AUDIO
        // Clean up effect slots
        for (auto& pair : effectSlots) {
            alDeleteAuxiliaryEffectSlots(1, &pair.second);
        }
        effectSlots.clear();
        
        // Clean up filters
        for (auto& pair : filters) {
            alDeleteFilters(1, &pair.second);
        }
        filters.clear();
        
        // Clean up environments
        audioEnvironments.clear();
#endif
        
        // Clean up base AudioEngine
        AudioEngine::cleanup();
        
        SPARKY_LOG_INFO("AdvancedAudioEngine cleaned up");
    }
    
    void AdvancedAudioEngine::setAdvancedListenerProperties(const AdvancedAudioListenerProperties& properties) {
        advancedListenerProperties = properties;
        
#ifdef ENABLE_AUDIO
        // Set base listener properties
        alListener3f(AL_POSITION, properties.position.x, properties.position.y, properties.position.z);
        alListener3f(AL_VELOCITY, properties.velocity.x, properties.velocity.y, properties.velocity.z);
        
        // Set orientation
        ALfloat orientation[] = {
            properties.orientationForward.x, properties.orientationForward.y, properties.orientationForward.z,
            properties.orientationUp.x, properties.orientationUp.y, properties.orientationUp.z
        };
        alListenerfv(AL_ORIENTATION, orientation);
        
        // Set speed of sound and Doppler factor
        alSpeedOfSound(properties.speedOfSound);
        alDopplerFactor(properties.dopplerFactor);
#endif
    }
    
    const AdvancedAudioListenerProperties& AdvancedAudioEngine::getAdvancedListenerProperties() const {
        return advancedListenerProperties;
    }
    
    bool AdvancedAudioEngine::loadAdvancedSound(const std::string& name, const std::string& filepath, bool streaming) {
#ifdef ENABLE_AUDIO
        // For now, we'll use the base implementation
        // In a real implementation, we would handle streaming differently
        return AudioEngine::loadSound(name, filepath);
#else
        return false;
#endif
    }
    
    ALuint AdvancedAudioEngine::playAdvancedSound(const std::string& name, bool loop, const glm::vec3& position) {
#ifdef ENABLE_AUDIO
        // Play using base implementation
        ALuint source = AudioEngine::playSound(name, loop);
        
        // Set position
        if (source != 0) {
            alSource3f(source, AL_POSITION, position.x, position.y, position.z);
        }
        
        return source;
#else
        return 0;
#endif
    }
    
    void AdvancedAudioEngine::stopAdvancedSound(ALuint source) {
#ifdef ENABLE_AUDIO
        AudioEngine::stopSound(source);
#endif
    }
    
    void AdvancedAudioEngine::setAdvancedSoundProperties(ALuint source, const AdvancedAudioSourceProperties& properties) {
#ifdef ENABLE_AUDIO
        if (source != 0) {
            // Set base properties
            alSourcef(source, AL_REFERENCE_DISTANCE, properties.minDistance);
            alSourcef(source, AL_MAX_DISTANCE, properties.maxDistance);
            alSourcef(source, AL_ROLLOFF_FACTOR, properties.rolloffFactor);
            alSourcef(source, AL_CONE_INNER_ANGLE, properties.coneInnerAngle);
            alSourcef(source, AL_CONE_OUTER_ANGLE, properties.coneOuterAngle);
            alSourcef(source, AL_CONE_OUTER_GAIN, properties.coneOuterGain);
            alSourcef(source, AL_DOPPLER_FACTOR, properties.enableDoppler ? properties.dopplerFactor : 0.0f);
            
            // Set advanced properties if EFX is supported
            if (alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
                // Air absorption
                alSourcef(source, AL_AIR_ABSORPTION_FACTOR, properties.airAbsorptionFactor);
                
                // Room rolloff
                alSourcef(source, AL_ROOM_ROLLOFF_FACTOR, properties.roomRolloffFactor);
                
                // Cone inner gain (if supported)
                // Note: This might not be available on all OpenAL implementations
            }
        }
#endif
    }
    
    void AdvancedAudioEngine::setSoundOcclusion(ALuint source, const AudioOcclusionData& occlusionData) {
#ifdef ENABLE_AUDIO
        if (source != 0 && alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
            // In a full implementation, we would create and apply filters for occlusion
            // For now, we'll just log that the method exists
            SPARKY_LOG_DEBUG("Setting sound occlusion for source");
        }
#endif
    }
    
    void AdvancedAudioEngine::setSoundEnvironment(ALuint source, const std::string& environmentName) {
#ifdef ENABLE_AUDIO
        if (source != 0 && alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
            // In a full implementation, we would apply the environment effect to the source
            // For now, we'll just log that the method exists
            SPARKY_LOG_DEBUG("Setting sound environment: " + environmentName + " for source");
        }
#endif
    }
    
    void AdvancedAudioEngine::setSoundSpatialization(ALuint source, bool enableHRTF) {
#ifdef ENABLE_AUDIO
        if (source != 0) {
            // In a full implementation, we would enable HRTF spatialization
            // For now, we'll just log that the method exists
            SPARKY_LOG_DEBUG("Setting sound spatialization for source, HRTF: " + std::string(enableHRTF ? "enabled" : "disabled"));
        }
#endif
    }
    
    bool AdvancedAudioEngine::createAudioEnvironment(const AudioEnvironment& environment) {
#ifdef ENABLE_AUDIO
        if (alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
            // Store the environment
            audioEnvironments[environment.name] = environment;
            return true;
        }
        return false;
#else
        return false;
#endif
    }
    
    void AdvancedAudioEngine::setActiveEnvironment(const std::string& environmentName) {
#ifdef ENABLE_AUDIO
        auto it = audioEnvironments.find(environmentName);
        if (it != audioEnvironments.end()) {
            activeEnvironment = environmentName;
            // In a full implementation, we would apply the environment settings
            SPARKY_LOG_DEBUG("Active environment set to: " + environmentName);
        }
#endif
    }
    
    void AdvancedAudioEngine::removeAudioEnvironment(const std::string& environmentName) {
#ifdef ENABLE_AUDIO
        audioEnvironments.erase(environmentName);
        if (activeEnvironment == environmentName) {
            activeEnvironment = "";
        }
#endif
    }
    
    bool AdvancedAudioEngine::createAdvancedAudioEffect(AudioEffectType type, const std::string& name, const std::unordered_map<std::string, float>& parameters) {
#ifdef ENABLE_AUDIO
        if (alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
            // In a full implementation, we would create the effect with the given parameters
            // For now, we'll just log that the method exists
            SPARKY_LOG_DEBUG("Creating advanced audio effect: " + name);
            return true;
        }
        return false;
#else
        return false;
#endif
    }
    
    void AdvancedAudioEngine::applyAdvancedAudioEffect(ALuint source, const std::string& effectName) {
#ifdef ENABLE_AUDIO
        if (source != 0 && alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
            // In a full implementation, we would apply the effect to the source
            // For now, we'll just log that the method exists
            SPARKY_LOG_DEBUG("Applying advanced audio effect: " + effectName + " to source");
        }
#endif
    }
    
    void AdvancedAudioEngine::removeAdvancedAudioEffect(ALuint source, const std::string& effectName) {
#ifdef ENABLE_AUDIO
        if (source != 0 && alcIsExtensionPresent(device, "ALC_EXT_EFX")) {
            // In a full implementation, we would remove the effect from the source
            // For now, we'll just log that the method exists
            SPARKY_LOG_DEBUG("Removing advanced audio effect: " + effectName + " from source");
        }
#endif
    }
    
    void AdvancedAudioEngine::playAdvancedGunshotSound(const glm::vec3& position, float distanceAttenuation, float occlusion) {
#ifdef ENABLE_AUDIO
        // Load the gunshot sound if not already loaded
        if (soundBuffers.find("advanced_gunshot") == soundBuffers.end()) {
            loadSound("advanced_gunshot", "gunshot.wav");
        }
        
        // Play the sound at the specified position
        ALuint source = playSound("advanced_gunshot");
        setSoundPosition(source, position);
        
        // Apply distance attenuation
        setSoundVolume(source, 0.7f * distanceAttenuation);
        
        // Apply occlusion effect
        if (occlusion > 0.0f) {
            // In a full implementation, we would apply a low-pass filter
            // For now, we'll just reduce the volume
            float occludedVolume = 0.7f * distanceAttenuation * (1.0f - occlusion * 0.5f);
            setSoundVolume(source, occludedVolume);
        }
#else
        // Dummy implementation
#endif
    }
    
    void AdvancedAudioEngine::playAdvancedExplosionSound(const glm::vec3& position, float distanceAttenuation, float occlusion) {
#ifdef ENABLE_AUDIO
        // Load the explosion sound if not already loaded
        if (soundBuffers.find("advanced_explosion") == soundBuffers.end()) {
            loadSound("advanced_explosion", "explosion.wav");
        }
        
        // Play the sound at the specified position
        ALuint source = playSound("advanced_explosion");
        setSoundPosition(source, position);
        
        // Apply distance attenuation
        setSoundVolume(source, 1.0f * distanceAttenuation);
        
        // Apply occlusion effect
        if (occlusion > 0.0f) {
            // In a full implementation, we would apply a low-pass filter
            // For now, we'll just reduce the volume
            float occludedVolume = 1.0f * distanceAttenuation * (1.0f - occlusion * 0.5f);
            setSoundVolume(source, occludedVolume);
        }
#else
        // Dummy implementation
#endif
    }
    
    void AdvancedAudioEngine::playAdvancedFootstepSound(const glm::vec3& position, float distanceAttenuation, float occlusion) {
#ifdef ENABLE_AUDIO
        // Load the footstep sound if not already loaded
        if (soundBuffers.find("advanced_footstep") == soundBuffers.end()) {
            loadSound("advanced_footstep", "footstep.wav");
        }
        
        // Play the sound at the specified position
        ALuint source = playSound("advanced_footstep");
        setSoundPosition(source, position);
        
        // Apply distance attenuation
        setSoundVolume(source, 0.3f * distanceAttenuation);
        
        // Apply occlusion effect
        if (occlusion > 0.0f) {
            // In a full implementation, we would apply a low-pass filter
            // For now, we'll just reduce the volume
            float occludedVolume = 0.3f * distanceAttenuation * (1.0f - occlusion * 0.5f);
            setSoundVolume(source, occludedVolume);
        }
#else
        // Dummy implementation
#endif
    }
    
    float AdvancedAudioEngine::calculateAdvancedDistanceAttenuation(const glm::vec3& sourcePos, const glm::vec3& listenerPos, 
                                                                 const AdvancedAudioSourceProperties& properties) {
        float distance = glm::distance(sourcePos, listenerPos);
        
        // Clamp distance to max distance
        distance = std::min(distance, properties.maxDistance);
        
        // Calculate attenuation using inverse distance model
        float attenuation = properties.minDistance / (properties.minDistance + properties.rolloffFactor * (distance - properties.minDistance));
        
        // Apply air absorption
        attenuation *= calculateAirAbsorptionEffect(distance, properties.airAbsorptionFactor);
        
        // Clamp attenuation between 0 and 1
        return std::max(0.0f, std::min(1.0f, attenuation));
    }
    
    float AdvancedAudioEngine::calculateOcclusionEffect(float occlusionFactor) {
        // Simple occlusion model - reduces high frequencies more than low frequencies
        return 1.0f - occlusionFactor * 0.5f;
    }
    
    float AdvancedAudioEngine::calculateAirAbsorptionEffect(float distance, float absorptionFactor) {
        // Simple air absorption model - reduces volume over distance
        // absorptionFactor should be between 0.0 (no absorption) and 1.0 (full absorption)
        return std::exp(-absorptionFactor * distance * 0.001f); // Scale distance for realistic effect
    }
}