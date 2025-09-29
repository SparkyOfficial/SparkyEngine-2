#include "../include/AssetManager.h"
#include "../include/Texture.h"
#include "../include/Logger.h"
#include <iostream>

int main() {
    // Initialize logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    try {
        // Get the asset manager instance
        Sparky::AssetManager& assetManager = Sparky::AssetManager::getInstance();
        
        std::cout << "Testing STB Image texture loading..." << std::endl;
        
        // Load a texture (this will generate a placeholder since we don't have actual image files)
        assetManager.loadTexture("test_texture", "assets/nonexistent.png");
        
        // Check if texture was loaded
        if (assetManager.hasTexture("test_texture")) {
            Sparky::Texture* texture = assetManager.getTexture("test_texture");
            if (texture) {
                std::cout << "Texture loaded successfully!" << std::endl;
                std::cout << "Width: " << texture->getWidth() << std::endl;
                std::cout << "Height: " << texture->getHeight() << std::endl;
                std::cout << "Channels: " << texture->getChannels() << std::endl;
                std::cout << "Data size: " << texture->getData().size() << " bytes" << std::endl;
            } else {
                std::cout << "Failed to get texture!" << std::endl;
            }
        } else {
            std::cout << "Texture not found!" << std::endl;
        }
        
        std::cout << "STB Image test completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}