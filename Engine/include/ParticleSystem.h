#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace Sparky {
    
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 color;
        float life;
        float maxLife;
        float size;
        
        Particle();
    };
    
    class ParticleSystem {
    public:
        ParticleSystem();
        ~ParticleSystem();
        
        void update(float deltaTime);
        void render();
        
        // Configuration methods
        void setMaxParticles(int max) { maxParticles = max; particles.resize(max); }
        void setEmissionPoint(const glm::vec3& point);
        void setEmissionDirection(const glm::vec3& direction);
        void setEmissionSpread(float spread);
        void setEmissionSpeed(float speed);
        void setEmissionRate(float rate);
        void setParticleLifetime(float lifetime);
        void setStartColor(const glm::vec4& color);
        void setEndColor(const glm::vec4& color);
        void setStartSize(float size);
        void setEndSize(float size);
        void setGravity(const glm::vec3& grav);
        void setActive(bool active);
        
        // Getters
        bool isActive() const;
        int getParticleCount() const;
        int getMaxParticles() const { return maxParticles; }
        float getEmissionRate() const { return emissionRate; }
        float getParticleLifetime() const { return particleLifetime; }
        
        // Preset particle effects
        void createMuzzleFlash(const glm::vec3& position, const glm::vec3& direction);
        void createExplosion(const glm::vec3& position);
        void createSmoke(const glm::vec3& position);

    private:
        std::vector<Particle> particles;
        int maxParticles;
        int particleCount;
        
        // Emission properties
        glm::vec3 emissionPoint;
        glm::vec3 emissionDirection;
        float emissionSpread;
        float emissionSpeed;
        float emissionRate;
        float lastEmissionTime;
        
        // Particle properties
        float particleLifetime;
        glm::vec4 startColor;
        glm::vec4 endColor;
        float startSize;
        float endSize;
        glm::vec3 gravity;
        
        bool systemActive;
        
        // Private helper methods
        void emitParticles(float deltaTime);
        void emitParticle();
    };
}