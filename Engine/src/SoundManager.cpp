#ifdef ENABLE_AUDIO
#include "../include/SoundManager.h"
#include "../include/Logger.h"
#include <AL/al.h>
#include <iostream>
#include <algorithm>

namespace Sparky {
    
    SoundManager& SoundManager::getInstance() {
        static SoundManager instance;
        return instance;
    }
    
    SoundManager::SoundManager() : audioEngine(&AudioEngine::getInstance()), 
                                  masterVolume(1.0f), musicVolume(0.7f) {
        // Initialize category volumes
        categoryVolumes[SoundCategory::WEAPON] = 1.0f;
        categoryVolumes[SoundCategory::ENEMY] = 1.0f;
        categoryVolumes[SoundCategory::ENVIRONMENT] = 1.0f;
        categoryVolumes[SoundCategory::PLAYER] = 1.0f;
        categoryVolumes[SoundCategory::UI] = 1.0f;
        categoryVolumes[SoundCategory::MUSIC] = 1.0f;
        
        // Initialize category mute states
        categoryMuted[SoundCategory::WEAPON] = false;
        categoryMuted[SoundCategory::ENEMY] = false;
        categoryMuted[SoundCategory::ENVIRONMENT] = false;
        categoryMuted[SoundCategory::PLAYER] = false;
        categoryMuted[SoundCategory::UI] = false;
        categoryMuted[SoundCategory::MUSIC] = false;
        
        Logger::getInstance().info("SoundManager created");
    }
    
    SoundManager::~SoundManager() {
        cleanup();
        Logger::getInstance().info("SoundManager destroyed");
    }
    
    bool SoundManager::initialize() {
        Logger::getInstance().info("Initializing SoundManager");
        
        if (!audioEngine->initialize()) {
            Logger::getInstance().error("Failed to initialize AudioEngine");
            return false;
        }
        
        // Load default sounds
        loadDefaultSounds();
        
        Logger::getInstance().info("SoundManager initialized successfully");
        return true;
    }
    
    void SoundManager::cleanup() {
        Logger::getInstance().info("Cleaning up SoundManager");
        stopAllSounds();
        audioEngine->cleanup();
    }
    
    // Weapon sounds
    void SoundManager::playWeaponSound(WeaponSoundType type, const glm::vec3& position) {
        if (categoryMuted[SoundCategory::WEAPON]) return;
        
        std::string soundName = getWeaponSoundName(type);
        ALuint source = audioEngine->playSound(soundName);
        if (source != 0) {
            audioEngine->setSoundPosition(source, position);
            setSoundVolume(source, categoryVolumes[SoundCategory::WEAPON]);
            activeSources.push_back(source);
        }
    }
    
    void SoundManager::playGunshotSound(const glm::vec3& position) {
        playWeaponSound(WeaponSoundType::SHOOT, position);
    }
    
    void SoundManager::playReloadSound(const glm::vec3& position) {
        playWeaponSound(WeaponSoundType::RELOAD, position);
    }
    
    void SoundManager::playWeaponEmptySound(const glm::vec3& position) {
        playWeaponSound(WeaponSoundType::EMPTY, position);
    }
    
    void SoundManager::playWeaponPickupSound(const glm::vec3& position) {
        playWeaponSound(WeaponSoundType::PICKUP, position);
    }
    
    void SoundManager::playWeaponDropSound(const glm::vec3& position) {
        playWeaponSound(WeaponSoundType::DROP, position);
    }
    
    // Enemy sounds
    void SoundManager::playEnemySound(EnemySoundType type, const glm::vec3& position) {
        if (categoryMuted[SoundCategory::ENEMY]) return;
        
        std::string soundName = getEnemySoundName(type);
        ALuint source = audioEngine->playSound(soundName);
        if (source != 0) {
            audioEngine->setSoundPosition(source, position);
            setSoundVolume(source, categoryVolumes[SoundCategory::ENEMY]);
            activeSources.push_back(source);
        }
    }
    
    void SoundManager::playEnemySpawnSound(const glm::vec3& position) {
        playEnemySound(EnemySoundType::SPAWN, position);
    }
    
    void SoundManager::playEnemyAttackSound(const glm::vec3& position) {
        playEnemySound(EnemySoundType::ATTACK, position);
    }
    
    void SoundManager::playEnemyHurtSound(const glm::vec3& position) {
        playEnemySound(EnemySoundType::HURT, position);
    }
    
    void SoundManager::playEnemyDeathSound(const glm::vec3& position) {
        playEnemySound(EnemySoundType::DEATH, position);
    }
    
    void SoundManager::playEnemyFootstepSound(const glm::vec3& position) {
        playEnemySound(EnemySoundType::FOOTSTEP, position);
    }
    
    void SoundManager::playEnemyDetectSound(const glm::vec3& position) {
        playEnemySound(EnemySoundType::DETECT, position);
    }
    
    // Environmental sounds
    void SoundManager::playEnvironmentSound(EnvironmentSoundType type, const glm::vec3& position) {
        if (categoryMuted[SoundCategory::ENVIRONMENT]) return;
        
        std::string soundName = getEnvironmentSoundName(type);
        ALuint source = audioEngine->playSound(soundName);
        if (source != 0) {
            audioEngine->setSoundPosition(source, position);
            setSoundVolume(source, categoryVolumes[SoundCategory::ENVIRONMENT]);
            activeSources.push_back(source);
        }
    }
    
    void SoundManager::playAmbientSound(const std::string& soundName, const glm::vec3& position, bool loop) {
        if (categoryMuted[SoundCategory::ENVIRONMENT]) return;
        
        ALuint source = audioEngine->playSound(soundName, loop);
        if (source != 0) {
            audioEngine->setSoundPosition(source, position);
            setSoundVolume(source, categoryVolumes[SoundCategory::ENVIRONMENT]);
            activeSources.push_back(source);
        }
    }
    
    void SoundManager::playDoorOpenSound(const glm::vec3& position) {
        playEnvironmentSound(EnvironmentSoundType::DOOR_OPEN, position);
    }
    
    void SoundManager::playDoorCloseSound(const glm::vec3& position) {
        playEnvironmentSound(EnvironmentSoundType::DOOR_CLOSE, position);
    }
    
    void SoundManager::playButtonPressSound(const glm::vec3& position) {
        playEnvironmentSound(EnvironmentSoundType::BUTTON_PRESS, position);
    }
    
    void SoundManager::playExplosionSound(const glm::vec3& position) {
        playEnvironmentSound(EnvironmentSoundType::EXPLOSION, position);
    }
    
    void SoundManager::playWaterSound(const glm::vec3& position) {
        playEnvironmentSound(EnvironmentSoundType::WATER, position);
    }
    
    void SoundManager::playWindSound(const glm::vec3& position) {
        playEnvironmentSound(EnvironmentSoundType::WIND, position);
    }
    
    // Player sounds
    void SoundManager::playPlayerSound(PlayerSoundType type, const glm::vec3& position) {
        if (categoryMuted[SoundCategory::PLAYER]) return;
        
        std::string soundName = getPlayerSoundName(type);
        ALuint source = audioEngine->playSound(soundName);
        if (source != 0) {
            audioEngine->setSoundPosition(source, position);
            setSoundVolume(source, categoryVolumes[SoundCategory::PLAYER]);
            activeSources.push_back(source);
        }
    }
    
    void SoundManager::playPlayerHurtSound(const glm::vec3& position) {
        playPlayerSound(PlayerSoundType::HURT, position);
    }
    
    void SoundManager::playPlayerDeathSound(const glm::vec3& position) {
        playPlayerSound(PlayerSoundType::DEATH, position);
    }
    
    void SoundManager::playPlayerFootstepSound(const glm::vec3& position) {
        playPlayerSound(PlayerSoundType::FOOTSTEP, position);
    }
    
    void SoundManager::playPlayerJumpSound(const glm::vec3& position) {
        playPlayerSound(PlayerSoundType::JUMP, position);
    }
    
    void SoundManager::playPlayerLandSound(const glm::vec3& position) {
        playPlayerSound(PlayerSoundType::LAND, position);
    }
    
    void SoundManager::playPlayerInteractSound(const glm::vec3& position) {
        playPlayerSound(PlayerSoundType::INTERACT, position);
    }
    
    // Music
    void SoundManager::playBackgroundMusic(const std::string& filepath) {
        if (categoryMuted[SoundCategory::MUSIC]) return;
        
        audioEngine->playMusic(filepath);
        audioEngine->setMusicVolume(musicVolume);
    }
    
    void SoundManager::stopBackgroundMusic() {
        audioEngine->stopMusic();
    }
    
    void SoundManager::setMusicVolume(float volume) {
        musicVolume = volume;
        audioEngine->setMusicVolume(volume);
    }
    
    // Sound settings
    void SoundManager::setMasterVolume(float volume) {
        masterVolume = volume;
        audioEngine->setMasterVolume(volume);
    }
    
    void SoundManager::setCategoryVolume(SoundCategory category, float volume) {
        categoryVolumes[category] = volume;
        // Update all active sounds in this category
        // This would require tracking which sources belong to which category
    }
    
    float SoundManager::getCategoryVolume(SoundCategory category) const {
        auto it = categoryVolumes.find(category);
        return (it != categoryVolumes.end()) ? it->second : 1.0f;
    }
    
    void SoundManager::muteCategory(SoundCategory category, bool mute) {
        categoryMuted[category] = mute;
        // Stop all active sounds in this category if muting
        if (mute) {
            // This would require tracking which sources belong to which category
        }
    }
    
    bool SoundManager::isCategoryMuted(SoundCategory category) const {
        auto it = categoryMuted.find(category);
        return (it != categoryMuted.end()) ? it->second : false;
    }
    
    // 3D positioning
    void SoundManager::setListenerPosition(const glm::vec3& position) {
        audioEngine->setListenerPosition(position);
    }
    
    void SoundManager::setListenerOrientation(const glm::vec3& forward, const glm::vec3& up) {
        audioEngine->setListenerOrientation(forward, up);
    }
    
    void SoundManager::setListenerVelocity(const glm::vec3& velocity) {
        audioEngine->setListenerVelocity(velocity);
    }
    
    // Sound effects
    void SoundManager::enableReverb(bool enable) {
        audioEngine->enableReverb(enable);
    }
    
    void SoundManager::setReverbProperties(float density, float diffusion, float gain) {
        audioEngine->setReverbProperties(density, diffusion, gain);
    }
    
    // Utility functions
    void SoundManager::preloadSound(const std::string& soundName, const std::string& filepath) {
        audioEngine->loadSound(soundName, filepath);
    }
    
    bool SoundManager::isSoundPlaying(const std::string& soundName) const {
        // This would require tracking active sources
        return false;
    }
    
    void SoundManager::stopAllSounds() {
        for (ALuint source : activeSources) {
            audioEngine->stopSound(source);
        }
        activeSources.clear();
    }
    
    // Helper methods
    std::string SoundManager::getWeaponSoundName(WeaponSoundType type) const {
        switch (type) {
            case WeaponSoundType::SHOOT: return "weapon_shoot";
            case WeaponSoundType::RELOAD: return "weapon_reload";
            case WeaponSoundType::EMPTY: return "weapon_empty";
            case WeaponSoundType::PICKUP: return "weapon_pickup";
            case WeaponSoundType::DROP: return "weapon_drop";
            default: return "weapon_shoot";
        }
    }
    
    std::string SoundManager::getEnemySoundName(EnemySoundType type) const {
        switch (type) {
            case EnemySoundType::SPAWN: return "enemy_spawn";
            case EnemySoundType::ATTACK: return "enemy_attack";
            case EnemySoundType::HURT: return "enemy_hurt";
            case EnemySoundType::DEATH: return "enemy_death";
            case EnemySoundType::FOOTSTEP: return "enemy_footstep";
            case EnemySoundType::DETECT: return "enemy_detect";
            default: return "enemy_attack";
        }
    }
    
    std::string SoundManager::getEnvironmentSoundName(EnvironmentSoundType type) const {
        switch (type) {
            case EnvironmentSoundType::AMBIENT: return "ambient";
            case EnvironmentSoundType::DOOR_OPEN: return "door_open";
            case EnvironmentSoundType::DOOR_CLOSE: return "door_close";
            case EnvironmentSoundType::BUTTON_PRESS: return "button_press";
            case EnvironmentSoundType::EXPLOSION: return "explosion";
            case EnvironmentSoundType::WATER: return "water";
            case EnvironmentSoundType::WIND: return "wind";
            default: return "ambient";
        }
    }
    
    std::string SoundManager::getPlayerSoundName(PlayerSoundType type) const {
        switch (type) {
            case PlayerSoundType::HURT: return "player_hurt";
            case PlayerSoundType::DEATH: return "player_death";
            case PlayerSoundType::FOOTSTEP: return "player_footstep";
            case PlayerSoundType::JUMP: return "player_jump";
            case PlayerSoundType::LAND: return "player_land";
            case PlayerSoundType::INTERACT: return "player_interact";
            default: return "player_hurt";
        }
    }
    
    void SoundManager::loadDefaultSounds() {
        // Load default weapon sounds
        audioEngine->loadSound("weapon_shoot", "sounds/weapon_shoot.wav");
        audioEngine->loadSound("weapon_reload", "sounds/weapon_reload.wav");
        audioEngine->loadSound("weapon_empty", "sounds/weapon_empty.wav");
        audioEngine->loadSound("weapon_pickup", "sounds/weapon_pickup.wav");
        audioEngine->loadSound("weapon_drop", "sounds/weapon_drop.wav");
        
        // Load default enemy sounds
        audioEngine->loadSound("enemy_spawn", "sounds/enemy_spawn.wav");
        audioEngine->loadSound("enemy_attack", "sounds/enemy_attack.wav");
        audioEngine->loadSound("enemy_hurt", "sounds/enemy_hurt.wav");
        audioEngine->loadSound("enemy_death", "sounds/enemy_death.wav");
        audioEngine->loadSound("enemy_footstep", "sounds/enemy_footstep.wav");
        audioEngine->loadSound("enemy_detect", "sounds/enemy_detect.wav");
        
        // Load default environmental sounds
        audioEngine->loadSound("door_open", "sounds/door_open.wav");
        audioEngine->loadSound("door_close", "sounds/door_close.wav");
        audioEngine->loadSound("button_press", "sounds/button_press.wav");
        audioEngine->loadSound("explosion", "sounds/explosion.wav");
        audioEngine->loadSound("water", "sounds/water.wav");
        audioEngine->loadSound("wind", "sounds/wind.wav");
        
        // Load default player sounds
        audioEngine->loadSound("player_hurt", "sounds/player_hurt.wav");
        audioEngine->loadSound("player_death", "sounds/player_death.wav");
        audioEngine->loadSound("player_footstep", "sounds/player_footstep.wav");
        audioEngine->loadSound("player_jump", "sounds/player_jump.wav");
        audioEngine->loadSound("player_land", "sounds/player_land.wav");
        audioEngine->loadSound("player_interact", "sounds/player_interact.wav");
    }
    
    void SoundManager::setSoundVolume(ALuint source, float volume) {
        audioEngine->setSoundVolume(source, volume * masterVolume);
    }
    
    void SoundManager::setSoundCategory(ALuint source, SoundCategory category) {
        // This would require a way to associate sources with categories
    }
}
#else
// Audio is disabled, provide empty implementation
#include "../include/SoundManager.h"
#include "../include/Logger.h"

namespace Sparky {
    
    SoundManager& SoundManager::getInstance() {
        static SoundManager instance;
        return instance;
    }
    
    SoundManager::SoundManager() : masterVolume(1.0f), musicVolume(0.7f) {
        Logger::getInstance().info("SoundManager created (audio disabled)");
    }
    
    SoundManager::~SoundManager() {
        Logger::getInstance().info("SoundManager destroyed (audio disabled)");
    }
    
    bool SoundManager::initialize() {
        Logger::getInstance().info("SoundManager initialized (audio disabled)");
        return true;
    }
    
    void SoundManager::cleanup() {
        Logger::getInstance().info("SoundManager cleaned up (audio disabled)");
    }
}
#endif