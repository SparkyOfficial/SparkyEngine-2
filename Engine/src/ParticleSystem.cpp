#include "../include/ParticleSystem.h"
#include "../include/Logger.h"
#include "../include/ResourceManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <algorithm>

namespace Sparky {
    
    Particle::Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f), maxLife(1.0f) {
    }
    
    ParticleSystem::ParticleSystem() : maxParticles(1000), particleCount(0), 
                                     emissionRate(100.0f), lastEmissionTime(0.0f),
                                     particleLifetime(2.0f), startColor(1.0f, 1.0f, 1.0f, 1.0f),
                                     endColor(1.0f, 1.0f, 1.0f, 0.0f), 
                                     startSize(0.1f), endSize(0.0f),
                                     gravity(0.0f, -9.81f, 0.0f), 
                                     systemActive(true) {
        particles.resize(maxParticles);
        SPARKY_LOG_DEBUG("ParticleSystem created with max " + std::to_string(maxParticles) + " particles");
    }
    
    ParticleSystem::~ParticleSystem() {
        SPARKY_LOG_DEBUG("ParticleSystem destroyed");
    }
    
    void ParticleSystem::update(float deltaTime) {
        if (!systemActive) return;
        
        // Emit new particles
        emitParticles(deltaTime);
        
        // Update existing particles
        for (int i = particleCount - 1; i >= 0; --i) {
            Particle& particle = particles[i];
            
            // Update particle life
            particle.life -= deltaTime;
            
            // Remove dead particles
            if (particle.life <= 0.0f) {
                // Move the last particle to this position
                if (i != particleCount - 1) {
                    particles[i] = particles[particleCount - 1];
                }
                particleCount--;
                continue;
            }
            
            // Update particle physics
            particle.velocity += gravity * deltaTime;
            particle.position += particle.velocity * deltaTime;
            
            // Update particle appearance based on life
            float lifeRatio = particle.life / particle.maxLife;
            particle.color = glm::mix(endColor, startColor, lifeRatio);
            particle.size = glm::mix(endSize, startSize, lifeRatio);
        }
    }
    
    void ParticleSystem::render() {
        // In a full implementation, we would render all particles
        // For now, we'll just log the particle count
        if (particleCount > 0) {
            SPARKY_LOG_DEBUG("Rendering " + std::to_string(particleCount) + " particles");
        }
    }
    
    void ParticleSystem::emitParticles(float deltaTime) {
        // Calculate how many particles to emit
        float particlesToEmit = emissionRate * deltaTime;
        int intParticlesToEmit = static_cast<int>(particlesToEmit);
        
        // Add fractional particles to a remainder for next frame
        static float emissionRemainder = 0.0f;
        emissionRemainder += particlesToEmit - intParticlesToEmit;
        if (emissionRemainder >= 1.0f) {
            intParticlesToEmit += static_cast<int>(emissionRemainder);
            emissionRemainder -= static_cast<int>(emissionRemainder);
        }
        
        // Emit particles
        for (int i = 0; i < intParticlesToEmit && particleCount < maxParticles; ++i) {
            emitParticle();
        }
    }
    
    void ParticleSystem::emitParticle() {
        if (particleCount >= maxParticles) return;
        
        Particle& particle = particles[particleCount];
        
        // Initialize particle properties
        particle.position = emissionPoint;
        particle.life = particle.maxLife = particleLifetime;
        particle.size = startSize;
        particle.color = startColor;
        
        // Generate random velocity
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> velDis(-1.0f, 1.0f);
        
        // Create velocity based on emission direction and spread
        glm::vec3 baseVelocity = emissionDirection;
        if (emissionSpread > 0.0f) {
            // Add random spread
            glm::vec3 randomOffset(velDis(gen), velDis(gen), velDis(gen));
            randomOffset = glm::normalize(randomOffset) * emissionSpread;
            baseVelocity += randomOffset;
        }
        
        particle.velocity = glm::normalize(baseVelocity) * emissionSpeed;
        
        particleCount++;
    }
    
    void ParticleSystem::setEmissionPoint(const glm::vec3& point) {
        emissionPoint = point;
    }
    
    void ParticleSystem::setEmissionDirection(const glm::vec3& direction) {
        emissionDirection = glm::normalize(direction);
    }
    
    void ParticleSystem::setEmissionSpread(float spread) {
        emissionSpread = spread;
    }
    
    void ParticleSystem::setEmissionSpeed(float speed) {
        emissionSpeed = speed;
    }
    
    void ParticleSystem::setEmissionRate(float rate) {
        emissionRate = rate;
    }
    
    void ParticleSystem::setParticleLifetime(float lifetime) {
        particleLifetime = lifetime;
    }
    
    void ParticleSystem::setStartColor(const glm::vec4& color) {
        startColor = color;
    }
    
    void ParticleSystem::setEndColor(const glm::vec4& color) {
        endColor = color;
    }
    
    void ParticleSystem::setStartSize(float size) {
        startSize = size;
    }
    
    void ParticleSystem::setEndSize(float size) {
        endSize = size;
    }
    
    void ParticleSystem::setGravity(const glm::vec3& grav) {
        gravity = grav;
    }
    
    void ParticleSystem::setActive(bool active) {
        systemActive = active;
    }
    
    bool ParticleSystem::isActive() const {
        return systemActive;
    }
    
    int ParticleSystem::getParticleCount() const {
        return particleCount;
    }
    
    // Preset particle effects
    void ParticleSystem::createMuzzleFlash(const glm::vec3& position, const glm::vec3& direction) {
        // Configure for muzzle flash effect
        setEmissionPoint(position);
        setEmissionDirection(direction);
        setEmissionSpread(0.5f); // Wide spread
        setEmissionSpeed(2.0f);   // Fast initial speed
        setEmissionRate(500.0f);  // High emission rate for instant effect
        setParticleLifetime(0.1f); // Very short lifetime
        setStartColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow
        setEndColor(glm::vec4(1.0f, 0.5f, 0.0f, 0.0f));   // Orange fading to transparent
        setStartSize(0.05f);
        setEndSize(0.0f);
        setGravity(glm::vec3(0.0f)); // No gravity
        
        // Emit a burst of particles
        for (int i = 0; i < 50; ++i) {
            emitParticle();
        }
        
        // Turn off continuous emission after burst
        setEmissionRate(0.0f);
    }
    
    void ParticleSystem::createExplosion(const glm::vec3& position) {
        // Configure for explosion effect
        setEmissionPoint(position);
        setEmissionDirection(glm::vec3(0.0f, 1.0f, 0.0f)); // Upward bias
        setEmissionSpread(1.0f); // Full spherical spread
        setEmissionSpeed(10.0f);  // High initial speed
        setEmissionRate(1000.0f); // Very high emission rate for instant effect
        setParticleLifetime(1.0f); // Longer lifetime
        setStartColor(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)); // Orange
        setEndColor(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f));   // Dark red fading to transparent
        setStartSize(0.2f);
        setEndSize(0.0f);
        setGravity(glm::vec3(0.0f, -5.0f, 0.0f)); // Some gravity to pull particles down
        
        // Emit a burst of particles
        for (int i = 0; i < 200; ++i) {
            emitParticle();
        }
        
        // Turn off continuous emission after burst
        setEmissionRate(0.0f);
    }
    
    void ParticleSystem::createSmoke(const glm::vec3& position) {
        // Configure for smoke effect
        setEmissionPoint(position);
        setEmissionDirection(glm::vec3(0.0f, 1.0f, 0.0f)); // Upward
        setEmissionSpread(0.2f); // Narrow spread
        setEmissionSpeed(1.0f);   // Slow initial speed
        setEmissionRate(50.0f);   // Moderate emission rate
        setParticleLifetime(3.0f); // Long lifetime
        setStartColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.8f)); // Gray, semi-transparent
        setEndColor(glm::vec4(0.8f, 0.8f, 0.8f, 0.0f));   // Light gray fading to transparent
        setStartSize(0.1f);
        setEndSize(0.5f); // Particles grow over time
        setGravity(glm::vec3(0.0f, 0.5f, 0.0f)); // Gentle upward force
        
        // Enable continuous emission
        setActive(true);
    }
}