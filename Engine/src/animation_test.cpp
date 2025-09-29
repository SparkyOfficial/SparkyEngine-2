#include "../include/Animation.h"
#include "../include/AnimationController.h"
#include "../include/GameObject.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

int main() {
    SPARKY_LOG_INFO("Starting Animation Test");
    
    // Create a game object
    auto object = std::make_unique<Sparky::GameObject>("AnimatedObject");
    
    // Create an animation
    auto animation = std::make_unique<Sparky::Animation>("Bounce");
    
    // Add keyframes
    Sparky::KeyFrame key1;
    key1.time = 0.0f;
    key1.position = glm::vec3(0.0f, 0.0f, 0.0f);
    key1.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    key1.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    animation->addKeyFrame(key1);
    
    Sparky::KeyFrame key2;
    key2.time = 1.0f;
    key2.position = glm::vec3(0.0f, 2.0f, 0.0f);
    key2.rotation = glm::vec3(0.0f, 180.0f, 0.0f);
    key2.scale = glm::vec3(1.5f, 1.5f, 1.5f);
    animation->addKeyFrame(key2);
    
    Sparky::KeyFrame key3;
    key3.time = 2.0f;
    key3.position = glm::vec3(0.0f, 0.0f, 0.0f);
    key3.rotation = glm::vec3(0.0f, 360.0f, 0.0f);
    key3.scale = glm::vec3(1.0f, 1.0f, 1.0f);
    animation->addKeyFrame(key3);
    
    // Create animation controller
    auto controller = object->addComponent<Sparky::AnimationController>();
    controller->addAnimation(std::move(animation));
    controller->setAnimation("Bounce");
    controller->setLooping(true);
    controller->setPlaybackSpeed(1.0f);
    controller->play();
    
    // Simulate for 5 seconds
    float deltaTime = 1.0f / 60.0f; // 60 FPS
    for (int i = 0; i < 300; i++) { // 300 frames = 5 seconds
        controller->update(deltaTime);
        
        // Print position every 60 frames (1 second)
        if (i % 60 == 0) {
            glm::vec3 pos = object->getPosition();
            glm::vec3 rot = object->getRotation();
            glm::vec3 scale = object->getScale();
            SPARKY_LOG_INFO("Frame " + std::to_string(i) + 
                          " - Position: (" + std::to_string(pos.x) + ", " + 
                          std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")" +
                          " Rotation: (" + std::to_string(rot.x) + ", " + 
                          std::to_string(rot.y) + ", " + std::to_string(rot.z) + ")" +
                          " Scale: (" + std::to_string(scale.x) + ", " + 
                          std::to_string(scale.y) + ", " + std::to_string(scale.z) + ")");
        }
    }
    
    SPARKY_LOG_INFO("Animation Test Completed");
    return 0;
}