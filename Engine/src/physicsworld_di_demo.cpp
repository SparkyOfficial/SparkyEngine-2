#include "../include/PhysicsWorld.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

// Demo to showcase PhysicsWorld dependency injection
int main() {
    std::cout << "=== PhysicsWorld Dependency Injection Demo ===" << std::endl;
    
    // Example 1: Using singleton (backward compatibility)
    std::cout << "\n--- Example 1: Singleton (Backward Compatibility) ---" << std::endl;
    Sparky::PhysicsWorld& singletonWorld = Sparky::PhysicsWorld::getInstance();
    glm::vec3 earthGravity = singletonWorld.getGravity();
    std::cout << "Singleton PhysicsWorld gravity: (" << earthGravity.x << ", " << earthGravity.y << ", " << earthGravity.z << ")" << std::endl;
    
    // Example 2: Creating PhysicsWorld with dependency injection
    std::cout << "\n--- Example 2: Dependency Injection ---" << std::endl;
    auto customWorld = std::make_unique<Sparky::PhysicsWorld>(glm::vec3(0.0f, -1.6f, 0.0f)); // Moon gravity
    glm::vec3 moonGravity = customWorld->getGravity();
    std::cout << "Custom PhysicsWorld gravity: (" << moonGravity.x << ", " << moonGravity.y << ", " << moonGravity.z << ")" << std::endl;
    
    // Example 3: Using factory method
    std::cout << "\n--- Example 3: Factory Method ---" << std::endl;
    auto factoryWorld = Sparky::PhysicsWorld::create(glm::vec3(0.0f, -20.0f, 0.0f)); // Custom gravity
    glm::vec3 customGravity = factoryWorld->getGravity();
    std::cout << "Factory PhysicsWorld gravity: (" << customGravity.x << ", " << customGravity.y << ", " << customGravity.z << ")" << std::endl;
    
    // Example 4: Multiple PhysicsWorld instances
    std::cout << "\n--- Example 4: Multiple Instances ---" << std::endl;
    auto world1 = Sparky::PhysicsWorld::create(glm::vec3(0.0f, -9.81f, 0.0f)); // Earth
    auto world2 = Sparky::PhysicsWorld::create(glm::vec3(0.0f, -3.7f, 0.0f));  // Mars
    auto world3 = Sparky::PhysicsWorld::create(glm::vec3(0.0f, -23.1f, 0.0f)); // Jupiter
    
    std::cout << "Earth simulation gravity: (" << world1->getGravity().y << ")" << std::endl;
    std::cout << "Mars simulation gravity: (" << world2->getGravity().y << ")" << std::endl;
    std::cout << "Jupiter simulation gravity: (" << world3->getGravity().y << ")" << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}