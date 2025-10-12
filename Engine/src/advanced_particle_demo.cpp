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
        AdvancedEmitterProperties advancedProps;
        
        // Set up an advanced explosion effect
        advancedProps.position[0] = 0.0f; advancedProps.position[1] = 0.0f; advancedProps.position[2] = 0.0f;
        advancedProps.direction[0] = 0.0f; advancedProps.direction[1] = 1.0f; advancedProps.direction[2] = 0.0f;
        advancedProps.spread = 1.0f; // Full spherical spread
        advancedProps.minSpeed = 5.0f; advancedProps.maxSpeed = 20.0f;
        advancedProps.speed = 15.0f;
        advancedProps.speedVariation = 10.0f;
        advancedProps.emissionRate = 0.0f; // No continuous emission for burst effect
        advancedProps.minLifetime = 0.5f; advancedProps.maxLifetime = 2.0f;
        advancedProps.lifetime = 1.5f;
        advancedProps.lifetimeVariation = 0.5f;
        advancedProps.startColor[0] = 1.0f; advancedProps.startColor[1] = 0.5f; advancedProps.startColor[2] = 0.0f; advancedProps.startColor[3] = 1.0f; // Orange
        advancedProps.endColor[0] = 0.5f; advancedProps.endColor[1] = 0.0f; advancedProps.endColor[2] = 0.0f; advancedProps.endColor[3] = 0.0f;   // Dark red fading to transparent
        advancedProps.minStartSize = 0.1f; advancedProps.maxStartSize = 0.3f;
        advancedProps.startSize = 0.2f;
        advancedProps.minEndSize = 0.0f; advancedProps.maxEndSize = 0.1f;
        advancedProps.endSize = 0.05f;
        advancedProps.sizeVariation = 0.1f;
        advancedProps.gravity[0] = 0.0f; advancedProps.gravity[1] = -5.0f; advancedProps.gravity[2] = 0.0f; // Some gravity to pull particles down
        advancedProps.acceleration[0] = advancedProps.acceleration[1] = advancedProps.acceleration[2] = 0.0f;
        advancedProps.minRotationSpeed = 0.0f; advancedProps.maxRotationSpeed = 5.0f;
        advancedProps.rotationSpeed = 2.0f;
        advancedProps.rotationSpeedVariation = 2.0f;
        advancedProps.minMass = 0.5f; advancedProps.maxMass = 2.0f;
        advancedProps.mass = 1.0f;
        advancedProps.massVariation = 0.5f;
        advancedProps.type = ParticleType::EXPLOSION;
        advancedProps.emissionArea[0] = 5.0f; advancedProps.emissionArea[1] = 5.0f; advancedProps.emissionArea[2] = 5.0f;
        advancedProps.useVolumeEmission = true;
        advancedProps.windInfluence = 0.1f;
        advancedProps.turbulence = 0.2f;
        advancedProps.turbulenceScale = 0.5f;
        advancedProps.turbulenceSpeed = 2.0f;
        advancedProps.useTurbulence = true;
        advancedProps.airDensity = 1.225f; // Standard air density
        advancedProps.gravityScale = 1.0f;
        advancedProps.inheritEmitterVelocity = false;
        advancedProps.velocityInheritance = 0.0f;
        advancedProps.affectedByWind = true;
        
        particleSystem->setAdvancedEmitterProperties(advancedProps);
        
        // Add some force fields
        ForceField repulsionField;
        repulsionField.position = glm::vec3(2.0f, 2.0f, 2.0f);
        repulsionField.radius = 3.0f;
        repulsionField.force = glm::vec3(0.0f, 5.0f, 0.0f); // Upward force
        repulsionField.strength = 10.0f;
        repulsionField.falloff = 0.5f;
        repulsionField.enabled = true;
        particleSystem->addForceField(repulsionField);
        
        // Add some attractors
        ParticleAttractor attractor;
        attractor.position = glm::vec3(-2.0f, 1.0f, -2.0f);
        attractor.radius = 4.0f;
        attractor.strength = 5.0f;
        attractor.falloff = 0.3f;
        attractor.enabled = true;
        particleSystem->addAttractor(attractor);
        
        // Add wind zones
        WindZone windZone;
        windZone.position = glm::vec3(0.0f, 3.0f, 0.0f);
        windZone.direction = glm::vec3(1.0f, 0.0f, 0.0f); // Wind blowing in positive X direction
        windZone.radius = 5.0f;
        windZone.speed = 3.0f;
        windZone.turbulence = 0.5f;
        windZone.enabled = true;
        particleSystem->addWindZone(windZone);
        
        // Enable collision detection
        particleSystem->setCollisionEnabled(true);
        particleSystem->setCollisionBounds(glm::vec3(-10.0f, 0.0f, -10.0f), glm::vec3(10.0f, 20.0f, 10.0f));

        SPARKY_LOG_INFO("Advanced Particle Demo initialized successfully!");
        return true;
    }

    void update(float deltaTime) {
        // Update the particle system
        particleSystem->update(deltaTime);
        
        // Print particle count
        int particleCount = particleSystem->getParticleCount();
        SPARKY_LOG_INFO("Active particles: " + std::to_string(particleCount));
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Advanced Particle Demo!");
            return;
        }

        // Create an explosion effect
        particleSystem->createAdvancedExplosion(0.0f, 0.0f, 0.0f, 1.0f, 5.0f);
        
        // Create some smoke
        particleSystem->createAdvancedSmoke(0.0f, 2.0f, 0.0f, 0.8f, 3.0f);
        
        // Create some sparks
        particleSystem->createAdvancedSparks(1.0f, 1.0f, 1.0f, 20, 1.0f);

        // Simulate for 5 seconds
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting advanced particle simulation...");
        
        while (totalTime < 5.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Every second, create some additional effects
            if (static_cast<int>(totalTime) > static_cast<int>(totalTime - deltaTime)) {
                static int effectCounter = 0;
                effectCounter++;
                
                switch (effectCounter % 3) {
                    case 0:
                        particleSystem->createAdvancedExplosion(
                            (effectCounter % 5) - 2.0f,
                            0.0f,
                            (effectCounter % 3) - 1.0f,
                            0.5f, 3.0f);
                        SPARKY_LOG_INFO("Creating explosion effect");
                        break;
                    case 1:
                        particleSystem->createAdvancedSmoke(
                            (effectCounter % 5) - 2.0f,
                            2.0f,
                            (effectCounter % 3) - 1.0f,
                            0.5f, 2.0f);
                        SPARKY_LOG_INFO("Creating smoke effect");
                        break;
                    case 2:
                        particleSystem->createAdvancedSparks(
                            (effectCounter % 5) - 2.0f,
                            1.0f,
                            (effectCounter % 3) - 1.0f,
                            10, 0.5f);
                        SPARKY_LOG_INFO("Creating sparks effect");
                        break;
                }
            }
            
            // Sleep to simulate frame time (in a real application, this would be handled by the engine)
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
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