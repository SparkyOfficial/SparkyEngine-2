/*
 * AdvancedParticleSystem.h
 * 
 * Author: Андрій Будильников
 * 
 * Advanced particle system with enhanced features
 */

#pragma once

#include "ParticleSystem.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Sparky {
    
    // Advanced particle properties with more realistic physics
    struct AdvancedParticleProperties : public ParticleProperties {
        float angularVelocity;        // Angular velocity for rotation
        float dragCoefficient;        // Drag coefficient for air resistance
        float buoyancy;              // Buoyancy factor
        float temperature;           // Particle temperature
        float heatTransferRate;      // Heat transfer rate
        float elasticity;            // Bounciness (0.0 to 1.0)
        float friction;              // Friction coefficient
        float charge;                // Electric charge
        bool collidesWithWorld;      // Whether particle collides with world geometry
        bool affectedByWind;         // Whether particle is affected by wind
        float windFactor;            // How much wind affects the particle (0.0 to 1.0)
        
        AdvancedParticleProperties();
    };
    
    // Advanced emitter properties with more control
    struct AdvancedEmitterProperties : public EmitterProperties {
        float minSpeed;              // Minimum emission speed
        float maxSpeed;              // Maximum emission speed
        float minLifetime;           // Minimum particle lifetime
        float maxLifetime;           // Maximum particle lifetime
        float minStartSize;          // Minimum start size
        float maxStartSize;          // Maximum start size
        float minEndSize;            // Minimum end size
        float maxEndSize;            // Maximum end size
        float minRotationSpeed;      // Minimum rotation speed
        float maxRotationSpeed;      // Maximum rotation speed
        float minMass;               // Minimum mass
        float maxMass;               // Maximum mass
        float emissionArea[3];       // Emission area dimensions (width, height, depth)
        bool useVolumeEmission;      // Whether to emit from volume or surface
        float windInfluence;         // How much wind affects emitted particles (0.0 to 1.0)
        float turbulence;            // Turbulence factor (0.0 to 1.0)
        float turbulenceScale;       // Turbulence scale
        float turbulenceSpeed;       // Turbulence speed
        bool useTurbulence;          // Whether to apply turbulence
        float airDensity;            // Air density for drag calculations
        float gravityScale;          // Gravity scale factor
        bool inheritEmitterVelocity; // Whether particles inherit emitter velocity
        float velocityInheritance;   // How much velocity to inherit (0.0 to 1.0)
        bool affectedByWind;         // Whether particles are affected by wind
        float buoyancy;              // Buoyancy factor
        
        AdvancedEmitterProperties();
    };
    
    // Particle collision data
    struct ParticleCollision {
        int particleIndex;
        glm::vec3 collisionPoint;
        glm::vec3 collisionNormal;
        float collisionImpulse;
        bool resolved;
        
        ParticleCollision();
    };
    
    // Particle force field
    struct ForceField {
        glm::vec3 position;
        float radius;
        glm::vec3 force;
        float strength;
        float falloff;
        bool enabled;
        
        ForceField();
    };
    
    // Particle attractor
    struct ParticleAttractor {
        glm::vec3 position;
        float radius;
        float strength;
        float falloff;
        bool enabled;
        
        ParticleAttractor();
    };
    
    // Wind zone
    struct WindZone {
        glm::vec3 position;
        glm::vec3 direction;
        float radius;
        float speed;
        float turbulence;
        bool enabled;
        
        WindZone();
    };
    
    // Particle sub-emitter
    struct SubEmitter {
        int parentParticleType;      // Particle type that triggers sub-emission
        EmitterProperties properties; // Emitter properties for sub-emission
        int emitCount;               // Number of particles to emit per trigger
        bool enabled;
        
        SubEmitter();
    };
    
    class AdvancedParticleSystem : public ParticleSystem {
    public:
        AdvancedParticleSystem();
        ~AdvancedParticleSystem();
        
        void update(float deltaTime) override;
        void render() override;
        
        // Advanced configuration methods
        void setAdvancedEmitterProperties(const AdvancedEmitterProperties& props);
        const AdvancedEmitterProperties& getAdvancedEmitterProperties() const;
        
        // Force fields
        void addForceField(const ForceField& field);
        void removeForceField(int index);
        void clearForceFields();
        const std::vector<ForceField>& getForceFields() const;
        
        // Attractors
        void addAttractor(const ParticleAttractor& attractor);
        void removeAttractor(int index);
        void clearAttractors();
        const std::vector<ParticleAttractor>& getAttractors() const;
        
        // Wind zones
        void addWindZone(const WindZone& windZone);
        void removeWindZone(int index);
        void clearWindZones();
        const std::vector<WindZone>& getWindZones() const;
        
        // Sub-emitters
        void addSubEmitter(const SubEmitter& subEmitter);
        void removeSubEmitter(int index);
        void clearSubEmitters();
        const std::vector<SubEmitter>& getSubEmitters() const;
        
        // Collision system
        void setCollisionEnabled(bool enabled);
        bool isCollisionEnabled() const;
        void setCollisionBounds(const glm::vec3& min, const glm::vec3& max);
        void getCollisionBounds(glm::vec3& min, glm::vec3& max) const;
        
        // Advanced particle emission
        void emitAdvancedParticle();
        void emitAdvancedBurst(int count);
        
        // Advanced preset effects
        void createAdvancedExplosion(float x, float y, float z, float intensity = 1.0f, float radius = 5.0f);
        void createAdvancedSmoke(float x, float y, float z, float density = 1.0f, float height = 3.0f);
        void createAdvancedFire(float x, float y, float z, float intensity = 1.0f, float height = 2.0f);
        void createAdvancedSparks(float x, float y, float z, int count = 10, float spread = 1.0f);
        void createAdvancedBlood(float x, float y, float z, float dirX, float dirY, float dirZ, int count = 5);
        void createAdvancedMist(float x, float y, float z, float density = 1.0f, float spread = 2.0f);
        void createAdvancedTrail(float startX, float startY, float startZ, float endX, float endY, float endZ, float duration = 1.0f);
        void createAdvancedShockwave(float x, float y, float z, float radius = 5.0f, float intensity = 1.0f);
        void createAdvancedLightning(float startX, float startY, float startZ, float endX, float endY, float endZ, int branches = 5);
        void createAdvancedRain(float x, float y, float z, float areaWidth = 10.0f, float areaHeight = 10.0f, float intensity = 1.0f);
        void createAdvancedSnow(float x, float y, float z, float areaWidth = 10.0f, float areaHeight = 10.0f, float intensity = 1.0f);
        
        // Effect combinations
        void createAdvancedExplosionWithSmoke(float x, float y, float z, float intensity = 1.0f, float radius = 5.0f);
        void createAdvancedFireWithSmoke(float x, float y, float z, float intensity = 1.0f, float height = 2.0f);
        void createAdvancedVolcanicEruption(float x, float y, float z, float intensity = 1.0f);

    private:
        // Advanced particle data
        std::vector<AdvancedParticleProperties> advancedParticles;
        AdvancedEmitterProperties advancedEmitterProps;
        
        // Force fields, attractors, wind zones
        std::vector<ForceField> forceFields;
        std::vector<ParticleAttractor> attractors;
        std::vector<WindZone> windZones;
        std::vector<SubEmitter> subEmitters;
        
        // Collision system
        bool collisionEnabled;
        glm::vec3 collisionBoundsMin;
        glm::vec3 collisionBoundsMax;
        std::vector<ParticleCollision> collisions;
        
        // Private helper methods
        void updateAdvancedParticle(AdvancedParticleProperties& particle, float deltaTime);
        void applyAdvancedPhysics(AdvancedParticleProperties& particle, float deltaTime);
        void applyForceFields(AdvancedParticleProperties& particle, float deltaTime);
        void applyAttractors(AdvancedParticleProperties& particle, float deltaTime);
        void applyWindZones(AdvancedParticleProperties& particle, float deltaTime);
        void handleCollisions(AdvancedParticleProperties& particle, float deltaTime);
        void handleSubEmitters(AdvancedParticleProperties& particle, float deltaTime);
        glm::vec3 calculateTurbulence(const glm::vec3& position, float time);
        glm::vec3 calculateWindEffect(const glm::vec3& position, const glm::vec3& velocity);
        
        // Override base class methods
        void updateParticle(ParticleProperties& particle, float deltaTime) override;
        void applyPhysics(ParticleProperties& particle, float deltaTime) override;
    };
}