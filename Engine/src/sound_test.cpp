#include "../include/SoundManager.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <thread>
#include <chrono>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    Sparky::Logger::getInstance().info("=== Sound Effects System Test ===");
    
    // Initialize sound manager
    Sparky::SoundManager& soundManager = Sparky::SoundManager::getInstance();
    if (!soundManager.initialize()) {
        Sparky::Logger::getInstance().error("Failed to initialize SoundManager");
        return 1;
    }
    
    Sparky::Logger::getInstance().info("SoundManager initialized successfully");
    
    // Test weapon sounds
    Sparky::Logger::getInstance().info("Testing weapon sounds...");
    soundManager.playGunshotSound(glm::vec3(0.0f, 0.0f, 0.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playReloadSound(glm::vec3(2.0f, 0.0f, 0.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playWeaponEmptySound(glm::vec3(4.0f, 0.0f, 0.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Test enemy sounds
    Sparky::Logger::getInstance().info("Testing enemy sounds...");
    soundManager.playEnemySpawnSound(glm::vec3(0.0f, 0.0f, 2.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playEnemyAttackSound(glm::vec3(2.0f, 0.0f, 2.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playEnemyHurtSound(glm::vec3(4.0f, 0.0f, 2.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playEnemyDeathSound(glm::vec3(6.0f, 0.0f, 2.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Test environmental sounds
    Sparky::Logger::getInstance().info("Testing environmental sounds...");
    soundManager.playDoorOpenSound(glm::vec3(0.0f, 0.0f, 4.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playButtonPressSound(glm::vec3(2.0f, 0.0f, 4.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playExplosionSound(glm::vec3(4.0f, 0.0f, 4.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Test player sounds
    Sparky::Logger::getInstance().info("Testing player sounds...");
    soundManager.playPlayerHurtSound(glm::vec3(0.0f, 0.0f, 6.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playPlayerFootstepSound(glm::vec3(2.0f, 0.0f, 6.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    soundManager.playPlayerJumpSound(glm::vec3(4.0f, 0.0f, 6.0f));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Test sound settings
    Sparky::Logger::getInstance().info("Testing sound settings...");
    soundManager.setMasterVolume(0.8f);
    soundManager.setCategoryVolume(Sparky::SoundCategory::WEAPON, 0.7f);
    soundManager.setCategoryVolume(Sparky::SoundCategory::ENEMY, 0.6f);
    soundManager.muteCategory(Sparky::SoundCategory::MUSIC, true);
    
    Sparky::Logger::getInstance().info("Master volume: " + std::to_string(soundManager.getMasterVolume()));
    Sparky::Logger::getInstance().info("Weapon category volume: " + 
                                     std::to_string(soundManager.getCategoryVolume(Sparky::SoundCategory::WEAPON)));
    Sparky::Logger::getInstance().info("Music category muted: " + 
                                     std::string(soundManager.isCategoryMuted(Sparky::SoundCategory::MUSIC) ? "true" : "false"));
    
    // Test 3D positioning
    Sparky::Logger::getInstance().info("Testing 3D positioning...");
    soundManager.setListenerPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    soundManager.setListenerOrientation(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    soundManager.setListenerVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Test sound effects
    Sparky::Logger::getInstance().info("Testing sound effects...");
    soundManager.enableReverb(true);
    soundManager.setReverbProperties(0.5f, 0.3f, 0.8f);
    
    // Test utility functions
    Sparky::Logger::getInstance().info("Testing utility functions...");
    soundManager.preloadSound("test_sound", "sounds/test.wav");
    soundManager.stopAllSounds();
    
    // Show sound system features
    Sparky::Logger::getInstance().info("");
    Sparky::Logger::getInstance().info("=== Sound System Features ===");
    Sparky::Logger::getInstance().info("1. Weapon sound effects (shoot, reload, empty, pickup, drop)");
    Sparky::Logger::getInstance().info("2. Enemy sound effects (spawn, attack, hurt, death, footstep, detect)");
    Sparky::Logger::getInstance().info("3. Environmental sound effects (doors, buttons, explosions, ambient)");
    Sparky::Logger::getInstance().info("4. Player sound effects (hurt, death, footstep, jump, land, interact)");
    Sparky::Logger::getInstance().info("5. Music playback and control");
    Sparky::Logger::getInstance().info("6. Volume control (master and per-category)");
    Sparky::Logger::getInstance().info("7. 3D spatial audio positioning");
    Sparky::Logger::getInstance().info("8. Sound effects (reverb, echo, etc.)");
    Sparky::Logger::getInstance().info("9. Sound muting per category");
    Sparky::Logger::getInstance().info("10. Sound preloading and management");
    
    // Cleanup
    soundManager.cleanup();
    Sparky::Logger::getInstance().info("");
    Sparky::Logger::getInstance().info("Sound effects system test completed successfully!");
    return 0;
}