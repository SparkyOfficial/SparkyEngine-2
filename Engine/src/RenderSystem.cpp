#include "../include/RenderSystem.h"
#include "../include/RenderComponent.h"
#include "../include/Logger.h"
#include <algorithm>

namespace Sparky {

    RenderSystem::RenderSystem() : renderer(nullptr) {
    }

    RenderSystem::~RenderSystem() {
        cleanup();
    }

    void RenderSystem::initialize(VulkanRenderer* renderer) {
        this->renderer = renderer;
        SPARKY_LOG_INFO("RenderSystem initialized");
    }

    void RenderSystem::cleanup() {
        gameObjects.clear();
        renderer = nullptr;
        SPARKY_LOG_INFO("RenderSystem cleaned up");
    }

    void RenderSystem::registerGameObject(GameObject* gameObject) {
        if (!gameObject) {
            return;
        }

        // Check if the object is already registered
        auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
        if (it == gameObjects.end()) {
            gameObjects.push_back(gameObject);
            SPARKY_LOG_DEBUG("Registered game object: " + gameObject->getName());
        }
    }

    void RenderSystem::unregisterGameObject(GameObject* gameObject) {
        if (!gameObject) {
            return;
        }

        auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
        if (it != gameObjects.end()) {
            gameObjects.erase(it);
            SPARKY_LOG_DEBUG("Unregistered game object: " + gameObject->getName());
        }
    }

    void RenderSystem::update(float deltaTime) {
        // Render system doesn't need update logic in this implementation
        // But we could add logic here for things like frustum culling, LOD, etc.
    }

    void RenderSystem::render() {
        if (!renderer) {
            SPARKY_LOG_ERROR("RenderSystem not initialized with a renderer");
            return;
        }

        // Get the mesh renderer from the Vulkan renderer
        MeshRenderer& meshRenderer = renderer->getMeshRenderer();

        // Render all registered game objects that have render components
        for (GameObject* gameObject : gameObjects) {
            if (gameObject) {
                renderGameObject(gameObject);
            }
        }
    }

    void RenderSystem::renderGameObject(GameObject* gameObject) {
        if (!gameObject || !renderer) {
            return;
        }

        // Get the render component from the game object
        RenderComponent* renderComponent = gameObject->getComponent<RenderComponent>();
        if (renderComponent && renderComponent->getMesh()) {
            // In a real implementation, we would pass the actual command buffer
            // For now, we'll use VK_NULL_HANDLE as a placeholder
            MeshRenderer& meshRenderer = renderer->getMeshRenderer();
            meshRenderer.renderMesh(*renderComponent->getMesh(), VK_NULL_HANDLE);
        }
    }
}