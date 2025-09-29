#include "Gun.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/PhysicsWorld.h"
#include "../../Engine/include/RigidBodyComponent.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/Mesh.h"

// Only include AudioEngine if audio is enabled
#ifdef ENABLE_AUDIO
#include "../../Engine/include/AudioEngine.h"
#include "../../Engine/include/AudioComponent.h"
#endif

#include "../../Engine/include/ParticleComponent.h"
#include "../../Engine/include/ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <random>

namespace Sparky {

    Gun::Gun() : GameObject(), camera(nullptr), currentAmmo(30), magazineSize(30), 
                totalAmmo(120), fireRate(10.0f), spread(1.0f), damage(10.0f),
                lastShotTime(0.0f), isReloading(false), reloadTime(2.0f), lastReloadTime(0.0f),
                recoil(0.5f), recoilRecovery(1.0f), currentRecoil(0.0f),
                muzzleVelocity(500.0f), weaponType("Assault Rifle") {
        setName("Gun");
        
        // Add a particle component for muzzle flash effects
        auto particleComponent = addComponent<ParticleComponent>();
        auto particleSystem = std::make_unique<ParticleSystem>();
        
        // Configure the particle system for muzzle flashes
        particleSystem->setParticleLifetime(0.1f);
        particleSystem->setStartColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow
        particleSystem->setEndColor(glm::vec4(1.0f, 0.5f, 0.0f, 0.0f));   // Orange fading
        particleSystem->setStartSize(0.05f);
        particleSystem->setEndSize(0.0f);
        particleSystem->setEmissionRate(0.0f); // No continuous emission
        particleSystem->setGravity(glm::vec3(0.0f));
        
        particleComponent->setParticleSystem(std::move(particleSystem));
        
        // Add an audio component for sound effects
#ifdef ENABLE_AUDIO
        auto audioComponent = addComponent<AudioComponent>();
        // Pre-load gunshot sound
        audioComponent->loadSound("gunshot", "gunshot.wav");
#endif

        SPARKY_LOG_INFO("Gun created with " + std::to_string(currentAmmo) + " ammo");
    }

    Gun::~Gun() {
    }

    void Gun::update(float deltaTime) {
        GameObject::update(deltaTime);
        
        // Handle reloading
        if (isReloading) {
            if (glfwGetTime() - lastReloadTime >= reloadTime) {
                isReloading = false;
                int ammoNeeded = magazineSize - currentAmmo;
                int ammoToLoad = std::min(ammoNeeded, totalAmmo);
                currentAmmo += ammoToLoad;
                totalAmmo -= ammoToLoad;
                SPARKY_LOG_INFO("Reloaded. Current ammo: " + std::to_string(currentAmmo));
            }
        }
        
        // Handle recoil recovery
        if (currentRecoil > 0.0f) {
            currentRecoil -= recoilRecovery * deltaTime;
            if (currentRecoil < 0.0f) {
                currentRecoil = 0.0f;
            }
        }
        
        // Handle shooting input
        InputManager& inputManager = InputManager::getInstance();
        if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && canShoot()) {
            shoot();
        }
        
        if (inputManager.isKeyPressed(GLFW_KEY_R) && !isReloading && currentAmmo < magazineSize && totalAmmo > 0) {
            reload();
        }
    }

    void Gun::render() {
        GameObject::render();
        
        // In a full implementation, we would render the gun model here
        // For now, we'll just log that we're rendering
        // SPARKY_LOG_DEBUG("Rendering gun");
    }

    void Gun::shoot() {
        if (!canShoot() || !camera) {
            return;
        }
        
        // Update last shot time
        lastShotTime = static_cast<float>(glfwGetTime());
        
        // Decrease ammo
        currentAmmo--;
        
        // Apply recoil
        currentRecoil += recoil;
        
        // Calculate shot direction with spread and recoil
        glm::vec3 shotDirection = calculateSpreadDirection();
        
        // Apply recoil to camera
        if (camera) {
            // Add some randomness to the recoil direction
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(-0.5f, 0.5f);
            
            float recoilYaw = dis(gen) * currentRecoil;
            float recoilPitch = -currentRecoil; // Recoil pushes the gun up
            
            camera->ProcessMouseMovement(recoilYaw, recoilPitch);
        }
        
        // Create a bullet object
        createBullet(shotDirection);
        
        // Play shooting sound
        playShootSound();
        
        // Create muzzle flash effect
        createMuzzleFlash(shotDirection);
        
        // Log the shot
        SPARKY_LOG_INFO("Gun fired! Ammo: " + std::to_string(currentAmmo) + 
                       ", Direction: (" + std::to_string(shotDirection.x) + ", " + 
                       std::to_string(shotDirection.y) + ", " + std::to_string(shotDirection.z) + ")");
    }

    void Gun::reload() {
        if (isReloading || currentAmmo >= magazineSize || totalAmmo <= 0) {
            return;
        }
        
        isReloading = true;
        lastReloadTime = static_cast<float>(glfwGetTime());
        SPARKY_LOG_INFO("Reloading weapon...");
    }

    bool Gun::canShoot() const {
        // Check if we have ammo and are not reloading
        if (currentAmmo <= 0 || isReloading) {
            return false;
        }
        
        // Check fire rate cooldown
        float currentTime = static_cast<float>(glfwGetTime());
        float timeSinceLastShot = currentTime - lastShotTime;
        float fireInterval = 1.0f / fireRate;
        
        return timeSinceLastShot >= fireInterval;
    }

    glm::vec3 Gun::calculateSpreadDirection() const {
        if (!camera) {
            return glm::vec3(0.0f, 0.0f, -1.0f);
        }
        
        // Get the base direction from the camera
        glm::vec3 direction = camera->Front;
        
        // Add spread by randomly perturbing the direction
        if (spread > 0.0f) {
            // Create a random number generator
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
            
            // Generate random offsets
            float offsetX = dis(gen) * spread * 0.01f;
            float offsetY = dis(gen) * spread * 0.01f;
            
            // Apply the offsets
            direction += glm::vec3(offsetX, offsetY, 0.0f);
            direction = glm::normalize(direction);
        }
        
        return direction;
    }
    
    void Gun::createBullet(const glm::vec3& direction) {
        // Create a bullet object
        auto bullet = std::make_unique<GameObject>("Bullet");
        bullet->setPosition(camera->getPosition() + camera->Front * 0.5f); // Start slightly in front of camera
        
        // Add a render component
        auto renderComponent = bullet->addComponent<RenderComponent>();
        auto bulletMesh = Mesh::createSphere(0.02f, 8, 8); // Small sphere for bullet
        renderComponent->setMesh(std::move(bulletMesh));
        
        // Add a rigid body component for physics
        auto rigidBody = bullet->addComponent<RigidBodyComponent>();
        rigidBody->setMass(0.01f); // Very light
        rigidBody->setVelocity(direction * muzzleVelocity);
        rigidBody->setAffectedByGravity(false); // Bullets aren't affected by gravity in most FPS games
        
        // Register with physics world
        PhysicsWorld::getInstance().addRigidBody(rigidBody);
        
        // In a full implementation, we would add the bullet to the game world
        // and set up collision detection for it to hit other objects
        
        SPARKY_LOG_DEBUG("Bullet created with velocity: " + 
                        std::to_string(direction.x * muzzleVelocity) + ", " + 
                        std::to_string(direction.y * muzzleVelocity) + ", " + 
                        std::to_string(direction.z * muzzleVelocity));
    }
    
    void Gun::createMuzzleFlash(const glm::vec3& direction) {
        // Get the particle component
        ParticleComponent* particleComponent = getComponent<ParticleComponent>();
        if (particleComponent) {
            // Emit a muzzle flash in the direction the gun is pointing
            particleComponent->emitMuzzleFlash(direction);
        }
    }
    
    // Getters and setters for new properties
    float Gun::getRecoil() const { return recoil; }
    void Gun::setRecoil(float rec) { recoil = rec; }
    
    float Gun::getRecoilRecovery() const { return recoilRecovery; }
    void Gun::setRecoilRecovery(float recovery) { recoilRecovery = recovery; }
    
    float Gun::getMuzzleVelocity() const { return muzzleVelocity; }
    void Gun::setMuzzleVelocity(float velocity) { muzzleVelocity = velocity; }
    
    const std::string& Gun::getWeaponType() const { return weaponType; }
    void Gun::setWeaponType(const std::string& type) { weaponType = type; }
}

#ifdef ENABLE_AUDIO
void Sparky::Gun::playShootSound() {
    // Get the audio component
    AudioComponent* audioComponent = getComponent<AudioComponent>();
    if (audioComponent) {
        // Play the gunshot sound
        audioComponent->playGunshot();
    } else {
        // Fallback to global audio engine
        if (camera) {
            AudioEngine::getInstance().playGunshotSound(camera->getPosition());
        }
    }
}
#else
void Sparky::Gun::playShootSound() {
    // Audio disabled - do nothing
}
#endif
