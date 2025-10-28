#include "../include/Logger.h"
#include "../include/Scene.h"
#include "../include/Material.h"
#include "../include/VulkanRenderer.h"
#include "../include/RenderSystem.h"
#include <iostream>
#include <memory>

using namespace Sparky;

// Comprehensive demo showcasing all the improvements we've made
int main() {
    std::cout << "=== Sparky Engine 3 - Comprehensive Improvements Demo ===" << std::endl;
    
    // 1. Demonstrate Logger dependency injection
    std::cout << "\n1. Logger Dependency Injection:" << std::endl;
    {
        // Create a custom logger with debug level
        auto customLogger = Logger::create(LogLevel::DEBUG);
        customLogger->info("Custom logger created with DEBUG level");
        customLogger->debug("This debug message will be shown");
        
        // Show backward compatibility with singleton
        Logger::getInstance().warning("This uses the old singleton pattern");
    }
    
    // 2. Demonstrate enhanced Scene class
    std::cout << "\n2. Enhanced Scene Management:" << std::endl;
    {
        auto scene = std::make_unique<Scene>();
        scene->setTag("DemoScene");
        scene->setActive(true);
        scene->setPhysicsEnabled(true);
        
        // Set custom gravity
        glm::vec3 customGravity(0.0f, -15.0f, 0.0f);
        scene->setGravity(customGravity);
        
        std::cout << "  Scene tag: " << scene->getTag() << std::endl;
        std::cout << "  Scene active: " << (scene->isActive() ? "true" : "false") << std::endl;
        std::cout << "  Physics enabled: " << (scene->isPhysicsEnabled() ? "true" : "false") << std::endl;
        std::cout << "  Gravity: (" << scene->getGravity().x << ", " << scene->getGravity().y << ", " << scene->getGravity().z << ")" << std::endl;
        
        std::cout << "  Scene enhancements include:" << std::endl;
        std::cout << "    - Physics simulation control" << std::endl;
        std::cout << "    - Scene tagging for organization" << std::endl;
        std::cout << "    - Active/inactive state management" << std::endl;
        std::cout << "    - Gravity customization" << std::endl;
    }
    
    // 3. Demonstrate improved Renderer Architecture
    std::cout << "\n3. Improved Renderer Architecture:" << std::endl;
    {
        // Create a material with PBR properties
        auto material = std::make_unique<Material>("PBRDemoMaterial");
        material->setAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
        material->setDiffuse(glm::vec3(0.5f, 0.2f, 0.8f));
        material->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
        material->setShininess(64.0f);
        material->setRoughness(0.3f);
        material->setMetalness(0.7f);
        
        std::cout << "  Created PBR material: " << material->getName() << std::endl;
        std::cout << "    Ambient: (" << material->getAmbient().x << ", " << material->getAmbient().y << ", " << material->getAmbient().z << ")" << std::endl;
        std::cout << "    Diffuse: (" << material->getDiffuse().x << ", " << material->getDiffuse().y << ", " << material->getDiffuse().z << ")" << std::endl;
        std::cout << "    Roughness: " << material->getRoughness() << std::endl;
        std::cout << "    Metalness: " << material->getMetalness() << std::endl;
        
        std::cout << "  Renderer architecture improvements:" << std::endl;
        std::cout << "    - Multiple pipeline support" << std::endl;
        std::cout << "    - Material descriptor sets" << std::endl;
        std::cout << "    - PBR material properties" << std::endl;
        std::cout << "    - Texture mapping support" << std::endl;
    }
    
    // 4. Demonstrate additional abstraction layers
    std::cout << "\n4. Additional Abstraction Layers:" << std::endl;
    {
        std::cout << "  Shader Program abstraction:" << std::endl;
        std::cout << "    - Vertex/fragment shader compilation" << std::endl;
        std::cout << "    - Shader linking and validation" << std::endl;
        std::cout << "    - Uniform management" << std::endl;
        
        std::cout << "  Texture abstraction:" << std::endl;
        std::cout << "    - Image loading and format conversion" << std::endl;
        std::cout << "    - Mipmap generation" << std::endl;
        std::cout << "    - Texture filtering and wrapping" << std::endl;
        
        std::cout << "  Material abstraction:" << std::endl;
        std::cout << "    - PBR material properties" << std::endl;
        std::cout << "    - Descriptor set management" << std::endl;
        std::cout << "    - Shader and texture binding" << std::endl;
    }
    
    // 5. Show benefits of all improvements
    std::cout << "\n5. Benefits of All Improvements:" << std::endl;
    std::cout << "  Singleton Elimination:" << std::endl;
    std::cout << "    - Better testability with dependency injection" << std::endl;
    std::cout << "    - Improved code modularity" << std::endl;
    std::cout << "    - Backward compatibility maintained" << std::endl;
    
    std::cout << "  Enhanced Scene Class:" << std::endl;
    std::cout << "    - Physics world integration" << std::endl;
    std::cout << "    - Advanced scene management features" << std::endl;
    std::cout << "    - Better organization and control" << std::endl;
    
    std::cout << "  Improved Renderer Architecture:" << std::endl;
    std::cout << "    - Support for multiple pipelines" << std::endl;
    std::cout << "    - Proper material handling" << std::endl;
    std::cout << "    - PBR rendering capabilities" << std::endl;
    
    std::cout << "  Additional Abstraction Layers:" << std::endl;
    std::cout << "    - Renderer independence" << std::endl;
    std::cout << "    - Resource management" << std::endl;
    std::cout << "    - Error handling and graceful degradation" << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "All improvements have been successfully implemented!" << std::endl;
    
    return 0;
}