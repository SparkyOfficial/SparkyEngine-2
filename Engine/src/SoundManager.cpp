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
        
        audioEngine->playBackgroundMusic(filepath);
    }
    
    void SoundManager::stopBackgroundMusic() {
        // In a full implementation, we would stop the music
        Logger::getInstance().info("Stopping background music");
    }
    
    void SoundManager::setMusicVolume(float volume) {
        musicVolume = std::max(0.0f, std::min(1.0f, volume));
        Logger::getInstance().info("Music volume set to: " + std::to_string(musicVolume));
    }
    
    // Sound settings
    void SoundManager::setMasterVolume(float volume) {
        masterVolume = std::max(0.0f, std::min(1.0f, volume));
        Logger::getInstance().info("Master volume set to: " + std::to_string(masterVolume));
    }
    
    void SoundManager::setCategoryVolume(SoundCategory category, float volume) {
        volume = std::max(0.0f, std::min(1.0f, volume));
        categoryVolumes[category] = volume;
        Logger::getInstance().info("Category volume set for category " + std::to_string(static_cast<int>(category)) + 
                                 " to: " + std::to_string(volume));
    }
    
    float SoundManager::getCategoryVolume(SoundCategory category) const {
        auto it = categoryVolumes.find(category);
        if (it != categoryVolumes.end()) {
            return it->second;
        }
        return 1.0f;
    }
    
    void SoundManager::muteCategory(SoundCategory category, bool mute) {
        categoryMuted[category] = mute;
        Logger::getInstance().info("Category " + std::to_string(static_cast<int>(category)) + 
                                 (mute ? " muted" : " unmuted"));
    }
    
    bool SoundManager::isCategoryMuted(SoundCategory category) const {
        auto it = categoryMuted.find(category);
        if (it != categoryMuted.end()) {
            return it->second;
        }
        return false;
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
        Logger::getInstance().info("Reverb " + std::string(enable ? "enabled" : "disabled"));
    }
    
    void SoundManager::setReverbProperties(float density, float diffusion, float gain) {
        Logger::getInstance().info("Reverb properties set - Density: " + std::to_string(density) + 
                                 ", Diffusion: " + std::to_string(diffusion) + 
                                 ", Gain: " + std::to_string(gain));
    }
    
    // Utility functions
    void SoundManager::preloadSound(const std::string& soundName, const std::string& filepath) {
        audioEngine->loadSound(soundName, filepath);
    }
    
    bool SoundManager::isSoundPlaying(const std::string& soundName) const {
        // In a full implementation, we would check if the sound is playing
        return false;
    }
    
    void SoundManager::stopAllSounds() {
        for (ALuint source : activeSources) {
            audioEngine->stopSound(source);
        }
        activeSources.clear();
        Logger::getInstance().info("All sounds stopped");
    }
    
    // Helper methods
    std::string SoundManager::getWeaponSoundName(WeaponSoundType type) const {
        switch (type) {
            case WeaponSoundType::SHOOT: return "weapon_shoot";
            case WeaponSoundType::RELOAD: return "weapon_reload";
            case WeaponSoundType::EMPTY: return "weapon_empty";
            case WeaponSoundType::PICKUP: return "weapon_pickup";
            case WeaponSoundType::DROP: return "weapon_drop";
            default: return "weapon_default";
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
            default: return "enemy_default";
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
            default: return "environment_default";
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
            default: return "player_default";
        }
    }
    
    void SoundManager::loadDefaultSounds() {
        Logger::getInstance().info("Loading default sounds");
        
        // In a real implementation, we would load actual sound files
        // For now, we'll just log that we're loading them
        
        // Weapon sounds
        audioEngine->loadSound("weapon_shoot", "sounds/weapon/shoot.wav");
        audioEngine->loadSound("weapon_reload", "sounds/weapon/reload.wav");
        audioEngine->loadSound("weapon_empty", "sounds/weapon/empty.wav");
        audioEngine->loadSound("weapon_pickup", "sounds/weapon/pickup.wav");
        audioEngine->loadSound("weapon_drop", "sounds/weapon/drop.wav");
        
        // Enemy sounds
        audioEngine->loadSound("enemy_spawn", "sounds/enemy/spawn.wav");
        audioEngine->loadSound("enemy_attack", "sounds/enemy/attack.wav");
        audioEngine->loadSound("enemy_hurt", "sounds/enemy/hurt.wav");
        audioEngine->loadSound("enemy_death", "sounds/enemy/death.wav");
        audioEngine->loadSound("enemy_footstep", "sounds/enemy/footstep.wav");
        audioEngine->loadSound("enemy_detect", "sounds/enemy/detect.wav");
        
        // Environmental sounds
        audioEngine->loadSound("ambient", "sounds/environment/ambient.wav");
        audioEngine->loadSound("door_open", "sounds/environment/door_open.wav");
        audioEngine->loadSound("door_close", "sounds/environment/door_close.wav");
        audioEngine->loadSound("button_press", "sounds/environment/button_press.wav");
        audioEngine->loadSound("explosion", "sounds/environment/explosion.wav");
        audioEngine->loadSound("water", "sounds/environment/water.wav");
        audioEngine->loadSound("wind", "sounds/environment/wind.wav");
        
        // Player sounds
        audioEngine->loadSound("player_hurt", "sounds/player/hurt.wav");
        audioEngine->loadSound("player_death", "sounds/player/death.wav");
        audioEngine->loadSound("player_footstep", "sounds/player/footstep.wav");
        audioEngine->loadSound("player_jump", "sounds/player/jump.wav");
        audioEngine->loadSound("player_land", "sounds/player/land.wav");
        audioEngine->loadSound("player_interact", "sounds/player/interact.wav");
        
        Logger::getInstance().info("Default sounds loaded");
    }
    
    void SoundManager::setSoundVolume(ALuint source, float volume) {
        if (source != 0) {
            float finalVolume = masterVolume * volume;
            audioEngine->setSoundVolume(source, finalVolume);
        }
    }
    
    void SoundManager::setSoundCategory(ALuint source, SoundCategory category) {
        // In a full implementation, we would track sound categories
        // For now, this is just a placeholder
    }
}