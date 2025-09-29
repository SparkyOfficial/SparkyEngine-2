#include "../include/ParticleSystem.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <random>
#include <algorithm>
#include <cmath>

namespace Sparky {

    ParticleSystem::ParticleSystem() : 
        activeParticleCount(0),
        emissionRate(10.0f),
        emissionAccumulator(0.0f),
        minLifetime(1.0f),
        maxLifetime(3.0f),
        startColor(1.0f, 1.0f, 1.0f, 1.0f),
        endColor(1.0f, 1.0f, 1.0f, 0.0f),
        startSize(0.1f),
        endSize(0.1f),
        minSpeed(1.0f),
        maxSpeed(3.0f),
        gravity(0.0f, -9.81f, 0.0f),
        emissionShape(EmissionShape::SPHERE),
        sphereRadius(0.1f),
        boxSize(0.1f, 0.1f, 0.1f),
        playing(true),
        loop(true) {
        
        // Pre-allocate particles
        particles.resize(1000);
        for (auto& particle : particles) {
            particle.active = false;
        }
    }

    ParticleSystem::~ParticleSystem() {
    }

    void ParticleSystem::update(float deltaTime) {
        if (!playing) return;
        
        // Emit new particles
        emitParticles(deltaTime);
        
        // Update existing particles
        updateParticles(deltaTime);
    }

    void ParticleSystem::render() {
        // Particle system doesn't directly render - particles are rendered by the renderer
        // This method is kept for potential future enhancements
    }

    void ParticleSystem::setEmissionRate(float rate) {
        emissionRate = rate;
    }

    void ParticleSystem::setParticleLifetime(float min, float max) {
        minLifetime = min;
        maxLifetime = max;
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

    void ParticleSystem::setStartSpeed(float min, float max) {
        minSpeed = min;
        maxSpeed = max;
    }

    void ParticleSystem::setGravity(const glm::vec3& gravity) {
        this->gravity = gravity;
    }

    void ParticleSystem::setEmissionShapeSphere(float radius) {
        emissionShape = EmissionShape::SPHERE;
        sphereRadius = radius;
    }

    void ParticleSystem::setEmissionShapeBox(const glm::vec3& size) {
        emissionShape = EmissionShape::BOX;
        boxSize = size;
    }

    void ParticleSystem::play() {
        playing = true;
    }

    void ParticleSystem::pause() {
        playing = false;
    }

    void ParticleSystem::stop() {
        playing = false;
        // Deactivate all particles
        for (auto& particle : particles) {
            particle.active = false;
        }
        activeParticleCount = 0;
    }

    void ParticleSystem::reset() {
        stop();
        play();
    }

    void ParticleSystem::emitParticles(float deltaTime) {
        emissionAccumulator += emissionRate * deltaTime;
        
        while (emissionAccumulator >= 1.0f && activeParticleCount < particles.size()) {
            // Find an inactive particle to activate
            for (auto& particle : particles) {
                if (!particle.active) {
                    initializeParticle(particle);
                    activeParticleCount++;
                    emissionAccumulator -= 1.0f;
                    break;
                }
            }
        }
    }

    void ParticleSystem::updateParticles(float deltaTime) {
        for (auto& particle : particles) {
            if (particle.active) {
                // Update life
                particle.life -= deltaTime;
                
                if (particle.life <= 0.0f) {
                    // Particle died
                    particle.active = false;
                    activeParticleCount--;
                    continue;
                }
                
                // Apply gravity
                particle.acceleration = gravity;
                
                // Update velocity
                particle.velocity += particle.acceleration * deltaTime;
                
                // Update position
                particle.position += particle.velocity * deltaTime;
                
                // Update color and size based on life
                float lifeRatio = particle.life / particle.maxLife;
                particle.color = glm::mix(endColor, startColor, lifeRatio);
                particle.size = glm::mix(endSize, startSize, lifeRatio);
            }
        }
    }

    void ParticleSystem::initializeParticle(Particle& particle) {
        particle.position = owner ? owner->getPosition() : glm::vec3(0.0f);
        particle.position += getRandomEmissionPosition();
        particle.velocity = getRandomEmissionVelocity();
        particle.acceleration = glm::vec3(0.0f);
        particle.color = startColor;
        particle.size = startSize;
        particle.maxLife = minLifetime + static_cast<float>(rand()) / RAND_MAX * (maxLifetime - minLifetime);
        particle.life = particle.maxLife;
        particle.active = true;
    }

    glm::vec3 ParticleSystem::getRandomEmissionPosition() const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
        
        if (emissionShape == EmissionShape::SPHERE) {
            // Random point in sphere
            glm::vec3 point;
            do {
                point = glm::vec3(dis(gen), dis(gen), dis(gen)) * sphereRadius;
            } while (glm::length(point) > sphereRadius);
            return point;
        } else {
            // Random point in box
            return glm::vec3(
                dis(gen) * boxSize.x * 0.5f,
                dis(gen) * boxSize.y * 0.5f,
                dis(gen) * boxSize.z * 0.5f
            );
        }
    }

    glm::vec3 ParticleSystem::getRandomEmissionVelocity() const {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
        
        glm::vec3 direction;
        if (emissionShape == EmissionShape::SPHERE) {
            // Random direction for sphere emission
            do {
                direction = glm::vec3(dis(gen), dis(gen), dis(gen));
            } while (glm::length(direction) == 0.0f);
            direction = glm::normalize(direction);
        } else {
            // Upward direction for box emission with some randomness
            direction = glm::vec3(dis(gen) * 0.1f, 1.0f, dis(gen) * 0.1f);
        }
        
        float speed = minSpeed + dis(gen) * (maxSpeed - minSpeed);
        return direction * speed;
    }
}