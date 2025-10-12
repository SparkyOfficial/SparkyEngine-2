/*
 * AdvancedAudioEngine.h
 * 
 * Author: Андрій Будильников
 * 
 * Advanced audio engine with enhanced 3D audio features
 */

#pragma once

#include "AudioEngine.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#ifdef ENABLE_AUDIO
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#endif

namespace Sparky {
    
    // Advanced audio source properties for realistic 3D positioning
    struct AdvancedAudioSourceProperties {
        float minDistance = 1.0f;
        float maxDistance = 100.0f;
        float rolloffFactor = 1.0f;
        float coneInnerAngle = 360.0f;
        float coneOuterAngle = 360.0f;
        float coneOuterGain = 0.0f;
        bool enableDoppler = true;
        float dopplerFactor = 1.0f;
        float airAbsorptionFactor = 0.0f;  // Air absorption (0.0 to 10.0)
        float roomRolloffFactor = 0.0f;    // Room rolloff factor (0.0 to 10.0)
        float coneInnerGain = 1.0f;        // Inner cone gain (0.0 to 1.0)
        bool enableOcclusion = false;      // Enable sound occlusion
        float occlusionFactor = 0.0f;      // Occlusion factor (0.0 to 1.0)
        bool enableObstruction = false;    // Enable sound obstruction
        float obstructionFactor = 0.0f;    // Obstruction factor (0.0 to 1.0)
    };
    
    // Audio environment for reverb and other effects
    struct AudioEnvironment {
        std::string name;
        float density = 1.0f;         // Environment density (0.0 to 1.0)
        float diffusion = 1.0f;       // Environment diffusion (0.0 to 1.0)
        float gain = 0.32f;           // Overall gain (0.0 to 1.0)
        float gainHF = 0.89f;         // High-frequency gain (0.0 to 1.0)
        float decayTime = 1.49f;      // Decay time (0.1 to 20.0 seconds)
        float decayHFRatio = 0.83f;   // High-frequency decay ratio (0.1 to 2.0)
        float reflectionsGain = 0.05f; // Early reflections gain (0.0 to 3.16)
        float reflectionsDelay = 0.007f; // Early reflections delay (0.0 to 0.3 seconds)
        float lateReverbGain = 1.26f; // Late reverb gain (0.0 to 10.0)
        float lateReverbDelay = 0.011f; // Late reverb delay (0.0 to 0.1 seconds)
        float airAbsorptionGainHF = 0.994f; // Air absorption gain HF (0.892 to 1.0)
        float roomRolloffFactor = 0.0f; // Room rolloff factor (0.0 to 10.0)
        bool decayHFLimit = true;     // Decay HF limit (AL_TRUE or AL_FALSE)
    };
    
    // Audio occlusion/obstruction data
    struct AudioOcclusionData {
        float directOcclusion = 0.0f;     // Direct path occlusion (0.0 to 1.0)
        float reverbOcclusion = 0.0f;     // Reverb path occlusion (0.0 to 1.0)
        float directObstruction = 0.0f;   // Direct path obstruction (0.0 to 1.0)
        float reverbObstruction = 0.0f;   // Reverb path obstruction (0.0 to 1.0)
    };
    
    // Advanced audio listener properties
    struct AdvancedAudioListenerProperties {
        float metersPerUnit = 1.0f;       // Scale factor for distance calculations
        float speedOfSound = 343.3f;      // Speed of sound in units per second
        float dopplerFactor = 1.0f;       // Global Doppler factor
        glm::vec3 position = glm::vec3(0.0f);
        glm::vec3 velocity = glm::vec3(0.0f);
        glm::vec3 orientationForward = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 orientationUp = glm::vec3(0.0f, 1.0f, 0.0f);
    };
    
    class AdvancedAudioEngine : public AudioEngine {
    public:
        static AdvancedAudioEngine& getInstance();
        
        bool initialize() override;
        void cleanup() override;
        
        // Advanced listener controls
        void setAdvancedListenerProperties(const AdvancedAudioListenerProperties& properties);
        const AdvancedAudioListenerProperties& getAdvancedListenerProperties() const;
        
        // Advanced sound management
#ifdef ENABLE_AUDIO
        bool loadAdvancedSound(const std::string& name, const std::string& filepath, bool streaming = false);
        ALuint playAdvancedSound(const std::string& name, bool loop = false, const glm::vec3& position = glm::vec3(0.0f));
        void stopAdvancedSound(ALuint source);
#else
        bool loadAdvancedSound(const std::string& name, const std::string& filepath, bool streaming = false) { return false; }
        unsigned int playAdvancedSound(const std::string& name, bool loop = false, const glm::vec3& position = glm::vec3(0.0f)) { return 0; }
        void stopAdvancedSound(unsigned int source) {}
#endif
        
        // Advanced source controls
#ifdef ENABLE_AUDIO
        void setAdvancedSoundProperties(ALuint source, const AdvancedAudioSourceProperties& properties);
        void setSoundOcclusion(ALuint source, const AudioOcclusionData& occlusionData);
        void setSoundEnvironment(ALuint source, const std::string& environmentName);
        void setSoundSpatialization(ALuint source, bool enableHRTF = true);
#else
        void setAdvancedSoundProperties(unsigned int source, const AdvancedAudioSourceProperties& properties) {}
        void setSoundOcclusion(unsigned int source, const AudioOcclusionData& occlusionData) {}
        void setSoundEnvironment(unsigned int source, const std::string& environmentName) {}
        void setSoundSpatialization(unsigned int source, bool enableHRTF = true) {}
#endif
        
        // Environment management
#ifdef ENABLE_AUDIO
        bool createAudioEnvironment(const AudioEnvironment& environment);
        void setActiveEnvironment(const std::string& environmentName);
        void removeAudioEnvironment(const std::string& environmentName);
#else
        bool createAudioEnvironment(const AudioEnvironment& environment) { return false; }
        void setActiveEnvironment(const std::string& environmentName) {}
        void removeAudioEnvironment(const std::string& environmentName) {}
#endif
        
        // Audio effects
#ifdef ENABLE_AUDIO
        bool createAdvancedAudioEffect(AudioEffectType type, const std::string& name, const std::unordered_map<std::string, float>& parameters);
        void applyAdvancedAudioEffect(ALuint source, const std::string& effectName);
        void removeAdvancedAudioEffect(ALuint source, const std::string& effectName);
#else
        bool createAdvancedAudioEffect(AudioEffectType type, const std::string& name, const std::unordered_map<std::string, float>& parameters) { return false; }
        void applyAdvancedAudioEffect(unsigned int source, const std::string& effectName) {}
        void removeAdvancedAudioEffect(unsigned int source, const std::string& effectName) {}
#endif
        
        // Preset sounds with advanced features
        void playAdvancedGunshotSound(const glm::vec3& position, float distanceAttenuation = 1.0f, float occlusion = 0.0f);
        void playAdvancedExplosionSound(const glm::vec3& position, float distanceAttenuation = 1.0f, float occlusion = 0.0f);
        void playAdvancedFootstepSound(const glm::vec3& position, float distanceAttenuation = 1.0f, float occlusion = 0.0f);
        
        // Utility functions
        float calculateAdvancedDistanceAttenuation(const glm::vec3& sourcePos, const glm::vec3& listenerPos, 
                                                 const AdvancedAudioSourceProperties& properties);
        float calculateOcclusionEffect(float occlusionFactor);
        float calculateAirAbsorptionEffect(float distance, float absorptionFactor);

    private:
        AdvancedAudioEngine();
        ~AdvancedAudioEngine();
        
#ifdef ENABLE_AUDIO
        // Effect slots and filters
        std::unordered_map<std::string, ALuint> effectSlots;
        std::unordered_map<std::string, ALuint> filters;
        
        // Environments
        std::unordered_map<std::string, AudioEnvironment> audioEnvironments;
        std::string activeEnvironment;
        
        // Advanced listener properties
        AdvancedAudioListenerProperties advancedListenerProperties;
#else
        // Dummy variables when audio is disabled
        std::unordered_map<std::string, unsigned int> effectSlots;
        std::unordered_map<std::string, unsigned int> filters;
        
        // Environments
        std::unordered_map<std::string, AudioEnvironment> audioEnvironments;
        std::string activeEnvironment;
        
        // Advanced listener properties
        AdvancedAudioListenerProperties advancedListenerProperties;
#endif
    };
}