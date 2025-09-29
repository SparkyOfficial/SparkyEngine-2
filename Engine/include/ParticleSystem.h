#pragma once

#include "Component.h"
#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace Sparky {
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec4 color;
        float life;
        float maxLife;
        float size;
        bool active;
    };

    class ParticleSystem : public Component {
    public:
        ParticleSystem();
        virtual ~ParticleSystem();

        void update(float deltaTime) override;
        void render() override;

        // Particle system configuration
        void setEmissionRate(float rate);
        void setParticleLifetime(float min, float max);
        void setStartColor(const glm::vec4& color);
        void setEndColor(const glm::vec4& color);
        void setStartSize(float size);
        void setEndSize(float size);
        void setStartSpeed(float min, float max);
        void setGravity(const glm::vec3& gravity);
        
        // Emission shape
        void setEmissionShapeSphere(float radius);
        void setEmissionShapeBox(const glm::vec3& size);
        
        // Control
        void play();
        void pause();
        void stop();
        void reset();

        bool isPlaying() const { return playing; }
        size_t getActiveParticleCount() const { return activeParticleCount; }

    private:
        std::vector<Particle> particles;
        size_t activeParticleCount;
        
        // Configuration
        float emissionRate;
        float emissionAccumulator;
        float minLifetime, maxLifetime;
        glm::vec4 startColor, endColor;
        float startSize, endSize;
        float minSpeed, maxSpeed;
        glm::vec3 gravity;
        
        // Emission shape
        enum class EmissionShape { SPHERE, BOX };
        EmissionShape emissionShape;
        float sphereRadius;
        glm::vec3 boxSize;
        
        // State
        bool playing;
        bool loop;
        
        // Helper methods
        void emitParticles(float deltaTime);
        void updateParticles(float deltaTime);
        void initializeParticle(Particle& particle);
        glm::vec3 getRandomEmissionPosition() const;
        glm::vec3 getRandomEmissionVelocity() const;
    };
}