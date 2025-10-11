#include "../include/CollisionSystem.h"
#include "../include/Logger.h"
#include <iostream>

int main() {
    // Initialize logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing Enhanced Physics System..." << std::endl;
    
    // Test AABB collision
    glm::vec3 posA(0.0f, 0.0f, 0.0f);
    glm::vec3 sizeA(2.0f, 2.0f, 2.0f);
    glm::vec3 posB(1.5f, 0.0f, 0.0f);
    glm::vec3 sizeB(2.0f, 2.0f, 2.0f);
    
    bool aabbCollision = Sparky::CollisionSystem::checkAABBCollision(posA, sizeA, posB, sizeB);
    
    std::cout << "AABB Collision Test: " << (aabbCollision ? "PASSED" : "FAILED") << std::endl;
    
    // Test sphere collision
    glm::vec3 spherePosA(0.0f, 0.0f, 0.0f);
    float radiusA = 1.0f;
    glm::vec3 spherePosB(1.5f, 0.0f, 0.0f);
    float radiusB = 1.0f;
    
    bool sphereCollision = Sparky::CollisionSystem::checkSphereCollision(spherePosA, radiusA, spherePosB, radiusB);
    
    std::cout << "Sphere Collision Test: " << (sphereCollision ? "PASSED" : "FAILED") << std::endl;
    
    // Test sphere-AABB collision
    bool sphereAABBCollision = Sparky::CollisionSystem::checkSphereAABBCollision(spherePosA, radiusA, posB, sizeB);
    
    std::cout << "Sphere-AABB Collision Test: " << (sphereAABBCollision ? "PASSED" : "FAILED") << std::endl;
    
    if (aabbCollision && sphereCollision && sphereAABBCollision) {
        std::cout << "Enhanced Physics System test PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Enhanced Physics System test FAILED!" << std::endl;
        return 1;
    }
}