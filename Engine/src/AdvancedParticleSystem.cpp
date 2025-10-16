#include "../include/AdvancedParticleSystem.h"
#include "../include/GameObject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>
#include <cmath>
#include <random>

namespace Sparky {
    // ParticleForce implementation
    ParticleForce::ParticleForce(ForceType type)
        : m_type(type)
        , m_enabled(true) {
    }
    
    // GravityForce implementation
    GravityForce::GravityForce(const glm::vec3& gravity)
        : ParticleForce(ForceType::GRAVITY)
        , m_gravity(gravity) {
    }
    
    void GravityForce::applyForce(Particle& particle, float deltaTime) {
        if (!m_enabled) return;
        
        particle.acceleration += m_gravity;
    }
    
    // WindForce implementation
    WindForce::WindForce(const glm::vec3& direction, float strength)
        : ParticleForce(ForceType::WIND)
        , m_direction(glm::normalize(direction))
        , m_strength(strength) {
    }
    
    void WindForce::applyForce(Particle& particle, float deltaTime) {
        if (!m_enabled) return;
        
        particle.acceleration += m_direction * m_strength;
    }
    
    // VortexForce implementation
    VortexForce::VortexForce(const glm::vec3& center, const glm::vec3& axis, float strength, float attraction)
        : ParticleForce(ForceType::VORTEX)
        , m_center(center)
        , m_axis(glm::normalize(axis))
        , m_strength(strength)
        , m_attraction(attraction) {
    }
    
    void VortexForce::applyForce(Particle& particle, float deltaTime) {
        if (!m_enabled) return;
        
        glm::vec3 offset = particle.position - m_center;
        glm::vec3 radial = offset - glm::dot(offset, m_axis) * m_axis;
        glm::vec3 tangent = glm::cross(m_axis, radial);
        
        // Vortex force
        particle.acceleration += glm::normalize(tangent) * m_strength;
        
        // Attraction toward center
        particle.acceleration -= glm::normalize(radial) * m_attraction;
    }
    
    // TurbulenceForce implementation
    TurbulenceForce::TurbulenceForce(float scale, float speed, float strength)
        : ParticleForce(ForceType::TURBULENCE)
        , m_scale(scale)
        , m_speed(speed)
        , m_strength(strength)
        , m_randomGenerator(std::random_device{}())
        , m_randomDistribution(-1.0f, 1.0f) {
    }
    
    void TurbulenceForce::applyForce(Particle& particle, float deltaTime) {
        if (!m_enabled) return;
        
        // Simple turbulence using random forces
        glm::vec3 turbulence(
            m_randomDistribution(m_randomGenerator),
            m_randomDistribution(m_randomGenerator),
            m_randomDistribution(m_randomGenerator)
        );
        
        particle.acceleration += turbulence * m_strength;
    }
    
    // ParticleModifier implementation
    ParticleModifier::ParticleModifier(ModifierType type)
        : m_type(type)
        , m_enabled(true) {
    }
    
    // ColorOverLifetimeModifier implementation
    ColorOverLifetimeModifier::ColorOverLifetimeModifier()
        : ParticleModifier(ModifierType::COLOR_OVER_LIFETIME)
        , m_startColor(1.0f, 1.0f, 1.0f, 1.0f)
        , m_endColor(1.0f, 1.0f, 1.0f, 0.0f) {
    }
    
    void ColorOverLifetimeModifier::modifyParticle(Particle& particle, float deltaTime) {
        if (!m_enabled) return;
        
        float t = particle.age / particle.maxLifetime;
        particle.color = glm::mix(m_startColor, m_endColor, t);
    }
    
    void ColorOverLifetimeModifier::setColorRange(const glm::vec4& startColor, const glm::vec4& endColor) {
        m_startColor = startColor;
        m_endColor = endColor;
    }
    
    // SizeOverLifetimeModifier implementation
    SizeOverLifetimeModifier::SizeOverLifetimeModifier()
        : ParticleModifier(ModifierType::SIZE_OVER_LIFETIME)
        , m_startSize(1.0f, 1.0f)
        , m_endSize(0.0f, 0.0f) {
    }
    
    void SizeOverLifetimeModifier::modifyParticle(Particle& particle, float deltaTime) {
        if (!m_enabled) return;
        
        float t = particle.age / particle.maxLifetime;
        particle.size = glm::mix(m_startSize, m_endSize, t);
    }
    
    void SizeOverLifetimeModifier::setSizeRange(const glm::vec2& startSize, const glm::vec2& endSize) {
        m_startSize = startSize;
        m_endSize = endSize;
    }
    
    // RotationOverLifetimeModifier implementation
    RotationOverLifetimeModifier::RotationOverLifetimeModifier()
        : ParticleModifier(ModifierType::ROTATION_OVER_LIFETIME)
        , m_startRotation(0.0f)
        , m_endRotation(0.0f) {
    }
    
    void RotationOverLifetimeModifier::modifyParticle(Particle& particle, float deltaTime) {
        if (!m_enabled) return;
        
        float t = particle.age / particle.maxLifetime;
        particle.rotation = glm::mix(m_startRotation, m_endRotation, t);
    }
    
    void RotationOverLifetimeModifier::setRotationRange(float startRotation, float endRotation) {
        m_startRotation = startRotation;
        m_endRotation = endRotation;
    }
    
    // ParticleEmitter implementation
    ParticleEmitter::ParticleEmitter(EmitterType type)
        : m_type(type)
        , m_emissionRate(10.0f)
        , m_burstCount(10)
        , m_minLifetime(1.0f)
        , m_maxLifetime(5.0f)
        , m_minSpeed(1.0f)
        , m_maxSpeed(5.0f)
        , m_minSize(0.1f, 0.1f)
        , m_maxSize(1.0f, 1.0f)
        , m_minColor(1.0f, 1.0f, 1.0f, 1.0f)
        , m_maxColor(1.0f, 1.0f, 1.0f, 0.0f)
        , m_radius(1.0f)
        , m_size(1.0f, 1.0f, 1.0f)
        , m_direction(0.0f, 1.0f, 0.0f)
        , m_spreadAngle(45.0f)
        , m_randomGenerator(std::random_device{}())
        , m_randomDistribution(0.0f, 1.0f) {
    }
    
    ParticleEmitter::~ParticleEmitter() {
    }
    
    void ParticleEmitter::setLifetimeRange(float minLifetime, float maxLifetime) {
        m_minLifetime = minLifetime;
        m_maxLifetime = maxLifetime;
    }
    
    void ParticleEmitter::setSpeedRange(float minSpeed, float maxSpeed) {
        m_minSpeed = minSpeed;
        m_maxSpeed = maxSpeed;
    }
    
    void ParticleEmitter::setSizeRange(const glm::vec2& minSize, const glm::vec2& maxSize) {
        m_minSize = minSize;
        m_maxSize = maxSize;
    }
    
    void ParticleEmitter::setColorRange(const glm::vec4& minColor, const glm::vec4& maxColor) {
        m_minColor = minColor;
        m_maxColor = maxColor;
    }
    
    void ParticleEmitter::emitBurst(int count) {
        // This would be handled by the particle system
    }
    
    void ParticleEmitter::initializeParticle(Particle& particle, const glm::vec3& emitterPosition) const {
        // Initialize particle properties
        particle.position = generateRandomPosition(emitterPosition);
        particle.velocity = generateRandomDirection() * 
                           (m_minSpeed + m_randomDistribution(m_randomGenerator) * (m_maxSpeed - m_minSpeed));
        particle.acceleration = glm::vec3(0.0f);
        particle.color = glm::mix(m_minColor, m_maxColor, m_randomDistribution(m_randomGenerator));
        particle.size = glm::mix(m_minSize, m_maxSize, m_randomDistribution(m_randomGenerator));
        particle.rotation = 0.0f;
        particle.angularVelocity = 0.0f;
        particle.lifetime = m_minLifetime + m_randomDistribution(m_randomGenerator) * (m_maxLifetime - m_minLifetime);
        particle.maxLifetime = particle.lifetime;
        particle.age = 0.0f;
        particle.active = true;
        particle.uvOffset = glm::vec2(0.0f);
        particle.uvScale = glm::vec2(1.0f);
        particle.customData1 = glm::vec4(0.0f);
        particle.customData2 = glm::vec4(0.0f);
    }
    
    glm::vec3 ParticleEmitter::generateRandomDirection() const {
        float angle1 = m_randomDistribution(m_randomGenerator) * 2.0f * glm::pi<float>();
        float angle2 = m_randomDistribution(m_randomGenerator) * glm::pi<float>();
        
        // Convert angles to direction vector
        glm::vec3 direction;
        direction.x = sin(angle2) * cos(angle1);
        direction.y = cos(angle2);
        direction.z = sin(angle2) * sin(angle1);
        
        // Apply spread angle
        if (m_spreadAngle < 180.0f) {
            float spreadRad = glm::radians(m_spreadAngle);
            // Use mix instead of slerp for vectors
            glm::vec3 spreadDir = glm::mix(m_direction, direction, spreadRad / glm::pi<float>());
            direction = spreadDir;
        }
        
        return glm::normalize(direction);
    }
    
    glm::vec3 ParticleEmitter::generateRandomPosition(const glm::vec3& emitterPosition) const {
        glm::vec3 position = emitterPosition;
        
        switch (m_type) {
            case EmitterType::POINT:
                // No offset for point emitter
                break;
                
            case EmitterType::SPHERE: {
                float u = m_randomDistribution(m_randomGenerator);
                float v = m_randomDistribution(m_randomGenerator);
                float theta = u * 2.0f * glm::pi<float>();
                float phi = acos(2.0f * v - 1.0f);
                float r = m_radius * cbrt(m_randomDistribution(m_randomGenerator));
                float sinTheta = sin(theta);
                float cosTheta = cos(theta);
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);
                position += glm::vec3(r * sinPhi * cosTheta, r * cosPhi, r * sinPhi * sinTheta);
                break;
            }
            
            case EmitterType::BOX: {
                position.x += (m_randomDistribution(m_randomGenerator) - 0.5f) * m_size.x;
                position.y += (m_randomDistribution(m_randomGenerator) - 0.5f) * m_size.y;
                position.z += (m_randomDistribution(m_randomGenerator) - 0.5f) * m_size.z;
                break;
            }
            
            case EmitterType::CIRCLE: {
                float angle = m_randomDistribution(m_randomGenerator) * 2.0f * glm::pi<float>();
                float r = m_radius * sqrt(m_randomDistribution(m_randomGenerator));
                position.x += r * cos(angle);
                position.z += r * sin(angle);
                break;
            }
            
            case EmitterType::CONE: {
                float angle = m_randomDistribution(m_randomGenerator) * 2.0f * glm::pi<float>();
                float r = m_radius * m_randomDistribution(m_randomGenerator);
                position.x += r * cos(angle);
                position.z += r * sin(angle);
                position.y += m_randomDistribution(m_randomGenerator) * m_size.y;
                break;
            }
            
            case EmitterType::HEMISPHERE: {
                float u = m_randomDistribution(m_randomGenerator);
                float v = m_randomDistribution(m_randomGenerator);
                float theta = u * 2.0f * glm::pi<float>();
                float phi = acos(v); // Only upper hemisphere
                float r = m_radius * cbrt(m_randomDistribution(m_randomGenerator));
                float sinTheta = sin(theta);
                float cosTheta = cos(theta);
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);
                position += glm::vec3(r * sinPhi * cosTheta, r * cosPhi, r * sinPhi * sinTheta);
                break;
            }
        }
        
        return position;
    }
    
    // AdvancedParticleSystem implementation
    AdvancedParticleSystem::AdvancedParticleSystem(int maxParticles)
        : ParticleSystem()
        , m_maxParticles(maxParticles)
        , m_duration(5.0f)
        , m_looping(true)
        , m_playOnAwake(true)
        , m_startDelay(0.0f)
        , m_currentTime(0.0f)
        , m_delayTimer(0.0f)
        , m_isPlaying(false)
        , m_isPaused(false)
        , m_isStopped(true)
        , m_blendMode(ParticleBlendMode::ALPHA)
        , m_useWorldSpace(true)
        , m_emissionTimer(0.0f) {
        m_particles.resize(maxParticles);
    }
    
    AdvancedParticleSystem::~AdvancedParticleSystem() {
    }
    
    void AdvancedParticleSystem::initialize() {
        //ParticleSystem::initialize();
        
        // Start playing if configured to do so
        if (m_playOnAwake) {
            play();
        }
    }
    
    void AdvancedParticleSystem::update(float deltaTime) {
        ParticleSystem::update(deltaTime);
        
        if (!m_isPlaying || m_isPaused) return;
        
        // Handle start delay
        if (m_delayTimer < m_startDelay) {
            m_delayTimer += deltaTime;
            return;
        }
        
        // Update system time
        m_currentTime += deltaTime;
        
        // Handle duration and looping
        if (m_currentTime >= m_duration) {
            if (m_looping) {
                m_currentTime = 0.0f;
            } else {
                stop();
                return;
            }
        }
        
        // Update particles
        updateParticles(deltaTime);
        
        // Update emission
        updateEmission(deltaTime);
    }
    
    void AdvancedParticleSystem::destroy() {
        //ParticleSystem::destroy();
        clear();
    }
    
    void AdvancedParticleSystem::render() {
        ParticleSystem::render();
        // Advanced rendering would go here
    }
    
    void AdvancedParticleSystem::setMaxParticles(int maxParticles) {
        m_maxParticles = maxParticles;
        m_particles.resize(maxParticles);
    }
    
    void AdvancedParticleSystem::addEmitter(std::unique_ptr<ParticleEmitter> emitter) {
        if (emitter) {
            m_emitters.push_back(std::move(emitter));
        }
    }
    
    ParticleEmitter* AdvancedParticleSystem::getEmitter(size_t index) const {
        if (index < m_emitters.size()) {
            return m_emitters[index].get();
        }
        return nullptr;
    }
    
    void AdvancedParticleSystem::addForce(std::unique_ptr<ParticleForce> force) {
        if (force) {
            m_forces.push_back(std::move(force));
        }
    }
    
    ParticleForce* AdvancedParticleSystem::getForce(size_t index) const {
        if (index < m_forces.size()) {
            return m_forces[index].get();
        }
        return nullptr;
    }
    
    void AdvancedParticleSystem::addModifier(std::unique_ptr<ParticleModifier> modifier) {
        if (modifier) {
            m_modifiers.push_back(std::move(modifier));
        }
    }
    
    ParticleModifier* AdvancedParticleSystem::getModifier(size_t index) const {
        if (index < m_modifiers.size()) {
            return m_modifiers[index].get();
        }
        return nullptr;
    }
    
    void AdvancedParticleSystem::emit(int count) {
        spawnParticles(count);
    }
    
    void AdvancedParticleSystem::clear() {
        for (auto& particle : m_particles) {
            particle.active = false;
        }
    }
    
    size_t AdvancedParticleSystem::getActiveParticleCount() const {
        size_t count = 0;
        for (const auto& particle : m_particles) {
            if (particle.active) {
                count++;
            }
        }
        return count;
    }
    
    void AdvancedParticleSystem::play() {
        m_isPlaying = true;
        m_isPaused = false;
        m_isStopped = false;
        m_delayTimer = 0.0f;
        m_currentTime = 0.0f;
    }
    
    void AdvancedParticleSystem::stop() {
        m_isPlaying = false;
        m_isPaused = false;
        m_isStopped = true;
        clear();
    }
    
    void AdvancedParticleSystem::pause() {
        if (m_isPlaying) {
            m_isPaused = true;
        }
    }
    
    void AdvancedParticleSystem::resume() {
        if (m_isPlaying && m_isPaused) {
            m_isPaused = false;
        }
    }
    
    void AdvancedParticleSystem::setTexture(const std::string& texturePath) {
        m_texturePath = texturePath;
    }
    
    void AdvancedParticleSystem::updateParticles(float deltaTime) {
        // Update all active particles
        for (auto& particle : m_particles) {
            if (!particle.active) continue;
            
            // Update age
            particle.age += deltaTime;
            
            // Check if particle should die
            if (particle.age >= particle.lifetime) {
                particle.active = false;
                continue;
            }
            
            // Apply forces
            applyForces(deltaTime);
            
            // Apply modifiers
            applyModifiers(deltaTime);
            
            // Update velocity
            particle.velocity += particle.acceleration * deltaTime;
            
            // Update position
            particle.position += particle.velocity * deltaTime;
            
            // Update rotation
            particle.rotation += particle.angularVelocity * deltaTime;
            
            // Reset acceleration for next frame
            particle.acceleration = glm::vec3(0.0f);
        }
    }
    
    void AdvancedParticleSystem::updateEmission(float deltaTime) {
        // Update emission for all emitters
        for (const auto& emitter : m_emitters) {
            // Calculate how many particles to emit
            float particlesToEmit = emitter->getEmissionRate() * deltaTime;
            m_emissionTimer += particlesToEmit;
            
            if (m_emissionTimer >= 1.0f) {
                int count = static_cast<int>(m_emissionTimer);
                m_emissionTimer -= static_cast<float>(count);
                spawnParticles(count);
            }
        }
    }
    
    void AdvancedParticleSystem::applyForces(float deltaTime) {
        // Apply all forces to active particles
        for (auto& force : m_forces) {
            if (force->isEnabled()) {
                for (auto& particle : m_particles) {
                    if (particle.active) {
                        force->applyForce(particle, deltaTime);
                    }
                }
            }
        }
    }
    
    void AdvancedParticleSystem::applyModifiers(float deltaTime) {
        // Apply all modifiers to active particles
        for (auto& modifier : m_modifiers) {
            if (modifier->isEnabled()) {
                for (auto& particle : m_particles) {
                    if (particle.active) {
                        modifier->modifyParticle(particle, deltaTime);
                    }
                }
            }
        }
    }
    
    void AdvancedParticleSystem::spawnParticles(int count) {
        if (!owner) return;
        
        glm::vec3 emitterPosition = owner->getPosition();
        
        for (int i = 0; i < count; ++i) {
            Particle* particle = getFreeParticle();
            if (!particle) break;
            
            // Use the first emitter to initialize the particle
            if (!m_emitters.empty()) {
                m_emitters[0]->initializeParticle(*particle, emitterPosition);
            } else {
                // Default initialization
                particle->position = emitterPosition;
                particle->velocity = glm::vec3(0.0f, 1.0f, 0.0f);
                particle->acceleration = glm::vec3(0.0f);
                particle->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                particle->size = glm::vec2(0.1f, 0.1f);
                particle->rotation = 0.0f;
                particle->angularVelocity = 0.0f;
                particle->lifetime = 5.0f;
                particle->maxLifetime = 5.0f;
                particle->age = 0.0f;
                particle->active = true;
            }
        }
    }
    
    Particle* AdvancedParticleSystem::getFreeParticle() {
        // Find the first inactive particle
        for (auto& particle : m_particles) {
            if (!particle.active) {
                return &particle;
            }
        }
        return nullptr;
    }
}