#include "../include/ResourceManager.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

// Simple test classes for the resource manager demo
class Texture {
public:
    Texture() { std::cout << "Texture created" << std::endl; }
    ~Texture() { std::cout << "Texture destroyed" << std::endl; }
};

class Shader {
public:
    Shader() { std::cout << "Shader created" << std::endl; }
    ~Shader() { std::cout << "Shader destroyed" << std::endl; }
};

class Model {
public:
    Model() { std::cout << "Model created" << std::endl; }
    ~Model() { std::cout << "Model destroyed" << std::endl; }
};

// Demo to showcase ResourceManager dependency injection
int main() {
    std::cout << "=== ResourceManager Dependency Injection Demo ===" << std::endl;
    
    // Example 1: Using singleton (backward compatibility)
    std::cout << "\n--- Example 1: Singleton (Backward Compatibility) ---" << std::endl;
    Sparky::ResourceManager& singletonManager = Sparky::ResourceManager::getInstance();
    std::cout << "Singleton ResourceManager created" << std::endl;
    
    // Example 2: Creating ResourceManager with dependency injection
    std::cout << "\n--- Example 2: Dependency Injection ---" << std::endl;
    auto customManager = std::make_unique<Sparky::ResourceManager>("./assets/");
    std::cout << "Custom ResourceManager created with resource path: " << customManager->getResourcePath() << std::endl;
    
    // Example 3: Using factory method
    std::cout << "\n--- Example 3: Factory Method ---" << std::endl;
    auto factoryManager = Sparky::ResourceManager::create("./resources/");
    std::cout << "Factory ResourceManager created with resource path: " << factoryManager->getResourcePath() << std::endl;
    
    // Example 4: Multiple ResourceManager instances for different purposes
    std::cout << "\n--- Example 4: Multiple Instances ---" << std::endl;
    auto graphicsManager = Sparky::ResourceManager::create("./assets/textures/");
    auto audioManager = Sparky::ResourceManager::create("./assets/audio/");
    auto modelManager = Sparky::ResourceManager::create("./assets/models/");
    
    std::cout << "Graphics manager path: " << graphicsManager->getResourcePath() << std::endl;
    std::cout << "Audio manager path: " << audioManager->getResourcePath() << std::endl;
    std::cout << "Model manager path: " << modelManager->getResourcePath() << std::endl;
    
    // Example 5: Loading resources with different managers
    std::cout << "\n--- Example 5: Resource Loading ---" << std::endl;
    graphicsManager->loadResource<Texture>("player_texture", "./assets/textures/player.png");
    graphicsManager->loadResource<Shader>("basic_shader", "./assets/shaders/basic.vert");
    modelManager->loadResource<Model>("player_model", "./assets/models/player.obj");
    
    std::cout << "Resources loaded:" << std::endl;
    std::cout << "  Player texture: " << (graphicsManager->hasResource<Texture>("player_texture") ? "loaded" : "not loaded") << std::endl;
    std::cout << "  Basic shader: " << (graphicsManager->hasResource<Shader>("basic_shader") ? "loaded" : "not loaded") << std::endl;
    std::cout << "  Player model: " << (modelManager->hasResource<Model>("player_model") ? "loaded" : "not loaded") << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}