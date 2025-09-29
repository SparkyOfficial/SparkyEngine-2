#pragma once

#include <vector>
#include <memory>
#include "VulkanRenderer.h"
#include "GameObject.h"

namespace Sparky {
    class RenderSystem {
    public:
        RenderSystem();
        ~RenderSystem();

        void initialize(VulkanRenderer* renderer);
        void cleanup();

        void registerGameObject(GameObject* gameObject);
        void unregisterGameObject(GameObject* gameObject);
        
        void update(float deltaTime);
        void render();
        
        // Getter for game objects
        const std::vector<GameObject*>& getGameObjects() const { return gameObjects; }

    private:
        VulkanRenderer* renderer;
        std::vector<GameObject*> gameObjects;
        
        void renderGameObject(GameObject* gameObject);
    };
}