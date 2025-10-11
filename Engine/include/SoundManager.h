#pragma once

#include "AudioEngine.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>

namespace Sparky {
    
    // Sound categories
    enum class SoundCategory {
        WEAPON,
        ENEMY,
        ENVIRONMENT,
        PLAYER,
        UI,
        MUSIC
    };
    
    // Weapon sound types
    enum class WeaponSoundType {
        SHOOT,
        RELOAD,
        EMPTY,
        PICKUP,
        DROP
    };
    
    // Enemy sound types
    enum class EnemySoundType {
        SPAWN,
        ATTACK,
        HURT,
        DEATH,
        FOOTSTEP,
        DETECT
    };
    
    // Environmental sound types
    enum class EnvironmentSoundType {
        AMBIENT,
        DOOR_OPEN,
        DOOR_CLOSE,
        BUTTON_PRESS,
        EXPLOSION,
        WATER,
        WIND
    };
    
    // Player sound types
    enum class PlayerSoundType {
        HURT,
        DEATH,
        FOOTSTEP,
        JUMP,
        LAND,
        INTERACT
    };
    
    class SoundManager {
    public:
        static SoundManager& getInstance();
        
        // Initialize and cleanup
        bool initialize();
        void cleanup();
        
        // Weapon sounds
        void playWeaponSound(WeaponSoundType type, const glm::vec3& position = glm::vec3(0.0f));
        void playGunshotSound(const glm::vec3& position = glm::vec3(0.0f));
        void playReloadSound(const glm::vec3& position = glm::vec3(0.0f));
        void playWeaponEmptySound(const glm::vec3& position = glm::vec3(0.0f));
        void playWeaponPickupSound(const glm::vec3& position = glm::vec3(0.0f));
        void playWeaponDropSound(const glm::vec3& position = glm::vec3(0.0f));
        
        // Enemy sounds
        void playEnemySound(EnemySoundType type, const glm::vec3& position = glm::vec3(0.0f));
        void playEnemySpawnSound(const glm::vec3& position = glm::vec3(0.0f));
        void playEnemyAttackSound(const glm::vec3& position = glm::vec3(0.0f));
        void playEnemyHurtSound(const glm::vec3& position = glm::vec3(0.0f));
        void playEnemyDeathSound(const glm::vec3& position = glm::vec3(0.0f));
        void playEnemyFootstepSound(const glm::vec3& position = glm::vec3(0.0f));
        void playEnemyDetectSound(const glm::vec3& position = glm::vec3(0.0f));
        
        // Environmental sounds
        void playEnvironmentSound(EnvironmentSoundType type, const glm::vec3& position = glm::vec3(0.0f));
        void playAmbientSound(const std::string& soundName, const glm::vec3& position = glm::vec3(0.0f), bool loop = true);
        void playDoorOpenSound(const glm::vec3& position = glm::vec3(0.0f));
        void playDoorCloseSound(const glm::vec3& position = glm::vec3(0.0f));
        void playButtonPressSound(const glm::vec3& position = glm::vec3(0.0f));
        void playExplosionSound(const glm::vec3& position = glm::vec3(0.0f));
        void playWaterSound(const glm::vec3& position = glm::vec3(0.0f));
        void playWindSound(const glm::vec3& position = glm::vec3(0.0f));
        
        // Player sounds
        void playPlayerSound(PlayerSoundType type, const glm::vec3& position = glm::vec3(0.0f));
        void playPlayerHurtSound(const glm::vec3& position = glm::vec3(0.0f));
        void playPlayerDeathSound(const glm::vec3& position = glm::vec3(0.0f));
        void playPlayerFootstepSound(const glm::vec3& position = glm::vec3(0.0f));
        void playPlayerJumpSound(const glm::vec3& position = glm::vec3(0.0f));
        void playPlayerLandSound(const glm::vec3& position = glm::vec3(0.0f));
        void playPlayerInteractSound(const glm::vec3& position = glm::vec3(0.0f));
        
        // Music
        void playBackgroundMusic(const std::string& filepath);
        void stopBackgroundMusic();
        void setMusicVolume(float volume);
        float getMusicVolume() const { return musicVolume; }
        
        // Sound settings
        void setMasterVolume(float volume);
        float getMasterVolume() const { return masterVolume; }
        
        void setCategoryVolume(SoundCategory category, float volume);
        float getCategoryVolume(SoundCategory category) const;
        
        void muteCategory(SoundCategory category, bool mute = true);
        bool isCategoryMuted(SoundCategory category) const;
        
        // 3D positioning
        void setListenerPosition(const glm::vec3& position);
        void setListenerOrientation(const glm::vec3& forward, const glm::vec3& up);
        void setListenerVelocity(const glm::vec3& velocity);
        
        // Sound effects
        void enableReverb(bool enable);
        void setReverbProperties(float density, float diffusion, float gain);
        
        // Utility functions
        void preloadSound(const std::string& soundName, const std::string& filepath);
        bool isSoundPlaying(const std::string& soundName) const;
        void stopAllSounds();

    private:
        SoundManager();
        ~SoundManager();
        
        // Helper methods
        std::string getWeaponSoundName(WeaponSoundType type) const;
        std::string getEnemySoundName(EnemySoundType type) const;
        std::string getEnvironmentSoundName(EnvironmentSoundType type) const;
        std::string getPlayerSoundName(PlayerSoundType type) const;
        
        void loadDefaultSounds();
        void setSoundVolume(ALuint source, float volume);
        void setSoundCategory(ALuint source, SoundCategory category);
        
        AudioEngine* audioEngine;
        float masterVolume;
        float musicVolume;
        std::unordered_map<SoundCategory, float> categoryVolumes;
        std::unordered_map<SoundCategory, bool> categoryMuted;
        std::vector<ALuint> activeSources;
    };
}