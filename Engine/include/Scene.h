#pragma once

#include "GameObject.h"
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {
    class RenderSystem;
    class PhysicsWorld;
    
    class Scene {
    public:
        Scene();
        ~Scene();

        void update(float deltaTime);
        void render();
        
        // GameObject management
        void addGameObject(std::unique_ptr<GameObject> object);
        void removeGameObject(const std::string& name);
        GameObject* getGameObject(const std::string& name);
        
        // Scene lifecycle
        void initialize(RenderSystem* renderSystem, PhysicsWorld* physicsWorld);
        void cleanup();
        
        // Scene management
        void setActive(bool active);
        bool isActive() const;
        
        // Physics simulation control
        void setPhysicsEnabled(bool enabled);
        bool isPhysicsEnabled() const;
        
        // Gravity control
        void setGravity(const glm::vec3& gravity);
        glm::vec3 getGravity() const;
        
        // Scene tagging for organization
        void setTag(const std::string& tag);
        const std::string& getTag() const;
        
        // Get all game objects
        const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const { return gameObjects; }
        
        // Scene statistics
        size_t getGameObjectCount() const;

    private:
        std::vector<std::unique_ptr<GameObject>> gameObjects;
        RenderSystem* renderSystem;
        PhysicsWorld* physicsWorld;
        bool active;
        bool physicsEnabled;
        glm::vec3 gravity;
        std::string tag;
        
        // Helper methods
        void registerGameObjectWithSystems(GameObject* object);
        void unregisterGameObjectFromSystems(GameObject* object);
    };
}