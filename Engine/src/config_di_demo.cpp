#include "../include/Config.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

// Demo to showcase Config dependency injection
int main() {
    std::cout << "=== Config Dependency Injection Demo ===" << std::endl;
    
    // Example 1: Using singleton (backward compatibility)
    std::cout << "\n--- Example 1: Singleton (Backward Compatibility) ---" << std::endl;
    Sparky::Config& singletonConfig = Sparky::Config::getInstance();
    singletonConfig.setInt("window_width", 1920);
    singletonConfig.setInt("window_height", 1080);
    singletonConfig.setBool("fullscreen", false);
    
    std::cout << "Singleton config values:" << std::endl;
    std::cout << "  Window width: " << singletonConfig.getInt("window_width") << std::endl;
    std::cout << "  Window height: " << singletonConfig.getInt("window_height") << std::endl;
    std::cout << "  Fullscreen: " << (singletonConfig.getBool("fullscreen") ? "true" : "false") << std::endl;
    
    // Example 2: Creating Config with dependency injection
    std::cout << "\n--- Example 2: Dependency Injection ---" << std::endl;
    auto customConfig = std::make_unique<Sparky::Config>();
    customConfig->setString("player_name", "Sparky");
    customConfig->setInt("player_level", 1);
    customConfig->setFloat("player_health", 100.0f);
    
    std::cout << "Custom config values:" << std::endl;
    std::cout << "  Player name: " << customConfig->getString("player_name") << std::endl;
    std::cout << "  Player level: " << customConfig->getInt("player_level") << std::endl;
    std::cout << "  Player health: " << customConfig->getFloat("player_health") << std::endl;
    
    // Example 3: Using factory method
    std::cout << "\n--- Example 3: Factory Method ---" << std::endl;
    auto factoryConfig = Sparky::Config::create();
    factoryConfig->setString("game_title", "Sparky Engine Demo");
    factoryConfig->setString("version", "3.0.0");
    factoryConfig->setBool("debug_mode", true);
    
    std::cout << "Factory config values:" << std::endl;
    std::cout << "  Game title: " << factoryConfig->getString("game_title") << std::endl;
    std::cout << "  Version: " << factoryConfig->getString("version") << std::endl;
    std::cout << "  Debug mode: " << (factoryConfig->getBool("debug_mode") ? "true" : "false") << std::endl;
    
    // Example 4: Multiple Config instances for different purposes
    std::cout << "\n--- Example 4: Multiple Instances ---" << std::endl;
    auto graphicsConfig = Sparky::Config::create();
    graphicsConfig->setInt("resolution_width", 1920);
    graphicsConfig->setInt("resolution_height", 1080);
    graphicsConfig->setString("renderer", "Vulkan");
    graphicsConfig->setBool("vsync", true);
    
    auto audioConfig = Sparky::Config::create();
    audioConfig->setFloat("master_volume", 0.8f);
    audioConfig->setFloat("music_volume", 0.6f);
    audioConfig->setFloat("sfx_volume", 1.0f);
    audioConfig->setBool("spatial_audio", true);
    
    std::cout << "Graphics config:" << std::endl;
    std::cout << "  Resolution: " << graphicsConfig->getInt("resolution_width") << "x" << graphicsConfig->getInt("resolution_height") << std::endl;
    std::cout << "  Renderer: " << graphicsConfig->getString("renderer") << std::endl;
    std::cout << "  VSync: " << (graphicsConfig->getBool("vsync") ? "enabled" : "disabled") << std::endl;
    
    std::cout << "Audio config:" << std::endl;
    std::cout << "  Master volume: " << audioConfig->getFloat("master_volume") << std::endl;
    std::cout << "  Music volume: " << audioConfig->getFloat("music_volume") << std::endl;
    std::cout << "  SFX volume: " << audioConfig->getFloat("sfx_volume") << std::endl;
    std::cout << "  Spatial audio: " << (audioConfig->getBool("spatial_audio") ? "enabled" : "disabled") << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}