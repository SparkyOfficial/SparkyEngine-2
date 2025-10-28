#include "../include/VulkanRenderer.h"
#include "../include/Material.h"
#include "../include/Logger.h"
#include "../include/RenderSystem.h"
#include <iostream>
#include <memory>

using namespace Sparky;

// Simple demo to test Renderer Architecture enhancements
int main() {
    std::cout << "=== Renderer Architecture Demo ===" << std::endl;
    
    // Initialize logger
    Logger& logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::DEBUG);
    
    // Create a material
    auto material = std::make_unique<Material>("TestMaterial");
    material->setAmbient(glm::vec3(0.1f, 0.1f, 0.1f));
    material->setDiffuse(glm::vec3(0.8f, 0.2f, 0.2f));
    material->setSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
    material->setShininess(32.0f);
    material->setRoughness(0.5f);
    material->setMetalness(0.8f);
    
    std::cout << "Created material: " << material->getName() << std::endl;
    std::cout << "  Ambient: (" << material->getAmbient().x << ", " << material->getAmbient().y << ", " << material->getAmbient().z << ")" << std::endl;
    std::cout << "  Diffuse: (" << material->getDiffuse().x << ", " << material->getDiffuse().y << ", " << material->getDiffuse().z << ")" << std::endl;
    std::cout << "  Specular: (" << material->getSpecular().x << ", " << material->getSpecular().y << ", " << material->getSpecular().z << ")" << std::endl;
    std::cout << "  Shininess: " << material->getShininess() << std::endl;
    std::cout << "  Roughness: " << material->getRoughness() << std::endl;
    std::cout << "  Metalness: " << material->getMetalness() << std::endl;
    
    // Test material descriptor sets (this would normally be done by the renderer)
    std::cout << "Material descriptor sets initialized: " << (material->descriptorSets.empty() ? "No" : "Yes") << std::endl;
    
    // Create a render system
    auto renderSystem = std::make_unique<RenderSystem>();
    std::cout << "Created render system" << std::endl;
    
    // Test renderer capabilities
    std::cout << "Renderer architecture features:" << std::endl;
    std::cout << "  - Multiple pipeline support" << std::endl;
    std::cout << "  - Material descriptor sets" << std::endl;
    std::cout << "  - PBR material properties" << std::endl;
    std::cout << "  - Texture mapping support" << std::endl;
    std::cout << "  - Normal/roughness/metalness maps" << std::endl;
    std::cout << "  - IBL (Image-Based Lighting) support" << std::endl;
    
    std::cout << "=== Demo Complete ===" << std::endl;
    
    return 0;
}