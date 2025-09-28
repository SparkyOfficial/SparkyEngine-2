#include "../include/ParticleSystem.h"
#include "../include/Logger.h"
#include <random>
#include <algorithm>

namespace Sparky {

    ParticleSystem::ParticleSystem(int maxParticles) : 
        maxParticles(maxParticles), running(false),
        emissionRate(100.0f), emissionAccumulator(0.0f),
        gravity(0.0f, -9.81f, 0.0f),
        minLife(1.0f), maxLife(3.0f),
        minSize(0.1f), maxSize(0.5f),
        minColor(1.0f, 1.0f, 1.0f, 1.0f),
        maxColor(1.0f, 1.0f, 1.0f, 1.0f) {
        
        particles.resize(maxParticles);
        
        // Initialize all particles as inactive
        for (auto& particle : particles) {
            particle.active = false;
        }
        
        SPARKY_LOG_DEBUG("Created particle system with " + std::to_string(maxParticles) + " particles");
    }

    ParticleSystem::~ParticleSystem() {
    }

    void ParticleSystem::emitParticle(const glm::vec3& position, const glm::vec3& velocity, 
                                     const glm::vec4& color, float life, float size) {
        // Find an inactive particle slot
        for (auto& particle : particles) {
            if (!particle.active) {
                particle.position = position;
                particle.velocity = velocity;
                particle.color = color;
                particle.life = life;
                particle.maxLife = life;
                particle.size = size;
                particle.active = true;
                return;
            }
        }
        
        // If we get here, all particles are active
        SPARKY_LOG_WARNING("Particle system is full, cannot emit new particle");
    }

    void ParticleSystem::emitParticles(int count, const glm::vec3& position, const glm::vec3& baseVelocity,
                                      const glm::vec4& color, float life, float size, float spread) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
        
        for (int i = 0; i < count; i++) {
            // Add random spread to velocity
            glm::vec3 velocity = baseVelocity;
            velocity.x += dis(gen) * spread;
            velocity.y += dis(gen) * spread;
            velocity.z += dis(gen) * spread;
            
            // Add random variation to life and size
            float randomLife = life + dis(gen) * 0.5f;
            float randomSize = size + dis(gen) * 0.1f;
            
            emitParticle(position, velocity, color, randomLife, randomSize);
        }
    }

    void ParticleSystem::setEmissionRate(float rate) {
        emissionRate = rate;
        SPARKY_LOG_DEBUG("Particle emission rate set to: " + std::to_string(rate));
    }

    void ParticleSystem::setGravity(const glm::vec3& gravity) {
        this->gravity = gravity;
        SPARKY_LOG_DEBUG("Particle gravity set to: (" + std::to_string(gravity.x) + ", " + 
                        std::to_string(gravity.y) + ", " + std::to_string(gravity.z) + ")");
    }

    void ParticleSystem::setLifetime(float minLife, float maxLife) {
        this->minLife = minLife;
        this->maxLife = maxLife;
        SPARKY_LOG_DEBUG("Particle lifetime set to: " + std::to_string(minLife) + " - " + std::to_string(maxLife));
    }

    void ParticleSystem::setStartSize(float minSize, float maxSize) {
        this->minSize = minSize;
        this->maxSize = maxSize;
        SPARKY_LOG_DEBUG("Particle size set to: " + std::to_string(minSize) + " - " + std::to_string(maxSize));
    }

    void ParticleSystem::setStartColor(const glm::vec4& minColor, const glm::vec4& maxColor) {
        this->minColor = minColor;
        this->maxColor = maxColor;
        SPARKY_LOG_DEBUG("Particle color range set");
    }

    void ParticleSystem::update(float deltaTime) {
        if (!running) return;

        // Update existing particles
        for (auto& particle : particles) {
            if (particle.active) {
                // Update life
                particle.life -= deltaTime;
                
                if (particle.life <= 0.0f) {
                    particle.active = false;
                    continue;
                }
                
                // Update position based on velocity
                particle.position += particle.velocity * deltaTime;
                
                // Apply gravity
                particle.velocity += gravity * deltaTime;
                
                // Update color based on life (fade out)
                float lifeRatio = particle.life / particle.maxLife;
                particle.color.a = lifeRatio;
            }
        }

        // Handle continuous emission
        if (emissionRate > 0.0f) {
            emissionAccumulator += emissionRate * deltaTime;
            
            int particlesToEmit = static_cast<int>(emissionAccumulator);
            if (particlesToEmit > 0) {
                // Emit particles at origin with upward velocity for demonstration
                static std::random_device rd;
                static std::mt19937 gen(rd());
                static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
                
                for (int i = 0; i < particlesToEmit; i++) {
                    glm::vec3 position(0.0f, 0.0f, 0.0f);
                    glm::vec3 velocity(0.0f, 5.0f, 0.0f);
                    glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
                    float life = minLife + dis(gen) * (maxLife - minLife);
                    float size = minSize + dis(gen) * (maxSize - minSize);
                    
                    emitParticle(position, velocity, color, life, size);
                }
                
                emissionAccumulator -= particlesToEmit;
            }
        }

        SPARKY_LOG_DEBUG("Updated particle system with " + std::to_string(getActiveParticleCount()) + " active particles");
    }

    void ParticleSystem::render() {
        if (!running) return;

        // Render all active particles using the graphics API
        // In a complete implementation, this would render all active particles with their current properties
        int activeCount = getActiveParticleCount();
        if (activeCount > 0) {
            SPARKY_LOG_DEBUG("Rendering " + std::to_string(activeCount) + " particles");
        }
    }

    void ParticleSystem::start() {
        running = true;
        SPARKY_LOG_DEBUG("Particle system started");
    }

    void ParticleSystem::stop() {
        running = false;
        SPARKY_LOG_DEBUG("Particle system stopped");
    }

    void ParticleSystem::reset() {
        for (auto& particle : particles) {
            particle.active = false;
        }
        emissionAccumulator = 0.0f;
        SPARKY_LOG_DEBUG("Particle system reset");
    }

    int ParticleSystem::getActiveParticleCount() const {
        return std::count_if(particles.begin(), particles.end(), 
                            [](const Particle& p) { return p.active; });
    }
}