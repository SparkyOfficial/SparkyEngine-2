#include "Gun.h"
#include "GunImpl.h"
#include <string>
#include <memory>
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/PhysicsWorld.h"
#include "../../Engine/include/RigidBodyComponent.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/ParticleComponent.h"
#include <string>
#include <memory>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

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

    Gun::Gun() : GameObject(), impl(new GunImpl()) {
        setName("Gun");
        
        // Add a particle component for muzzle flash effects
        auto particleComponent = addComponent<ParticleComponent>();
        auto particleSystem = std::make_unique<ParticleSystem>();
        
        // Configure the particle system for muzzle flashes
        EmitterProperties props;
        props.lifetime = 0.1f;
        props.startColor[0] = 1.0f; props.startColor[1] = 1.0f; props.startColor[2] = 0.0f; props.startColor[3] = 1.0f; // Yellow
        props.endColor[0] = 1.0f; props.endColor[1] = 0.5f; props.endColor[2] = 0.0f; props.endColor[3] = 0.0f;   // Orange fading
        props.startSize = 0.05f;
        props.endSize = 0.0f;
        props.emissionRate = 0.0f; // No continuous emission
        props.gravity[0] = 0.0f; props.gravity[1] = 0.0f; props.gravity[2] = 0.0f;
        particleSystem->setEmitterProperties(props);
        
        particleComponent->setParticleSystem(std::move(particleSystem));
        
        // Add an audio component for sound effects
#ifdef ENABLE_AUDIO
        auto audioComponent = addComponent<AudioComponent>();
        // Pre-load gunshot sound
        audioComponent->loadSound("gunshot", "gunshot.wav");
#endif

        SPARKY_LOG_INFO("Gun created with " + std::to_string(impl->getAmmo()) + " ammo");
    }

    Gun::~Gun() {
        delete impl;
    }

    void Gun::update(float deltaTime) {
        Sparky::GameObject::update(deltaTime);
        impl->update(deltaTime);
        
        // Handle shooting input
        InputManager& inputManager = InputManager::getInstance();
#ifdef HAS_GLFW
        if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && canShoot()) {
            shoot();
        }
        
        if (inputManager.isKeyPressed(GLFW_KEY_R) && !impl->isReloading && impl->getAmmo() < impl->getMagazineSize() && impl->getTotalAmmo() > 0) {
            reload();
        }
#else
        // Fallback implementation for non-GLFW builds
        // For now, we'll just skip shooting and reloading
#endif
    }

    void Gun::render() {
        Sparky::GameObject::render();
        
        // In a full implementation, we would render the gun model here
        // For now, we'll just log that we're rendering
        // SPARKY_LOG_DEBUG("Rendering gun");
    }

    void Gun::shoot() {
        impl->shoot();
    }

    void Gun::reload() {
        impl->reload();
    }
    
    void Gun::aim() {
        impl->aim();
    }
    
    void Gun::unaim() {
        impl->unaim();
    }

    bool Gun::canShoot() const {
        return impl->canShoot();
    }
    
    void Gun::applyRecoil() {
        impl->applyRecoil();
    }
    
    // Burst fire control
    void Gun::startBurst() {
        impl->startBurst();
    }
    
    void Gun::updateBurst(float deltaTime) {
        impl->updateBurst(deltaTime);
    }

    // Getters and setters
    int Gun::getAmmo() const { return impl->getAmmo(); }
    int Gun::getMagazineSize() const { return impl->getMagazineSize(); }
    int Gun::getTotalAmmo() const { return impl->getTotalAmmo(); }
    float Gun::getFireRate() const { return impl->getFireRate(); }
    float Gun::getSpread() const { return impl->getSpread(); }
    float Gun::getDamage() const { return impl->getDamage(); }
    bool Gun::isAiming() const { return impl->getIsAiming(); }
    int Gun::getFiringMode() const { return impl->getFiringMode(); }
    int Gun::getAmmoType() const { return impl->getAmmoType(); }
    
    void Gun::setAmmo(int ammo) { impl->setAmmo(ammo); }
    void Gun::setMagazineSize(int size) { impl->setMagazineSize(size); }
    void Gun::setTotalAmmo(int ammo) { impl->setTotalAmmo(ammo); }
    void Gun::setFireRate(float rate) { impl->setFireRate(rate); }
    void Gun::setSpread(float sp) { impl->setSpread(sp); }
    void Gun::setDamage(float dmg) { impl->setDamage(dmg); }
    void Gun::setFiringMode(int mode) { impl->setFiringMode(mode); }
    void Gun::setAmmoType(int type) { impl->setAmmoType(type); }
    
    void Gun::setCamera(Camera* camera) { impl->setCamera(camera); }
    Camera* Gun::getCamera() const { return impl->getCamera(); }
    
    // Advanced weapon properties
    float Gun::getRecoil() const { return impl->getRecoil(); }
    void Gun::setRecoil(float recoil) { impl->setRecoil(recoil); }
    
    float Gun::getRecoilRecovery() const { return impl->getRecoilRecovery(); }
    void Gun::setRecoilRecovery(float recovery) { impl->setRecoilRecovery(recovery); }
    
    float Gun::getMuzzleVelocity() const { return impl->getMuzzleVelocity(); }
    void Gun::setMuzzleVelocity(float velocity) { impl->setMuzzleVelocity(velocity); }
    
    const char* Gun::getWeaponType() const { return impl->getWeaponType(); }
    void Gun::setWeaponType(const char* type) { impl->setWeaponType(type); }
    
    // Recoil pattern system
    void Gun::getRecoilPattern(float* pattern) const { impl->getRecoilPattern(pattern); }
    void Gun::setRecoilPattern(const float* pattern) { impl->setRecoilPattern(pattern); }
    
    // Weapon attachments
    void Gun::addAttachment(const char* name, float modifier, bool active) {
        impl->addAttachment(std::string(name), modifier, active);
    }
    
    void Gun::removeAttachment(const char* name) {
        impl->removeAttachment(std::string(name));
    }
    
    void Gun::toggleAttachment(const char* name) {
        impl->toggleAttachment(std::string(name));
    }
    
    int Gun::getAttachmentCount() const { return impl->getAttachmentCount(); }
    
    // Weapon statistics
    void Gun::getStats(int* stats) const { impl->getStats(stats); }
    void Gun::resetStats() { impl->resetStats(); }
    
    // Heat and wear system
    float Gun::getHeat() const { return impl->getHeat(); }
    float Gun::getWear() const { return impl->getWear(); }
    bool Gun::isOverheated() const { return impl->isOverheated(); }
    void Gun::coolDown(float deltaTime) { impl->coolDown(deltaTime); }
    
    // Jamming system
    bool Gun::isJammed() const { return impl->isJammed(); }
    void Gun::jam() { impl->jam(); }
    void Gun::unjam() { impl->unjam(); }
    
    // Reload mechanics
    float Gun::getReloadTime() const { return impl->getReloadTime(); }
    void Gun::setReloadTime(float time) { impl->setReloadTime(time); }
    
    // Burst fire settings
    int Gun::getBurstCount() const { return impl->getBurstCount(); }
    void Gun::setBurstCount(int count) { impl->setBurstCount(count); }
    int Gun::getCurrentBurstShot() const { return impl->getCurrentBurstShot(); }
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
        if (impl->getCamera()) {
            AudioEngine::getInstance().playGunshotSound(impl->getCamera()->getPosition());
        }
    }
}
#else
void Sparky::Gun::playShootSound() {
    // Audio disabled - do nothing
}
#endif