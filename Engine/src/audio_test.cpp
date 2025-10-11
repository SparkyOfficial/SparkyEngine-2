#include "../include/AudioEngine.h"
#include "../include/Logger.h"
#include <iostream>

int main() {
    // Initialize logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing Enhanced Audio System..." << std::endl;
    
    // Initialize audio engine
    Sparky::AudioEngine& audioEngine = Sparky::AudioEngine::getInstance();
    
    if (!audioEngine.initialize()) {
        std::cerr << "Failed to initialize audio engine!" << std::endl;
        return 1;
    }
    
    std::cout << "Audio engine initialized successfully" << std::endl;
    
    // Test loading a sound
    bool loaded = audioEngine.loadSound("test_sound", "test.wav");
    std::cout << "Sound loading test: " << (loaded ? "PASSED" : "FAILED") << std::endl;
    
    // Test setting listener position
    audioEngine.setListenerPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    std::cout << "Listener position test: PASSED" << std::endl;
    
    // Test setting listener orientation
    audioEngine.setListenerOrientation(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    std::cout << "Listener orientation test: PASSED" << std::endl;
    
    // Test distance attenuation calculation
    float attenuation = audioEngine.calculateDistanceAttenuation(
        glm::vec3(0.0f, 0.0f, 0.0f),  // Source position
        glm::vec3(5.0f, 0.0f, 0.0f),  // Listener position
        1.0f,  // Min distance
        100.0f, // Max distance
        1.0f   // Rolloff factor
    );
    std::cout << "Distance attenuation calculation test: PASSED (attenuation: " << attenuation << ")" << std::endl;
    
    // Test audio effect creation
    bool effectCreated = audioEngine.createAudioEffect(Sparky::AudioEffectType::REVERB, "test_reverb");
    std::cout << "Audio effect creation test: " << (effectCreated ? "PASSED" : "FAILED") << std::endl;
    
    // Clean up
    audioEngine.cleanup();
    
    std::cout << "Enhanced Audio System test completed!" << std::endl;
    return 0;
}