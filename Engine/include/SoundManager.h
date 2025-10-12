#pragma once

#ifdef ENABLE_AUDIO
#include "AudioEngine.h"
#endif

#include <string>
#include <unordered_map>
#include <vector>

// Forward declarations
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
    
#ifdef ENABLE_AUDIO
    typedef unsigned int ALuint;
    // Forward declaration for glm when audio is enabled
    namespace glm {
        template<typename T> struct vec3;
        typedef vec3<float> vec3f;
    }
#else
    typedef unsigned int ALuint;
#endif
    
    class SoundManager {
    public:
        static SoundManager& getInstance();
        
        // Initialize and cleanup
        bool initialize();
        void cleanup();
        
        // Weapon sounds
#ifdef ENABLE_AUDIO
        void playWeaponSound(WeaponSoundType type, const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playGunshotSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playReloadSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playWeaponEmptySound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playWeaponPickupSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playWeaponDropSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
#else
        template<typename T> struct vec3 { T x, y, z; vec3(T x=0, T y=0, T z=0) : x(x), y(y), z(z) {} };
        typedef vec3<float> vec3f;
        void playWeaponSound(WeaponSoundType type, const vec3f& position = vec3f(0.0f)) {}
        void playGunshotSound(const vec3f& position = vec3f(0.0f)) {}
        void playReloadSound(const vec3f& position = vec3f(0.0f)) {}
        void playWeaponEmptySound(const vec3f& position = vec3f(0.0f)) {}
        void playWeaponPickupSound(const vec3f& position = vec3f(0.0f)) {}
        void playWeaponDropSound(const vec3f& position = vec3f(0.0f)) {}
#endif
        
        // Enemy sounds
#ifdef ENABLE_AUDIO
        void playEnemySound(EnemySoundType type, const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playEnemySpawnSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playEnemyAttackSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playEnemyHurtSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playEnemyDeathSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playEnemyFootstepSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playEnemyDetectSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
#else
        void playEnemySound(EnemySoundType type, const vec3f& position = vec3f(0.0f)) {}
        void playEnemySpawnSound(const vec3f& position = vec3f(0.0f)) {}
        void playEnemyAttackSound(const vec3f& position = vec3f(0.0f)) {}
        void playEnemyHurtSound(const vec3f& position = vec3f(0.0f)) {}
        void playEnemyDeathSound(const vec3f& position = vec3f(0.0f)) {}
        void playEnemyFootstepSound(const vec3f& position = vec3f(0.0f)) {}
        void playEnemyDetectSound(const vec3f& position = vec3f(0.0f)) {}
#endif
        
        // Environmental sounds
#ifdef ENABLE_AUDIO
        void playEnvironmentSound(EnvironmentSoundType type, const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playAmbientSound(const std::string& soundName, const glm::vec3<float>& position = glm::vec3<float>(0.0f), bool loop = true);
        void playDoorOpenSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playDoorCloseSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playButtonPressSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playExplosionSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playWaterSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playWindSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
#else
        void playEnvironmentSound(EnvironmentSoundType type, const vec3f& position = vec3f(0.0f)) {}
        void playAmbientSound(const std::string& soundName, const vec3f& position = vec3f(0.0f), bool loop = true) {}
        void playDoorOpenSound(const vec3f& position = vec3f(0.0f)) {}
        void playDoorCloseSound(const vec3f& position = vec3f(0.0f)) {}
        void playButtonPressSound(const vec3f& position = vec3f(0.0f)) {}
        void playExplosionSound(const vec3f& position = vec3f(0.0f)) {}
        void playWaterSound(const vec3f& position = vec3f(0.0f)) {}
        void playWindSound(const vec3f& position = vec3f(0.0f)) {}
#endif
        
        // Player sounds
#ifdef ENABLE_AUDIO
        void playPlayerSound(PlayerSoundType type, const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playPlayerHurtSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playPlayerDeathSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playPlayerFootstepSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playPlayerJumpSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playPlayerLandSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
        void playPlayerInteractSound(const glm::vec3<float>& position = glm::vec3<float>(0.0f));
#else
        void playPlayerSound(PlayerSoundType type, const vec3f& position = vec3f(0.0f)) {}
        void playPlayerHurtSound(const vec3f& position = vec3f(0.0f)) {}
        void playPlayerDeathSound(const vec3f& position = vec3f(0.0f)) {}
        void playPlayerFootstepSound(const vec3f& position = vec3f(0.0f)) {}
        void playPlayerJumpSound(const vec3f& position = vec3f(0.0f)) {}
        void playPlayerLandSound(const vec3f& position = vec3f(0.0f)) {}
        void playPlayerInteractSound(const vec3f& position = vec3f(0.0f)) {}
#endif
        
        // Music
#ifdef ENABLE_AUDIO
        void playBackgroundMusic(const std::string& filepath);
        void stopBackgroundMusic();
        void setMusicVolume(float volume);
#else
        void playBackgroundMusic(const std::string& filepath) {}
        void stopBackgroundMusic() {}
        void setMusicVolume(float volume) {}
#endif
        float getMusicVolume() const { return musicVolume; }
        
        // Sound settings
#ifdef ENABLE_AUDIO
        void setMasterVolume(float volume);
        void setCategoryVolume(SoundCategory category, float volume);
        void muteCategory(SoundCategory category, bool mute = true);
        void setListenerPosition(const glm::vec3<float>& position);
        void setListenerOrientation(const glm::vec3<float>& forward, const glm::vec3<float>& up);
        void setListenerVelocity(const glm::vec3<float>& velocity);
        void enableReverb(bool enable);
        void setReverbProperties(float density, float diffusion, float gain);
        void preloadSound(const std::string& soundName, const std::string& filepath);
#else
        void setMasterVolume(float volume) {}
        void setCategoryVolume(SoundCategory category, float volume) {}
        void muteCategory(SoundCategory category, bool mute = true) {}
        void setListenerPosition(const vec3f& position) {}
        void setListenerOrientation(const vec3f& forward, const vec3f& up) {}
        void setListenerVelocity(const vec3f& velocity) {}
        void enableReverb(bool enable) {}
        void setReverbProperties(float density, float diffusion, float gain) {}
        void preloadSound(const std::string& soundName, const std::string& filepath) {}
#endif
        float getMasterVolume() const { return masterVolume; }
        float getCategoryVolume(SoundCategory category) const;
        bool isCategoryMuted(SoundCategory category) const;
        
        // Utility functions
#ifdef ENABLE_AUDIO
        bool isSoundPlaying(const std::string& soundName) const;
        void stopAllSounds();
#else
        bool isSoundPlaying(const std::string& soundName) const { return false; }
        void stopAllSounds() {}
#endif

    private:
        SoundManager();
        ~SoundManager();
        
        // Helper methods
#ifdef ENABLE_AUDIO
        std::string getWeaponSoundName(WeaponSoundType type) const;
        std::string getEnemySoundName(EnemySoundType type) const;
        std::string getEnvironmentSoundName(EnvironmentSoundType type) const;
        std::string getPlayerSoundName(PlayerSoundType type) const;
        void loadDefaultSounds();
        void setSoundVolume(ALuint source, float volume);
        void setSoundCategory(ALuint source, SoundCategory category);
#else
        std::string getWeaponSoundName(WeaponSoundType type) const { return ""; }
        std::string getEnemySoundName(EnemySoundType type) const { return ""; }
        std::string getEnvironmentSoundName(EnvironmentSoundType type) const { return ""; }
        std::string getPlayerSoundName(PlayerSoundType type) const { return ""; }
        void loadDefaultSounds() {}
        void setSoundVolume(ALuint source, float volume) {}
        void setSoundCategory(ALuint source, SoundCategory category) {}
#endif
        
#ifdef ENABLE_AUDIO
        AudioEngine* audioEngine;
#else
        void* audioEngine; // Dummy pointer when audio is disabled
#endif
        float masterVolume;
        float musicVolume;
        std::unordered_map<SoundCategory, float> categoryVolumes;
        std::unordered_map<SoundCategory, bool> categoryMuted;
        std::vector<ALuint> activeSources;
    };
}