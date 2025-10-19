/*
 * advanced_particle_demo.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple demo to demonstrate the advanced particle system
 */

#include "../include/SparkyEngine.h"
#include "../include/AdvancedParticleSystem.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace Sparky;

class AdvancedParticleDemo {
private:
    std::unique_ptr<AdvancedParticleSystem> particleSystem;

public:
    AdvancedParticleDemo() {
        particleSystem = std::make_unique<AdvancedParticleSystem>();
    }

    ~AdvancedParticleDemo() {
    }

    bool initialize() {
        SPARKY_LOG_INFO("Initializing Advanced Particle Demo...");

        // Configure the particle system
        particleSystem->setMaxParticles(2000);
        particleSystem->setDuration(5.0f);
        particleSystem->setLooping(true);
        particleSystem->setPlayOnAwake(true);
        
        // Add an emitter
        auto emitter = std::make_unique<ParticleEmitter>(EmitterType::SPHERE);
        emitter->setEmissionRate(100.0f);
        emitter->setMinLifetime(0.5f);
        emitter->setMaxLifetime(2.0f);
        emitter->setMinSpeed(5.0f);
        emitter->setMaxSpeed(20.0f);
        emitter->setMinSize(glm::vec2(0.1f, 0.1f));
        emitter->setMaxSize(glm::vec2(0.3f, 0.3f));
        emitter->setMinColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
        emitter->setMaxColor(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f));   // Dark red fading to transparent
        particleSystem->addEmitter(std::move(emitter));
        
        // Add some forces
        auto gravityForce = std::make_unique<GravityForce>(glm::vec3(0.0f, -5.0f, 0.0f));
        particleSystem->addForce(std::move(gravityForce));
        
        // Add some modifiers
        auto sizeModifier = std::make_unique<SizeOverLifetimeModifier>();
        sizeModifier->setSizeRange(glm::vec2(0.2f, 0.2f), glm::vec2(0.05f, 0.05f));
        particleSystem->addModifier(std::move(sizeModifier));
        
        // Set rendering properties
        particleSystem->setBlendMode(ParticleBlendMode::ADDITIVE);
        particleSystem->setUseWorldSpace(true);

        SPARKY_LOG_INFO("Advanced Particle Demo initialized successfully!");
        return true;
    }

    void update(float deltaTime) {
        // Update the particle system
        particleSystem->update(deltaTime);
        
        // Print particle count
        size_t particleCount = particleSystem->getActiveParticleCount();
        SPARKY_LOG_INFO("Active particles: " + std::to_string(particleCount));
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Advanced Particle Demo!");
            return;
        }

        // Play the particle system
        particleSystem->play();
        
        // Emit some particles
        particleSystem->emit(100);

        // Simulate for 5 seconds
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting advanced particle simulation...");
        
        while (totalTime < 5.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Every second, emit some additional particles
            if (static_cast<int>(totalTime) > static_cast<int>(totalTime - deltaTime)) {
                static int effectCounter = 0;
                effectCounter++;
                
                particleSystem->emit(50);
                SPARKY_LOG_INFO("Emitting additional particles");
            }
        }
        
        SPARKY_LOG_INFO("Advanced particle simulation completed!");
    }
};

int main() {
    try {
        AdvancedParticleDemo demo;
        demo.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}