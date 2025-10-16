#pragma once

#include "ParticleSystem.h"
#include "Component.h"
#include "GameObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <string>
#include <random>
#include <functional>

namespace Sparky {
    // Forward declarations
    class ParticleEmitter;
    class ParticleRenderer;
    class ParticleForce;
    class ParticleModifier;
    
    // Particle structure
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec4 color;
        glm::vec2 size;
        float rotation;
        float angularVelocity;
        float lifetime;
        float maxLifetime;
        float age;
        bool active;
        
        // UV animation
        glm::vec2 uvOffset;
        glm::vec2 uvScale;
        
        // Custom data
        glm::vec4 customData1;
        glm::vec4 customData2;
    };
    
    // Particle emitter types
    enum class EmitterType {
        POINT,
        SPHERE,
        BOX,
        CIRCLE,
        CONE,
        HEMISPHERE
    };
    
    // Particle blend modes
    enum class ParticleBlendMode {
        ALPHA,
        ADDITIVE,
        MULTIPLY,
        SCREEN,
        SUBTRACTIVE
    };
    
    // Particle force types
    enum class ForceType {
        GRAVITY,
        WIND,
        VORTEX,
        TURBULENCE,
        DRAG,
        CUSTOM
    };
    
    // Particle modifier types
    enum class ModifierType {
        COLOR_OVER_LIFETIME,
        SIZE_OVER_LIFETIME,
        ROTATION_OVER_LIFETIME,
        VELOCITY_OVER_LIFETIME,
        FORCE_OVER_LIFETIME,
        UV_ANIMATION,
        CUSTOM
    };
    
    // Particle force base class
    class ParticleForce {
    public:
        ParticleForce(ForceType type);
        virtual ~ParticleForce() = default;
        
        ForceType getType() const { return m_type; }
        void setEnabled(bool enabled) { m_enabled = enabled; }
        bool isEnabled() const { return m_enabled; }
        
        virtual void applyForce(Particle& particle, float deltaTime) = 0;
        
    protected:
        ForceType m_type;
        bool m_enabled;
    };
    
    // Gravity force
    class GravityForce : public ParticleForce {
    public:
        GravityForce(const glm::vec3& gravity = glm::vec3(0.0f, -9.81f, 0.0f));
        virtual ~GravityForce() = default;
        
        virtual void applyForce(Particle& particle, float deltaTime) override;
        
        void setGravity(const glm::vec3& gravity) { m_gravity = gravity; }
        const glm::vec3& getGravity() const { return m_gravity; }
        
    private:
        glm::vec3 m_gravity;
    };
    
    // Wind force
    class WindForce : public ParticleForce {
    public:
        WindForce(const glm::vec3& direction = glm::vec3(1.0f, 0.0f, 0.0f), float strength = 1.0f);
        virtual ~WindForce() = default;
        
        virtual void applyForce(Particle& particle, float deltaTime) override;
        
        void setDirection(const glm::vec3& direction) { m_direction = glm::normalize(direction); }
        const glm::vec3& getDirection() const { return m_direction; }
        
        void setStrength(float strength) { m_strength = strength; }
        float getStrength() const { return m_strength; }
        
    private:
        glm::vec3 m_direction;
        float m_strength;
    };
    
    // Vortex force
    class VortexForce : public ParticleForce {
    public:
        VortexForce(const glm::vec3& center = glm::vec3(0.0f), const glm::vec3& axis = glm::vec3(0.0f, 1.0f, 0.0f), 
                   float strength = 1.0f, float attraction = 1.0f);
        virtual ~VortexForce() = default;
        
        virtual void applyForce(Particle& particle, float deltaTime) override;
        
        void setCenter(const glm::vec3& center) { m_center = center; }
        const glm::vec3& getCenter() const { return m_center; }
        
        void setAxis(const glm::vec3& axis) { m_axis = glm::normalize(axis); }
        const glm::vec3& getAxis() const { return m_axis; }
        
        void setStrength(float strength) { m_strength = strength; }
        float getStrength() const { return m_strength; }
        
        void setAttraction(float attraction) { m_attraction = attraction; }
        float getAttraction() const { return m_attraction; }
        
    private:
        glm::vec3 m_center;
        glm::vec3 m_axis;
        float m_strength;
        float m_attraction;
    };
    
    // Turbulence force
    class TurbulenceForce : public ParticleForce {
    public:
        TurbulenceForce(float scale = 1.0f, float speed = 1.0f, float strength = 1.0f);
        virtual ~TurbulenceForce() = default;
        
        virtual void applyForce(Particle& particle, float deltaTime) override;
        
        void setScale(float scale) { m_scale = scale; }
        float getScale() const { return m_scale; }
        
        void setSpeed(float speed) { m_speed = speed; }
        float getSpeed() const { return m_speed; }
        
        void setStrength(float strength) { m_strength = strength; }
        float getStrength() const { return m_strength; }
        
    private:
        float m_scale;
        float m_speed;
        float m_strength;
        std::mt19937 m_randomGenerator;
        std::uniform_real_distribution<float> m_randomDistribution;
    };
    
    // Particle modifier base class
    class ParticleModifier {
    public:
        ParticleModifier(ModifierType type);
        virtual ~ParticleModifier() = default;
        
        ModifierType getType() const { return m_type; }
        void setEnabled(bool enabled) { m_enabled = enabled; }
        bool isEnabled() const { return m_enabled; }
        
        virtual void modifyParticle(Particle& particle, float deltaTime) = 0;
        
    protected:
        ModifierType m_type;
        bool m_enabled;
    };
    
    // Color over lifetime modifier
    class ColorOverLifetimeModifier : public ParticleModifier {
    public:
        ColorOverLifetimeModifier();
        virtual ~ColorOverLifetimeModifier() = default;
        
        virtual void modifyParticle(Particle& particle, float deltaTime) override;
        
        // Color gradient (simplified with start and end colors)
        void setColorRange(const glm::vec4& startColor, const glm::vec4& endColor);
        void setStartColor(const glm::vec4& color) { m_startColor = color; }
        void setEndColor(const glm::vec4& color) { m_endColor = color; }
        
    private:
        glm::vec4 m_startColor;
        glm::vec4 m_endColor;
    };
    
    // Size over lifetime modifier
    class SizeOverLifetimeModifier : public ParticleModifier {
    public:
        SizeOverLifetimeModifier();
        virtual ~SizeOverLifetimeModifier() = default;
        
        virtual void modifyParticle(Particle& particle, float deltaTime) override;
        
        void setSizeRange(const glm::vec2& startSize, const glm::vec2& endSize);
        void setStartSize(const glm::vec2& size) { m_startSize = size; }
        void setEndSize(const glm::vec2& size) { m_endSize = size; }
        
    private:
        glm::vec2 m_startSize;
        glm::vec2 m_endSize;
    };
    
    // Rotation over lifetime modifier
    class RotationOverLifetimeModifier : public ParticleModifier {
    public:
        RotationOverLifetimeModifier();
        virtual ~RotationOverLifetimeModifier() = default;
        
        virtual void modifyParticle(Particle& particle, float deltaTime) override;
        
        void setRotationRange(float startRotation, float endRotation);
        void setStartRotation(float rotation) { m_startRotation = rotation; }
        void setEndRotation(float rotation) { m_endRotation = rotation; }
        
    private:
        float m_startRotation;
        float m_endRotation;
    };
    
    // Particle emitter
    class ParticleEmitter {
    public:
        ParticleEmitter(EmitterType type = EmitterType::POINT);
        ~ParticleEmitter();
        
        // Emitter properties
        void setEmissionRate(float rate) { m_emissionRate = rate; }
        float getEmissionRate() const { return m_emissionRate; }
        
        void setBurstCount(int count) { m_burstCount = count; }
        int getBurstCount() const { return m_burstCount; }
        
        void setLifetimeRange(float minLifetime, float maxLifetime);
        void setMinLifetime(float minLifetime) { m_minLifetime = minLifetime; }
        void setMaxLifetime(float maxLifetime) { m_maxLifetime = maxLifetime; }
        
        void setSpeedRange(float minSpeed, float maxSpeed);
        void setMinSpeed(float minSpeed) { m_minSpeed = minSpeed; }
        void setMaxSpeed(float maxSpeed) { m_maxSpeed = maxSpeed; }
        
        void setSizeRange(const glm::vec2& minSize, const glm::vec2& maxSize);
        void setMinSize(const glm::vec2& minSize) { m_minSize = minSize; }
        void setMaxSize(const glm::vec2& maxSize) { m_maxSize = maxSize; }
        
        void setColorRange(const glm::vec4& minColor, const glm::vec4& maxColor);
        void setMinColor(const glm::vec4& minColor) { m_minColor = minColor; }
        void setMaxColor(const glm::vec4& maxColor) { m_maxColor = maxColor; }
        
        // Emitter shape properties
        void setEmitterType(EmitterType type) { m_type = type; }
        EmitterType getEmitterType() const { return m_type; }
        
        void setEmitterRadius(float radius) { m_radius = radius; }
        float getEmitterRadius() const { return m_radius; }
        
        void setEmitterSize(const glm::vec3& size) { m_size = size; }
        const glm::vec3& getEmitterSize() const { return m_size; }
        
        void setEmitterDirection(const glm::vec3& direction) { m_direction = glm::normalize(direction); }
        const glm::vec3& getEmitterDirection() const { return m_direction; }
        
        void setSpreadAngle(float angle) { m_spreadAngle = angle; }
        float getSpreadAngle() const { return m_spreadAngle; }
        
        // Burst emission
        void emitBurst(int count);
        
        // Particle initialization
        void initializeParticle(Particle& particle, const glm::vec3& emitterPosition) const;
        
    private:
        EmitterType m_type;
        float m_emissionRate;
        int m_burstCount;
        
        // Particle properties ranges
        float m_minLifetime;
        float m_maxLifetime;
        float m_minSpeed;
        float m_maxSpeed;
        glm::vec2 m_minSize;
        glm::vec2 m_maxSize;
        glm::vec4 m_minColor;
        glm::vec4 m_maxColor;
        
        // Emitter shape properties
        float m_radius;
        glm::vec3 m_size;
        glm::vec3 m_direction;
        float m_spreadAngle;
        
        // Random number generation
        mutable std::mt19937 m_randomGenerator;
        mutable std::uniform_real_distribution<float> m_randomDistribution;
        
        // Helper methods
        glm::vec3 generateRandomDirection() const;
        glm::vec3 generateRandomPosition(const glm::vec3& emitterPosition) const;
    };
    
    // Advanced particle system
    class AdvancedParticleSystem : public ParticleSystem, public Component {
    public:
        AdvancedParticleSystem(int maxParticles = 1000);
        virtual ~AdvancedParticleSystem();
        
        // Component interface
        virtual void initialize();
        virtual void update(float deltaTime) override;
        virtual void destroy();
        virtual void render() override;
        
        // Particle system properties
        void setMaxParticles(int maxParticles);
        int getMaxParticles() const { return m_maxParticles; }
        
        void setDuration(float duration) { m_duration = duration; }
        float getDuration() const { return m_duration; }
        
        void setLooping(bool looping) { m_looping = looping; }
        bool isLooping() const { return m_looping; }
        
        void setPlayOnAwake(bool playOnAwake) { m_playOnAwake = playOnAwake; }
        bool getPlayOnAwake() const { return m_playOnAwake; }
        
        void setStartDelay(float delay) { m_startDelay = delay; }
        float getStartDelay() const { return m_startDelay; }
        
        // Emitters
        void addEmitter(std::unique_ptr<ParticleEmitter> emitter);
        ParticleEmitter* getEmitter(size_t index) const;
        size_t getEmitterCount() const { return m_emitters.size(); }
        
        // Forces
        void addForce(std::unique_ptr<ParticleForce> force);
        ParticleForce* getForce(size_t index) const;
        size_t getForceCount() const { return m_forces.size(); }
        
        // Modifiers
        void addModifier(std::unique_ptr<ParticleModifier> modifier);
        ParticleModifier* getModifier(size_t index) const;
        size_t getModifierCount() const { return m_modifiers.size(); }
        
        // Particle management
        void emit(int count = 1);
        void clear();
        size_t getActiveParticleCount() const;
        
        // Playback control
        void play();
        void stop();
        void pause();
        void resume();
        
        bool isPlaying() const { return m_isPlaying; }
        bool isPaused() const { return m_isPaused; }
        float getCurrentTime() const { return m_currentTime; }
        
        // Rendering properties
        void setBlendMode(ParticleBlendMode blendMode) { m_blendMode = blendMode; }
        ParticleBlendMode getBlendMode() const { return m_blendMode; }
        
        void setTexture(const std::string& texturePath);
        const std::string& getTexture() const { return m_texturePath; }
        
        void setUseWorldSpace(bool useWorldSpace) { m_useWorldSpace = useWorldSpace; }
        bool getUseWorldSpace() const { return m_useWorldSpace; }
        
        // Particle access
        const std::vector<Particle>& getParticles() const { return m_particles; }
        std::vector<Particle>& getParticles() { return m_particles; }
        
    private:
        // Particle storage
        std::vector<Particle> m_particles;
        int m_maxParticles;
        
        // System properties
        float m_duration;
        bool m_looping;
        bool m_playOnAwake;
        float m_startDelay;
        float m_currentTime;
        float m_delayTimer;
        
        // Playback state
        bool m_isPlaying;
        bool m_isPaused;
        bool m_isStopped;
        
        // Components
        std::vector<std::unique_ptr<ParticleEmitter>> m_emitters;
        std::vector<std::unique_ptr<ParticleForce>> m_forces;
        std::vector<std::unique_ptr<ParticleModifier>> m_modifiers;
        
        // Rendering properties
        ParticleBlendMode m_blendMode;
        std::string m_texturePath;
        bool m_useWorldSpace;
        
        // Emission tracking
        float m_emissionTimer;
        
        // Internal methods
        void updateParticles(float deltaTime);
        void updateEmission(float deltaTime);
        void applyForces(float deltaTime);
        void applyModifiers(float deltaTime);
        void spawnParticles(int count);
        Particle* getFreeParticle();
    };
}