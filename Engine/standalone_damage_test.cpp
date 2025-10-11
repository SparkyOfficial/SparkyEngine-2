#include <iostream>
#include <vector>
#include <string>
#include <algorithm>  // Add this for std::remove_if

// Standalone test for damage feedback concepts without engine dependencies

// Simple particle system simulation
struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float life;
    float maxLife;
    float r, g, b, a;
};

class SimpleParticleSystem {
private:
    std::vector<Particle> particles;
    
public:
    void createBloodEffect(float x, float y, float z, int count) {
        std::cout << "Creating " << count << " blood particles at (" << x << ", " << y << ", " << z << ")\n";
        for (int i = 0; i < count; i++) {
            Particle p;
            p.x = x;
            p.y = y;
            p.z = z;
            p.vx = (rand() % 100 - 50) / 100.0f; // Random velocity
            p.vy = (rand() % 100 - 50) / 100.0f;
            p.vz = (rand() % 100 - 50) / 100.0f;
            p.life = 1.0f;
            p.maxLife = 1.0f;
            p.r = 0.8f; p.g = 0.0f; p.b = 0.0f; p.a = 1.0f; // Red color
            particles.push_back(p);
        }
    }
    
    void createExplosion(float x, float y, float z) {
        std::cout << "Creating explosion effect at (" << x << ", " << y << ", " << z << ")\n";
        // Create many particles for explosion
        createBloodEffect(x, y, z, 50);
    }
    
    void update(float deltaTime) {
        // Update particle positions and life
        for (auto& p : particles) {
            p.x += p.vx * deltaTime;
            p.y += p.vy * deltaTime;
            p.z += p.vz * deltaTime;
            p.life -= deltaTime;
        }
        
        // Remove dead particles
        particles.erase(
            std::remove_if(particles.begin(), particles.end(), 
                          [](const Particle& p) { return p.life <= 0; }),
            particles.end()
        );
    }
    
    size_t getParticleCount() const {
        return particles.size();
    }
};

// Damage feedback system
class DamageFeedbackSystem {
private:
    SimpleParticleSystem particleSystem;
    float damageFlashTimer;
    float screenShakeTimer;
    
public:
    DamageFeedbackSystem() : damageFlashTimer(0), screenShakeTimer(0) {}
    
    void onDamageTaken(float damage, float dirX, float dirY, float dirZ) {
        std::cout << "Damage feedback triggered for " << damage << " damage\n";
        
        // Blood splatter effect
        int bloodCount = static_cast<int>(damage * 2);
        particleSystem.createBloodEffect(dirX, dirY, dirZ, bloodCount);
        
        // Screen flash effect
        damageFlashTimer = 0.2f;
        std::cout << "Screen flash effect activated\n";
        
        // Screen shake effect
        screenShakeTimer = 0.3f * (damage / 20.0f);
        std::cout << "Screen shake effect activated\n";
    }
    
    void onDeath() {
        std::cout << "Death feedback triggered\n";
        
        // Intense effects for death
        particleSystem.createExplosion(0, 0, 0);
        damageFlashTimer = 0.5f;
        screenShakeTimer = 1.0f;
        std::cout << "Intense death effects activated\n";
    }
    
    void update(float deltaTime) {
        particleSystem.update(deltaTime);
        
        if (damageFlashTimer > 0) {
            damageFlashTimer -= deltaTime;
        }
        
        if (screenShakeTimer > 0) {
            screenShakeTimer -= deltaTime;
        }
    }
    
    size_t getActiveParticleCount() const {
        return particleSystem.getParticleCount();
    }
};

// Simple health system
class HealthSystem {
private:
    float health;
    float maxHealth;
    DamageFeedbackSystem* feedback;
    
public:
    HealthSystem(float maxHealth, DamageFeedbackSystem* feedbackSystem) 
        : health(maxHealth), maxHealth(maxHealth), feedback(feedbackSystem) {}
    
    void takeDamage(float damage) {
        if (damage <= 0 || health <= 0) return;
        
        health -= damage;
        if (health < 0) health = 0;
        
        std::cout << "Health reduced to " << health << "/" << maxHealth << "\n";
        
        // Trigger damage feedback
        if (feedback) {
            feedback->onDamageTaken(damage, 0, 0, 1); // From front
        }
        
        if (health <= 0) {
            std::cout << "Entity died!\n";
            if (feedback) {
                feedback->onDeath();
            }
        }
    }
    
    void heal(float amount) {
        if (amount <= 0 || health <= 0) return;
        
        health += amount;
        if (health > maxHealth) health = maxHealth;
        
        std::cout << "Health increased to " << health << "/" << maxHealth << "\n";
    }
    
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    bool isAlive() const { return health > 0; }
};

int main() {
    std::cout << "Standalone Damage Feedback System Test\n";
    std::cout << "====================================\n\n";
    
    // Create damage feedback system
    DamageFeedbackSystem feedbackSystem;
    
    // Create health system with feedback
    HealthSystem player(100.0f, &feedbackSystem);
    
    std::cout << "Player created with " << player.getHealth() << " health\n\n";
    
    // Simulate combat
    std::cout << "--- Combat Simulation ---\n";
    
    // Player takes damage
    std::cout << "Player takes 15 damage:\n";
    player.takeDamage(15.0f);
    feedbackSystem.update(0.016f); // Update with ~60fps
    std::cout << "Active particles: " << feedbackSystem.getActiveParticleCount() << "\n\n";
    
    // Player takes more damage
    std::cout << "Player takes 25 damage:\n";
    player.takeDamage(25.0f);
    feedbackSystem.update(0.016f);
    std::cout << "Active particles: " << feedbackSystem.getActiveParticleCount() << "\n\n";
    
    // Player heals
    std::cout << "Player heals 20 health:\n";
    player.heal(20.0f);
    std::cout << "\n";
    
    // Player dies
    std::cout << "Player takes fatal damage:\n";
    player.takeDamage(85.0f);
    feedbackSystem.update(0.016f);
    std::cout << "Active particles: " << feedbackSystem.getActiveParticleCount() << "\n";
    std::cout << "Player alive: " << (player.isAlive() ? "Yes" : "No") << "\n\n";
    
    // Update feedback system to show particle effects
    std::cout << "Updating particle system for 2 seconds:\n";
    for (int i = 0; i < 120; i++) { // ~2 seconds at 60fps
        feedbackSystem.update(0.016f);
        if (i % 30 == 0) { // Print every 0.5 seconds
            std::cout << "Time " << (i * 0.016f) << "s - Active particles: " << feedbackSystem.getActiveParticleCount() << "\n";
        }
    }
    
    std::cout << "\nStandalone damage feedback test completed successfully!\n";
    std::cout << "In a full implementation, this would show visual effects like:\n";
    std::cout << "- Blood splatter particles\n";
    std::cout << "- Screen flash effects\n";
    std::cout << "- Screen shake effects\n";
    std::cout << "- Explosion effects for death\n";
    
    return 0;
}