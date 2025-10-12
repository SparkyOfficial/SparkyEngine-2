/*
 * advanced_audio_demo.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple demo to demonstrate the advanced audio engine
 */

#include "../include/SparkyEngine.h"
#include "../include/AdvancedAudioEngine.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

using namespace Sparky;

class AdvancedAudioDemo {
private:
    AdvancedAudioEngine& audioEngine;

public:
    AdvancedAudioDemo() : audioEngine(AdvancedAudioEngine::getInstance()) {
    }

    ~AdvancedAudioDemo() {
    }

    bool initialize() {
        SPARKY_LOG_INFO("Initializing Advanced Audio Demo...");

        // Initialize the audio engine
        if (!audioEngine.initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize AdvancedAudioEngine");
            return false;
        }

        // Set up advanced listener properties
        AdvancedAudioListenerProperties listenerProps;
        listenerProps.position = glm::vec3(0.0f, 0.0f, 0.0f);
        listenerProps.orientationForward = glm::vec3(0.0f, 0.0f, -1.0f);
        listenerProps.orientationUp = glm::vec3(0.0f, 1.0f, 0.0f);
        listenerProps.speedOfSound = 343.3f; // Standard speed of sound
        listenerProps.dopplerFactor = 1.0f;
        audioEngine.setAdvancedListenerProperties(listenerProps);

        // Create audio environments
        AudioEnvironment caveEnv;
        caveEnv.name = "cave";
        caveEnv.density = 0.8f;
        caveEnv.diffusion = 0.6f;
        caveEnv.gain = 0.5f;
        caveEnv.gainHF = 0.2f;
        caveEnv.decayTime = 3.0f;
        caveEnv.decayHFRatio = 0.1f;
        caveEnv.reflectionsGain = 0.1f;
        caveEnv.lateReverbGain = 1.5f;
        audioEngine.createAudioEnvironment(caveEnv);

        AudioEnvironment outdoorEnv;
        outdoorEnv.name = "outdoor";
        outdoorEnv.density = 0.2f;
        outdoorEnv.diffusion = 0.9f;
        outdoorEnv.gain = 0.8f;
        outdoorEnv.gainHF = 0.9f;
        outdoorEnv.decayTime = 0.5f;
        outdoorEnv.decayHFRatio = 0.9f;
        outdoorEnv.reflectionsGain = 0.05f;
        outdoorEnv.lateReverbGain = 0.5f;
        audioEngine.createAudioEnvironment(outdoorEnv);

        // Set active environment
        audioEngine.setActiveEnvironment("outdoor");

        // Load some sounds
        audioEngine.loadSound("gunshot", "sounds/gunshot.wav");
        audioEngine.loadSound("explosion", "sounds/explosion.wav");
        audioEngine.loadSound("footstep", "sounds/footstep.wav");

        SPARKY_LOG_INFO("Advanced Audio Demo initialized successfully!");
        return true;
    }

    void update(float deltaTime) {
        // In a real application, we would update the listener position based on camera
        // For this demo, we'll just log that the audio engine is running
        static float time = 0.0f;
        time += deltaTime;
        
        // Every 2 seconds, play a different sound
        if (static_cast<int>(time * 10) % 20 == 0) {
            static int soundIndex = 0;
            soundIndex = (soundIndex + 1) % 3;
            
            glm::vec3 soundPosition(
                sin(time) * 5.0f,
                0.0f,
                cos(time) * 5.0f
            );
            
            switch (soundIndex) {
                case 0:
                    audioEngine.playAdvancedGunshotSound(soundPosition, 1.0f, 0.0f);
                    SPARKY_LOG_INFO("Playing gunshot at position: (" + 
                                  std::to_string(soundPosition.x) + ", " + 
                                  std::to_string(soundPosition.y) + ", " + 
                                  std::to_string(soundPosition.z) + ")");
                    break;
                case 1:
                    audioEngine.playAdvancedExplosionSound(soundPosition, 1.0f, 0.0f);
                    SPARKY_LOG_INFO("Playing explosion at position: (" + 
                                  std::to_string(soundPosition.x) + ", " + 
                                  std::to_string(soundPosition.y) + ", " + 
                                  std::to_string(soundPosition.z) + ")");
                    break;
                case 2:
                    audioEngine.playAdvancedFootstepSound(soundPosition, 1.0f, 0.0f);
                    SPARKY_LOG_INFO("Playing footstep at position: (" + 
                                  std::to_string(soundPosition.x) + ", " + 
                                  std::to_string(soundPosition.y) + ", " + 
                                  std::to_string(soundPosition.z) + ")");
                    break;
            }
        }
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Advanced Audio Demo!");
            return;
        }

        // Simulate for 10 seconds
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting advanced audio simulation...");
        
        while (totalTime < 10.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Sleep to simulate frame time (in a real application, this would be handled by the engine)
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        // Cleanup
        audioEngine.cleanup();
        
        SPARKY_LOG_INFO("Advanced audio simulation completed!");
    }
};

int main() {
    try {
        AdvancedAudioDemo demo;
        demo.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}