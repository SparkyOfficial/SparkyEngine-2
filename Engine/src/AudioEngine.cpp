#ifdef ENABLE_AUDIO

#include "../include/AudioEngine.h"
#include "../include/Logger.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

namespace Sparky {
    
    AudioEngine& AudioEngine::getInstance() {
        static AudioEngine instance;
        return instance;
    }
    
    AudioEngine::AudioEngine() : device(nullptr), context(nullptr), listenerPosition(0.0f), listenerOrientation(0.0f), listenerVelocity(0.0f) {
        SPARKY_LOG_DEBUG("AudioEngine created");
    }
    
    AudioEngine::~AudioEngine() {
        cleanup();
        SPARKY_LOG_DEBUG("AudioEngine destroyed");
    }
    
    bool AudioEngine::initialize() {
        SPARKY_LOG_INFO("Initializing AudioEngine");
        
        try {
            // Open default device
            device = alcOpenDevice(nullptr);
            if (!device) {
                SPARKY_LOG_ERROR("Failed to open OpenAL device");
                return false;
            }
            
            // Create context
            context = alcCreateContext(device, nullptr);
            if (!context) {
                SPARKY_LOG_ERROR("Failed to create OpenAL context");
                return false;
            }
            
            // Make context current
            if (!alcMakeContextCurrent(context)) {
                SPARKY_LOG_ERROR("Failed to make OpenAL context current");
                return false;
            }
            
            // Check for errors
            ALenum error = alGetError();
            if (error != AL_NO_ERROR) {
                SPARKY_LOG_ERROR("OpenAL error during initialization: " + std::to_string(error));
                return false;
            }
            
            // Set up listener
            alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
            alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
            
            // Set up orientation (forward and up vectors)
            ALfloat orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
            alListenerfv(AL_ORIENTATION, orientation);
            
            SPARKY_LOG_INFO("AudioEngine initialized successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Exception during AudioEngine initialization: " + std::string(e.what()));
            return false;
        }
    }
    
    void AudioEngine::cleanup() {
        SPARKY_LOG_INFO("Cleaning up AudioEngine");
        
        // Clean up all sound buffers
        for (auto& pair : soundBuffers) {
            alDeleteBuffers(1, &pair.second);
        }
        soundBuffers.clear();
        
        // Clean up all sound sources
        for (auto source : soundSources) {
            alDeleteSources(1, &source);
        }
        soundSources.clear();
        
        // Clean up OpenAL context and device
        if (context) {
            alcMakeContextCurrent(nullptr);
            alcDestroyContext(context);
            context = nullptr;
        }
        
        if (device) {
            alcCloseDevice(device);
            device = nullptr;
        }
        
        SPARKY_LOG_INFO("AudioEngine cleaned up");
    }
    
    bool AudioEngine::loadSound(const std::string& name, const std::string& filepath) {
        SPARKY_LOG_DEBUG("Loading sound: " + name + " from " + filepath);
        
        // Check if sound is already loaded
        if (soundBuffers.find(name) != soundBuffers.end()) {
            SPARKY_LOG_WARNING("Sound already loaded: " + name);
            return true;
        }
        
        // For now, we'll create a simple beep sound as a placeholder
        // In a real implementation, we would load WAV/OGG files
        std::vector<short> data(44100); // 1 second of audio at 44.1kHz
        for (int i = 0; i < 44100; ++i) {
            // Generate a simple sine wave
            data[i] = static_cast<short>(32767.0 * sin(2.0 * 3.14159 * 440.0 * i / 44100.0));
        }
        
        // Generate buffer
        ALuint buffer;
        alGenBuffers(1, &buffer);
        
        // Load data into buffer
        alBufferData(buffer, AL_FORMAT_MONO16, data.data(), data.size() * sizeof(short), 44100);
        
        // Check for errors
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            SPARKY_LOG_ERROR("Failed to load sound " + name + ": OpenAL error " + std::to_string(error));
            alDeleteBuffers(1, &buffer);
            return false;
        }
        
        // Store buffer
        soundBuffers[name] = buffer;
        SPARKY_LOG_DEBUG("Sound loaded successfully: " + name);
        return true;
    }
    
    ALuint AudioEngine::playSound(const std::string& name, bool loop) {
        // Find the sound buffer
        auto it = soundBuffers.find(name);
        if (it == soundBuffers.end()) {
            SPARKY_LOG_WARNING("Sound not found: " + name);
            return 0;
        }
        
        // Generate a source
        ALuint source;
        alGenSources(1, &source);
        
        // Attach buffer to source
        alSourcei(source, AL_BUFFER, it->second);
        
        // Set looping
        alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        
        // Set 3D position to listener position by default
        alSource3f(source, AL_POSITION, listenerPosition.x, listenerPosition.y, listenerPosition.z);
        alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        
        // Play the sound
        alSourcePlay(source);
        
        // Store the source for cleanup
        soundSources.push_back(source);
        
        // Check for errors
        ALenum error = alGetError();
        if (error != AL_NO_ERROR) {
            SPARKY_LOG_ERROR("Failed to play sound " + name + ": OpenAL error " + std::to_string(error));
            alDeleteSources(1, &source);
            return 0;
        }
        
        SPARKY_LOG_DEBUG("Playing sound: " + name);
        return source;
    }
    
    void AudioEngine::stopSound(ALuint source) {
        if (source != 0) {
            alSourceStop(source);
            
            // Remove from our list of sources
            auto it = std::find(soundSources.begin(), soundSources.end(), source);
            if (it != soundSources.end()) {
                soundSources.erase(it);
            }
            
            // Delete the source
            alDeleteSources(1, &source);
        }
    }
    
    void AudioEngine::setListenerPosition(const glm::vec3& position) {
        listenerPosition = position;
        alListener3f(AL_POSITION, position.x, position.y, position.z);
    }
    
    void AudioEngine::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
        listenerOrientation = forward;
        ALfloat orientation[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
        alListenerfv(AL_ORIENTATION, orientation);
    }
    
    void AudioEngine::setListenerVelocity(const glm::vec3& velocity) {
        listenerVelocity = velocity;
        alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }
    
    void AudioEngine::setSoundPosition(ALuint source, const glm::vec3& position) {
        if (source != 0) {
            alSource3f(source, AL_POSITION, position.x, position.y, position.z);
        }
    }
    
    void AudioEngine::setSoundVelocity(ALuint source, const glm::vec3& velocity) {
        if (source != 0) {
            alSource3f(source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
        }
    }
    
    void AudioEngine::setSoundVolume(ALuint source, float volume) {
        if (source != 0) {
            alSourcef(source, AL_GAIN, volume);
        }
    }
    
    void AudioEngine::setSoundPitch(ALuint source, float pitch) {
        if (source != 0) {
            alSourcef(source, AL_PITCH, pitch);
        }
    }
    
    void AudioEngine::setSoundProperties(ALuint source, const AudioSourceProperties& properties) {
        if (source != 0) {
            // Set distance model parameters
            alSourcef(source, AL_REFERENCE_DISTANCE, properties.minDistance);
            alSourcef(source, AL_MAX_DISTANCE, properties.maxDistance);
            alSourcef(source, AL_ROLLOFF_FACTOR, properties.rolloffFactor);
            
            // Set cone parameters
            alSourcef(source, AL_CONE_INNER_ANGLE, properties.coneInnerAngle);
            alSourcef(source, AL_CONE_OUTER_ANGLE, properties.coneOuterAngle);
            alSourcef(source, AL_CONE_OUTER_GAIN, properties.coneOuterGain);
            
            // Set Doppler effect
            if (properties.enableDoppler) {
                alSourcef(source, AL_DOPPLER_FACTOR, properties.dopplerFactor);
            } else {
                alSourcef(source, AL_DOPPLER_FACTOR, 0.0f);
            }
        }
    }
    
    void AudioEngine::setSoundDistanceModel(ALuint source, int model) {
        if (source != 0) {
            alSourcei(source, AL_DISTANCE_MODEL, model);
        }
    }
    
    void AudioEngine::setSoundAttenuation(ALuint source, float minDistance, float maxDistance, float rolloffFactor) {
        if (source != 0) {
            alSourcef(source, AL_REFERENCE_DISTANCE, minDistance);
            alSourcef(source, AL_MAX_DISTANCE, maxDistance);
            alSourcef(source, AL_ROLLOFF_FACTOR, rolloffFactor);
        }
    }
    
    void AudioEngine::setSoundCone(ALuint source, float innerAngle, float outerAngle, float outerGain) {
        if (source != 0) {
            alSourcef(source, AL_CONE_INNER_ANGLE, innerAngle);
            alSourcef(source, AL_CONE_OUTER_ANGLE, outerAngle);
            alSourcef(source, AL_CONE_OUTER_GAIN, outerGain);
        }
    }
    
    void AudioEngine::setSoundDoppler(ALuint source, bool enable, float factor) {
        if (source != 0) {
            alSourcef(source, AL_DOPPLER_FACTOR, enable ? factor : 0.0f);
        }
    }
    
    bool AudioEngine::isSoundPlaying(ALuint source) {
        if (source == 0) return false;
        
        ALint state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }
    
    // Preset sounds for common game events
    void AudioEngine::playGunshotSound(const glm::vec3& position) {
        // Load the gunshot sound if not already loaded
        if (soundBuffers.find("gunshot") == soundBuffers.end()) {
            loadSound("gunshot", "gunshot.wav");
        }
        
        // Play the sound at the specified position
        ALuint source = playSound("gunshot");
        setSoundPosition(source, position);
        setSoundVolume(source, 0.7f); // Moderate volume
    }
    
    void AudioEngine::playExplosionSound(const glm::vec3& position) {
        // Load the explosion sound if not already loaded
        if (soundBuffers.find("explosion") == soundBuffers.end()) {
            loadSound("explosion", "explosion.wav");
        }
        
        // Play the sound at the specified position
        ALuint source = playSound("explosion");
        setSoundPosition(source, position);
        setSoundVolume(source, 1.0f); // Full volume
    }
    
    void AudioEngine::playFootstepSound(const glm::vec3& position) {
        // Load the footstep sound if not already loaded
        if (soundBuffers.find("footstep") == soundBuffers.end()) {
            loadSound("footstep", "footstep.wav");
        }
        
        // Play the sound at the specified position
        ALuint source = playSound("footstep");
        setSoundPosition(source, position);
        setSoundVolume(source, 0.3f); // Low volume
    }
    
    void AudioEngine::playBackgroundMusic(const std::string& filepath) {
        // Load the music if not already loaded
        if (soundBuffers.find("music") == soundBuffers.end()) {
            loadSound("music", filepath);
        }
        
        // Play the music in a loop
        ALuint source = playSound("music", true);
        setSoundVolume(source, 0.5f); // Medium volume
    }
    
    bool AudioEngine::createAudioEffect(AudioEffectType type, const std::string& name) {
        // In a full implementation, we would create OpenAL effects
        // For now, we'll just log that the method exists
        SPARKY_LOG_DEBUG("Creating audio effect: " + name);
        return true;
    }
    
    void AudioEngine::applyAudioEffect(ALuint source, const std::string& effectName) {
        // In a full implementation, we would apply OpenAL effects to the source
        // For now, we'll just log that the method exists
        SPARKY_LOG_DEBUG("Applying audio effect: " + effectName + " to source");
    }
    
    void AudioEngine::removeAudioEffect(ALuint source, const std::string& effectName) {
        // In a full implementation, we would remove OpenAL effects from the source
        // For now, we'll just log that the method exists
        SPARKY_LOG_DEBUG("Removing audio effect: " + effectName + " from source");
    }
    
    float AudioEngine::calculateDistanceAttenuation(const glm::vec3& sourcePos, const glm::vec3& listenerPos, 
                                                float minDistance, float maxDistance, float rolloffFactor) {
        float distance = glm::distance(sourcePos, listenerPos);
        
        // Clamp distance to max distance
        distance = std::min(distance, maxDistance);
        
        // Calculate attenuation using inverse distance model
        float attenuation = minDistance / (minDistance + rolloffFactor * (distance - minDistance));
        
        // Clamp attenuation between 0 and 1
        return std::max(0.0f, std::min(1.0f, attenuation));
    }
}

#else // ENABLE_AUDIO

// Fallback implementations when audio is not available
#include "../include/AudioEngine.h"

namespace Sparky {
    
    AudioEngine& AudioEngine::getInstance() {
        static AudioEngine instance;
        return instance;
    }
    
    AudioEngine::AudioEngine() : device(nullptr), context(nullptr), listenerPosition(0.0f), listenerOrientation(0.0f), listenerVelocity(0.0f) {}
    AudioEngine::~AudioEngine() {}
    
    bool AudioEngine::initialize() { return false; }
    void AudioEngine::cleanup() {}
    
    bool AudioEngine::loadSound(const std::string& name, const std::string& filepath) { return false; }
    unsigned int AudioEngine::playSound(const std::string& name, bool loop) { return 0; }
    void AudioEngine::stopSound(unsigned int source) {}
    
    void AudioEngine::setListenerPosition(const glm::vec3& position) {}
    void AudioEngine::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {}
    void AudioEngine::setListenerVelocity(const glm::vec3& velocity) {}
    
    void AudioEngine::setSoundPosition(unsigned int source, const glm::vec3& position) {}
    void AudioEngine::setSoundVelocity(unsigned int source, const glm::vec3& velocity) {}
    void AudioEngine::setSoundVolume(unsigned int source, float volume) {}
    void AudioEngine::setSoundPitch(unsigned int source, float pitch) {}
    bool AudioEngine::isSoundPlaying(unsigned int source) { return false; }
    
    void AudioEngine::setSoundProperties(unsigned int source, const AudioSourceProperties& properties) {}
    void AudioEngine::setSoundDistanceModel(unsigned int source, int model) {}
    void AudioEngine::setSoundAttenuation(unsigned int source, float minDistance, float maxDistance, float rolloffFactor) {}
    void AudioEngine::setSoundCone(unsigned int source, float innerAngle, float outerAngle, float outerGain) {}
    void AudioEngine::setSoundDoppler(unsigned int source, bool enable, float factor) {}
    
    bool AudioEngine::createAudioEffect(AudioEffectType type, const std::string& name) { return false; }
    void AudioEngine::applyAudioEffect(unsigned int source, const std::string& effectName) {}
    void AudioEngine::removeAudioEffect(unsigned int source, const std::string& effectName) {}
    
    void AudioEngine::playGunshotSound(const glm::vec3& position) {}
    void AudioEngine::playExplosionSound(const glm::vec3& position) {}
    void AudioEngine::playFootstepSound(const glm::vec3& position) {}
    void AudioEngine::playBackgroundMusic(const std::string& filepath) {}
    
    float AudioEngine::calculateDistanceAttenuation(const glm::vec3& sourcePos, const glm::vec3& listenerPos, 
                                                float minDistance, float maxDistance, float rolloffFactor) {
        return 0.0f;
    }
}

#endif // ENABLE_AUDIO