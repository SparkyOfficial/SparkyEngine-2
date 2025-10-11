#include "../include/ParticleSystem.h"
#include "../include/Logger.h"
#include "../include/ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <algorithm>
#include <cmath>

namespace Sparky {
    
    // ParticleProperties implementation
    ParticleProperties::ParticleProperties() : 
        startSize(0.1f), endSize(0.0f), life(0.0f), maxLife(1.0f),
        rotation(0.0f), rotationSpeed(0.0f), mass(1.0f), type(ParticleType::DEFAULT) {
        // Initialize arrays to zero
        for (int i = 0; i < 3; ++i) {
            position[i] = 0.0f;
            velocity[i] = 0.0f;
            acceleration[i] = 0.0f;
        }
        for (int i = 0; i < 4; ++i) {
            startColor[i] = 1.0f;
            endColor[i] = 1.0f;
        }
    }
    
    // EmitterProperties implementation
    EmitterProperties::EmitterProperties() :
        spread(0.0f), speed(1.0f), speedVariation(0.0f), emissionRate(100.0f),
        lifetime(2.0f), lifetimeVariation(0.0f), startSize(0.1f), endSize(0.0f),
        sizeVariation(0.0f), rotationSpeed(0.0f), rotationSpeedVariation(0.0f),
        mass(1.0f), massVariation(0.0f), type(ParticleType::DEFAULT) {
        // Initialize arrays
        position[0] = position[1] = position[2] = 0.0f;
        direction[0] = direction[2] = 0.0f; direction[1] = 1.0f; // Default upward
        gravity[0] = gravity[2] = 0.0f; gravity[1] = -9.81f; // Default gravity
        acceleration[0] = acceleration[1] = acceleration[2] = 0.0f;
        
        // Default colors
        startColor[0] = startColor[1] = startColor[2] = startColor[3] = 1.0f;
        endColor[0] = endColor[1] = endColor[2] = 1.0f; endColor[3] = 0.0f;
    }
    
    // ParticleTexture implementation
    ParticleTexture::ParticleTexture() : textureID(0) {
    }
    
    ParticleTexture::~ParticleTexture() {
    }
    
    ParticleSystem::ParticleSystem() : maxParticles(1000), particleCount(0), 
                                     lastEmissionTime(0.0f), systemActive(true) {
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
            updateParticle(particles[i], deltaTime);
            
            // Remove dead particles
            if (particles[i].life <= 0.0f) {
                // Move the last particle to this position
                if (i != particleCount - 1) {
                    particles[i] = particles[particleCount - 1];
                }
                particleCount--;
                continue;
            }
        }
    }
    
    void ParticleSystem::updateParticle(ParticleProperties& particle, float deltaTime) {
        // Update particle life
        particle.life -= deltaTime;
        
        if (particle.life > 0.0f) {
            // Apply physics
            applyPhysics(particle, deltaTime);
            
            // Update rotation
            particle.rotation += particle.rotationSpeed * deltaTime;
            
            // Update particle appearance based on life
            float lifeRatio = particle.life / particle.maxLife;
            // Color and size interpolation would happen here in a full implementation
        }
    }
    
    void ParticleSystem::applyPhysics(ParticleProperties& particle, float deltaTime) {
        // Apply acceleration (including gravity)
        for (int i = 0; i < 3; ++i) {
            particle.velocity[i] += (emitterProps.acceleration[i] + emitterProps.gravity[i]) * deltaTime;
        }
        
        // Update position
        for (int i = 0; i < 3; ++i) {
            particle.position[i] += particle.velocity[i] * deltaTime;
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
        float particlesToEmit = emitterProps.emissionRate * deltaTime;
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
    
    void ParticleSystem::emitBurst(int count) {
        for (int i = 0; i < count && particleCount < maxParticles; ++i) {
            emitParticle();
        }
    }
    
    void ParticleSystem::emitParticle() {
        if (particleCount >= maxParticles) return;
        
        ParticleProperties& particle = particles[particleCount];
        
        // Initialize particle properties from emitter
        for (int i = 0; i < 3; ++i) {
            particle.position[i] = emitterProps.position[i];
        }
        particle.life = particle.maxLife = emitterProps.lifetime;
        particle.startSize = emitterProps.startSize;
        particle.endSize = emitterProps.endSize;
        particle.type = emitterProps.type;
        
        for (int i = 0; i < 4; ++i) {
            particle.startColor[i] = emitterProps.startColor[i];
            particle.endColor[i] = emitterProps.endColor[i];
        }
        
        // Generate random velocity
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> velDis(-1.0f, 1.0f);
        static std::uniform_real_distribution<float> varDis(0.0f, 1.0f);
        
        // Create velocity based on emission direction and spread
        glm::vec3 baseVelocity(emitterProps.direction[0], emitterProps.direction[1], emitterProps.direction[2]);
        
        if (emitterProps.spread > 0.0f) {
            // Add random spread
            glm::vec3 randomOffset(velDis(gen), velDis(gen), velDis(gen));
            randomOffset = glm::normalize(randomOffset) * emitterProps.spread;
            baseVelocity += randomOffset;
        }
        
        // Apply speed variation
        float speed = emitterProps.speed;
        if (emitterProps.speedVariation > 0.0f) {
            speed += (varDis(gen) - 0.5f) * 2.0f * emitterProps.speedVariation;
        }
        
        baseVelocity = glm::normalize(baseVelocity) * speed;
        
        particle.velocity[0] = baseVelocity.x;
        particle.velocity[1] = baseVelocity.y;
        particle.velocity[2] = baseVelocity.z;
        
        // Set rotation speed with variation
        particle.rotationSpeed = emitterProps.rotationSpeed;
        if (emitterProps.rotationSpeedVariation > 0.0f) {
            particle.rotationSpeed += (varDis(gen) - 0.5f) * 2.0f * emitterProps.rotationSpeedVariation;
        }
        
        // Set mass with variation
        particle.mass = emitterProps.mass;
        if (emitterProps.massVariation > 0.0f) {
            particle.mass += (varDis(gen) - 0.5f) * 2.0f * emitterProps.massVariation;
        }
        
        particleCount++;
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
    
    void ParticleSystem::interpolateColor(float* result, const float* start, const float* end, float ratio) {
        for (int i = 0; i < 4; ++i) {
            result[i] = start[i] + (end[i] - start[i]) * ratio;
        }
    }
    
    float ParticleSystem::interpolateFloat(float start, float end, float ratio) {
        return start + (end - start) * ratio;
    }
    
    // Advanced preset particle effects
    void ParticleSystem::createExplosion(float x, float y, float z, float intensity) {
        // Configure for explosion effect
        emitterProps.position[0] = x;
        emitterProps.position[1] = y;
        emitterProps.position[2] = z;
        emitterProps.direction[0] = 0.0f; emitterProps.direction[1] = 1.0f; emitterProps.direction[2] = 0.0f;
        emitterProps.spread = 1.0f; // Full spherical spread
        emitterProps.speed = 10.0f * intensity;  // High initial speed
        emitterProps.speedVariation = 5.0f * intensity; // Speed variation
        emitterProps.emissionRate = 1000.0f * intensity; // Very high emission rate for instant effect
        emitterProps.lifetime = 1.0f; // Longer lifetime
        emitterProps.lifetimeVariation = 0.5f; // Lifetime variation
        emitterProps.startColor[0] = 1.0f; emitterProps.startColor[1] = 0.5f; emitterProps.startColor[2] = 0.0f; emitterProps.startColor[3] = 1.0f; // Orange
        emitterProps.endColor[0] = 0.5f; emitterProps.endColor[1] = 0.0f; emitterProps.endColor[2] = 0.0f; emitterProps.endColor[3] = 0.0f;   // Dark red fading to transparent
        emitterProps.startSize = 0.2f * intensity;
        emitterProps.endSize = 0.0f;
        emitterProps.sizeVariation = 0.1f * intensity;
        emitterProps.gravity[0] = 0.0f; emitterProps.gravity[1] = -5.0f; emitterProps.gravity[2] = 0.0f; // Some gravity to pull particles down
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 2.0f;
        emitterProps.rotationSpeedVariation = 1.0f;
        emitterProps.mass = 1.0f;
        emitterProps.massVariation = 0.5f;
        emitterProps.type = ParticleType::EXPLOSION;
        
        // Emit a burst of particles
        emitBurst(static_cast<int>(200 * intensity));
        
        // Turn off continuous emission after burst
        emitterProps.emissionRate = 0.0f;
    }
    
    void ParticleSystem::createSmoke(float x, float y, float z, float density) {
        // Configure for smoke effect
        emitterProps.position[0] = x;
        emitterProps.position[1] = y;
        emitterProps.position[2] = z;
        emitterProps.direction[0] = 0.0f; emitterProps.direction[1] = 1.0f; emitterProps.direction[2] = 0.0f; // Upward
        emitterProps.spread = 0.2f; // Narrow spread
        emitterProps.speed = 1.0f;   // Slow initial speed
        emitterProps.speedVariation = 0.5f;
        emitterProps.emissionRate = 50.0f * density;   // Moderate emission rate
        emitterProps.lifetime = 3.0f; // Long lifetime
        emitterProps.lifetimeVariation = 1.0f;
        emitterProps.startColor[0] = 0.5f; emitterProps.startColor[1] = 0.5f; emitterProps.startColor[2] = 0.5f; emitterProps.startColor[3] = 0.8f * density; // Gray, semi-transparent
        emitterProps.endColor[0] = 0.8f; emitterProps.endColor[1] = 0.8f; emitterProps.endColor[2] = 0.8f; emitterProps.endColor[3] = 0.0f;   // Light gray fading to transparent
        emitterProps.startSize = 0.1f;
        emitterProps.endSize = 0.5f; // Particles grow over time
        emitterProps.sizeVariation = 0.2f;
        emitterProps.gravity[0] = 0.0f; emitterProps.gravity[1] = 0.5f; emitterProps.gravity[2] = 0.0f; // Gentle upward force
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 0.5f;
        emitterProps.rotationSpeedVariation = 0.3f;
        emitterProps.mass = 0.1f;
        emitterProps.massVariation = 0.05f;
        emitterProps.type = ParticleType::SMOKE;
        
        // Enable continuous emission
        setActive(true);
    }
    
    void ParticleSystem::createFire(float x, float y, float z, float intensity) {
        // Configure for fire effect
        emitterProps.position[0] = x;
        emitterProps.position[1] = y;
        emitterProps.position[2] = z;
        emitterProps.direction[0] = 0.0f; emitterProps.direction[1] = 1.0f; emitterProps.direction[2] = 0.0f; // Upward
        emitterProps.spread = 0.3f; // Moderate spread
        emitterProps.speed = 3.0f * intensity;   // Moderate speed
        emitterProps.speedVariation = 1.0f * intensity;
        emitterProps.emissionRate = 200.0f * intensity;   // High emission rate
        emitterProps.lifetime = 1.5f; // Medium lifetime
        emitterProps.lifetimeVariation = 0.5f;
        emitterProps.startColor[0] = 1.0f; emitterProps.startColor[1] = 0.3f; emitterProps.startColor[2] = 0.0f; emitterProps.startColor[3] = 1.0f; // Red-orange
        emitterProps.endColor[0] = 1.0f; emitterProps.endColor[1] = 1.0f; emitterProps.endColor[2] = 0.0f; emitterProps.endColor[3] = 0.0f;   // Yellow fading to transparent
        emitterProps.startSize = 0.15f * intensity;
        emitterProps.endSize = 0.05f * intensity; // Particles shrink over time
        emitterProps.sizeVariation = 0.1f * intensity;
        emitterProps.gravity[0] = 0.0f; emitterProps.gravity[1] = 2.0f; emitterProps.gravity[2] = 0.0f; // Upward force
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 1.0f;
        emitterProps.rotationSpeedVariation = 0.5f;
        emitterProps.mass = 0.05f;
        emitterProps.massVariation = 0.02f;
        emitterProps.type = ParticleType::FIRE;
        
        // Enable continuous emission
        setActive(true);
    }
    
    void ParticleSystem::createSparks(float x, float y, float z, int count) {
        // Configure for sparks effect
        emitterProps.position[0] = x;
        emitterProps.position[1] = y;
        emitterProps.position[2] = z;
        emitterProps.direction[0] = 0.0f; emitterProps.direction[1] = 1.0f; emitterProps.direction[2] = 0.0f; // Upward bias
        emitterProps.spread = 1.0f; // Full spread
        emitterProps.speed = 15.0f;   // Fast speed
        emitterProps.speedVariation = 10.0f;
        emitterProps.emissionRate = 0.0f;   // No continuous emission
        emitterProps.lifetime = 0.5f; // Short lifetime
        emitterProps.lifetimeVariation = 0.2f;
        emitterProps.startColor[0] = 1.0f; emitterProps.startColor[1] = 1.0f; emitterProps.startColor[2] = 0.0f; emitterProps.startColor[3] = 1.0f; // Yellow
        emitterProps.endColor[0] = 1.0f; emitterProps.endColor[1] = 0.5f; emitterProps.endColor[2] = 0.0f; emitterProps.endColor[3] = 0.0f;   // Orange fading to transparent
        emitterProps.startSize = 0.02f;
        emitterProps.endSize = 0.0f; // Disappear
        emitterProps.sizeVariation = 0.01f;
        emitterProps.gravity[0] = 0.0f; emitterProps.gravity[1] = -9.81f; emitterProps.gravity[2] = 0.0f; // Gravity
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 5.0f;
        emitterProps.rotationSpeedVariation = 3.0f;
        emitterProps.mass = 0.01f;
        emitterProps.massVariation = 0.005f;
        emitterProps.type = ParticleType::SPARK;
        
        // Emit a burst of particles
        emitBurst(count);
    }
    
    void ParticleSystem::createBlood(float x, float y, float z, float dirX, float dirY, float dirZ, int count) {
        // Configure for blood effect
        emitterProps.position[0] = x;
        emitterProps.position[1] = y;
        emitterProps.position[2] = z;
        emitterProps.direction[0] = dirX; emitterProps.direction[1] = dirY; emitterProps.direction[2] = dirZ; // Direction of impact
        emitterProps.spread = 0.5f; // Moderate spread
        emitterProps.speed = 5.0f;   // Moderate speed
        emitterProps.speedVariation = 3.0f;
        emitterProps.emissionRate = 0.0f;   // No continuous emission
        emitterProps.lifetime = 2.0f; // Medium lifetime
        emitterProps.lifetimeVariation = 0.5f;
        emitterProps.startColor[0] = 0.8f; emitterProps.startColor[1] = 0.0f; emitterProps.startColor[2] = 0.0f; emitterProps.startColor[3] = 1.0f; // Dark red
        emitterProps.endColor[0] = 0.3f; emitterProps.endColor[1] = 0.0f; emitterProps.endColor[2] = 0.0f; emitterProps.endColor[3] = 0.0f;   // Darker red fading to transparent
        emitterProps.startSize = 0.05f;
        emitterProps.endSize = 0.0f; // Disappear
        emitterProps.sizeVariation = 0.02f;
        emitterProps.gravity[0] = 0.0f; emitterProps.gravity[1] = -9.81f; emitterProps.gravity[2] = 0.0f; // Gravity
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 2.0f;
        emitterProps.rotationSpeedVariation = 1.0f;
        emitterProps.mass = 0.1f;
        emitterProps.massVariation = 0.05f;
        emitterProps.type = ParticleType::BLOOD;
        
        // Emit a burst of particles
        emitBurst(count);
    }
    
    void ParticleSystem::createMist(float x, float y, float z, float density) {
        // Configure for mist effect
        emitterProps.position[0] = x;
        emitterProps.position[1] = y;
        emitterProps.position[2] = z;
        emitterProps.direction[0] = 0.0f; emitterProps.direction[1] = 1.0f; emitterProps.direction[2] = 0.0f; // Upward
        emitterProps.spread = 0.5f; // Wide spread
        emitterProps.speed = 0.5f;   // Very slow speed
        emitterProps.speedVariation = 0.2f;
        emitterProps.emissionRate = 30.0f * density;   // Low emission rate
        emitterProps.lifetime = 5.0f; // Long lifetime
        emitterProps.lifetimeVariation = 1.0f;
        emitterProps.startColor[0] = 0.9f; emitterProps.startColor[1] = 0.9f; emitterProps.startColor[2] = 1.0f; emitterProps.startColor[3] = 0.3f * density; // Light blue, very transparent
        emitterProps.endColor[0] = 0.9f; emitterProps.endColor[1] = 0.9f; emitterProps.endColor[2] = 1.0f; emitterProps.endColor[3] = 0.0f;   // Same color fading to transparent
        emitterProps.startSize = 0.3f;
        emitterProps.endSize = 0.8f; // Particles grow over time
        emitterProps.sizeVariation = 0.1f;
        emitterProps.gravity[0] = 0.0f; emitterProps.gravity[1] = 0.1f; emitterProps.gravity[2] = 0.0f; // Very gentle upward force
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 0.1f;
        emitterProps.rotationSpeedVariation = 0.05f;
        emitterProps.mass = 0.01f;
        emitterProps.massVariation = 0.005f;
        emitterProps.type = ParticleType::MIST;
        
        // Enable continuous emission
        setActive(true);
    }
    
    void ParticleSystem::createTrail(float startX, float startY, float startZ, float endX, float endY, float endZ, float duration) {
        // Configure for trail effect
        emitterProps.position[0] = startX;
        emitterProps.position[1] = startY;
        emitterProps.position[2] = startZ;
        emitterProps.direction[0] = endX - startX; 
        emitterProps.direction[1] = endY - startY; 
        emitterProps.direction[2] = endZ - startZ;
        
        // Normalize direction
        float length = std::sqrt(emitterProps.direction[0] * emitterProps.direction[0] +
                                emitterProps.direction[1] * emitterProps.direction[1] +
                                emitterProps.direction[2] * emitterProps.direction[2]);
        if (length > 0) {
            emitterProps.direction[0] /= length;
            emitterProps.direction[1] /= length;
            emitterProps.direction[2] /= length;
        }
        
        emitterProps.spread = 0.1f; // Tight spread
        emitterProps.speed = 2.0f;   // Moderate speed
        emitterProps.speedVariation = 0.5f;
        emitterProps.emissionRate = 100.0f;   // Moderate emission rate
        emitterProps.lifetime = duration; // Lifetime matches duration
        emitterProps.lifetimeVariation = duration * 0.2f;
        emitterProps.startColor[0] = 1.0f; emitterProps.startColor[1] = 1.0f; emitterProps.startColor[2] = 1.0f; emitterProps.startColor[3] = 0.8f; // White, semi-transparent
        emitterProps.endColor[0] = 1.0f; emitterProps.endColor[1] = 1.0f; emitterProps.endColor[2] = 1.0f; emitterProps.endColor[3] = 0.0f;   // White fading to transparent
        emitterProps.startSize = 0.05f;
        emitterProps.endSize = 0.0f; // Disappear
        emitterProps.sizeVariation = 0.02f;
        emitterProps.gravity[0] = emitterProps.gravity[1] = emitterProps.gravity[2] = 0.0f; // No gravity
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 0.0f;
        emitterProps.rotationSpeedVariation = 0.0f;
        emitterProps.mass = 0.01f;
        emitterProps.massVariation = 0.005f;
        emitterProps.type = ParticleType::TRAIL;
        
        // Enable continuous emission
        setActive(true);
    }
    
    void ParticleSystem::createShockwave(float x, float y, float z, float radius, float intensity) {
        // Configure for shockwave effect
        emitterProps.position[0] = x;
        emitterProps.position[1] = y;
        emitterProps.position[2] = z;
        emitterProps.direction[0] = 0.0f; emitterProps.direction[1] = 1.0f; emitterProps.direction[2] = 0.0f; // Upward bias
        emitterProps.spread = 0.1f; // Tight spread
        emitterProps.speed = 20.0f * intensity;   // Very fast speed
        emitterProps.speedVariation = 5.0f * intensity;
        emitterProps.emissionRate = 500.0f * intensity;   // High emission rate
        emitterProps.lifetime = 0.3f; // Very short lifetime
        emitterProps.lifetimeVariation = 0.1f;
        emitterProps.startColor[0] = 0.5f; emitterProps.startColor[1] = 0.5f; emitterProps.startColor[2] = 1.0f; emitterProps.startColor[3] = 0.7f * intensity; // Blue, semi-transparent
        emitterProps.endColor[0] = 0.5f; emitterProps.endColor[1] = 0.5f; emitterProps.endColor[2] = 1.0f; emitterProps.endColor[3] = 0.0f;   // Blue fading to transparent
        emitterProps.startSize = 0.1f * radius;
        emitterProps.endSize = 0.5f * radius; // Grow significantly
        emitterProps.sizeVariation = 0.05f * radius;
        emitterProps.gravity[0] = emitterProps.gravity[1] = emitterProps.gravity[2] = 0.0f; // No gravity
        emitterProps.acceleration[0] = emitterProps.acceleration[1] = emitterProps.acceleration[2] = 0.0f;
        emitterProps.rotationSpeed = 0.0f;
        emitterProps.rotationSpeedVariation = 0.0f;
        emitterProps.mass = 0.01f;
        emitterProps.massVariation = 0.005f;
        emitterProps.type = ParticleType::DEFAULT;
        
        // Emit a burst of particles in a ring formation
        int particleCount = static_cast<int>(50 * intensity);
        for (int i = 0; i < particleCount && this->particleCount < maxParticles; ++i) {
            emitParticle();
        }
        
        // Turn off continuous emission after burst
        emitterProps.emissionRate = 0.0f;
    }
    
    // Effect combinations
    void ParticleSystem::createExplosionWithSmoke(float x, float y, float z, float intensity) {
        // Create explosion
        createExplosion(x, y, z, intensity);
        
        // Create smoke
        createSmoke(x, y, z, intensity);
    }
    
    void ParticleSystem::createFireWithSmoke(float x, float y, float z, float intensity) {
        // Create fire
        createFire(x, y, z, intensity);
        
        // Create smoke
        createSmoke(x, y, z + 1.0f, intensity * 0.5f); // Smoke above the fire
    }
}