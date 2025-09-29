#include "../include/ParticleSystem.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

int main() {
    SPARKY_LOG_INFO("Starting Particle System Test");
    
    // Create a game object
    auto object = std::make_unique<Sparky::GameObject>("ParticleEmitter");
    object->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Create particle system
    auto particleSystem = object->addComponent<Sparky::ParticleSystem>();
    particleSystem->setEmissionRate(50.0f);
    particleSystem->setParticleLifetime(1.0f, 3.0f);
    particleSystem->setStartColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
    particleSystem->setEndColor(glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));   // Yellow to transparent
    particleSystem->setStartSize(0.1f);
    particleSystem->setEndSize(0.0f);
    particleSystem->setStartSpeed(2.0f, 5.0f);
    particleSystem->setGravity(glm::vec3(0.0f, -9.81f, 0.0f));
    particleSystem->setEmissionShapeSphere(0.1f);
    particleSystem->play();
    
    // Simulate for 5 seconds
    float deltaTime = 1.0f / 60.0f; // 60 FPS
    for (int i = 0; i < 300; i++) { // 300 frames = 5 seconds
        particleSystem->update(deltaTime);
        
        // Print active particle count every 60 frames (1 second)
        if (i % 60 == 0) {
            SPARKY_LOG_INFO("Frame " + std::to_string(i) + 
                          " - Active particles: " + std::to_string(particleSystem->getActiveParticleCount()));
        }
    }
    
    SPARKY_LOG_INFO("Particle System Test Completed");
    return 0;
}