#include "../include/Material.h"
#include "../include/Logger.h"
#include <iostream>

int main() {
    // Initialize logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing PBR Material implementation..." << std::endl;
    
    // Create a PBR material
    Sparky::Material pbrMaterial("TestPBRMaterial");
    
    // Set PBR properties
    pbrMaterial.setRoughness(0.3f);
    pbrMaterial.setMetalness(0.8f);
    
    // Test getters
    float roughness = pbrMaterial.getRoughness();
    float metalness = pbrMaterial.getMetalness();
    
    std::cout << "Material name: " << pbrMaterial.getName() << std::endl;
    std::cout << "Roughness: " << roughness << std::endl;
    std::cout << "Metalness: " << metalness << std::endl;
    
    if (roughness == 0.3f && metalness == 0.8f) {
        std::cout << "PBR Material test PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "PBR Material test FAILED!" << std::endl;
        return 1;
    }
}