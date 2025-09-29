#include "../include/RenderSystem.h"
#include "../include/RenderComponent.h"
#include "../include/Logger.h"
#include "../include/MeshRenderer.h"
#include <algorithm>

namespace Sparky {

    RenderSystem::RenderSystem() : renderer(nullptr) {
        SPARKY_LOG_DEBUG("RenderSystem created");
    }

    RenderSystem::~RenderSystem() {
        SPARKY_LOG_DEBUG("RenderSystem destroyed");
    }

    void RenderSystem::initialize(VulkanRenderer* renderer) {
        this->renderer = renderer;
        SPARKY_LOG_DEBUG("RenderSystem initialized with VulkanRenderer");
    }

    void RenderSystem::cleanup() {
        gameObjects.clear();
        SPARKY_LOG_DEBUG("RenderSystem cleaned up");
    }

    void RenderSystem::registerGameObject(GameObject* gameObject) {
        if (std::find(gameObjects.begin(), gameObjects.end(), gameObject) == gameObjects.end()) {
            gameObjects.push_back(gameObject);
            SPARKY_LOG_DEBUG("GameObject registered with RenderSystem: " + gameObject->getName());
        }
    }

    void RenderSystem::unregisterGameObject(GameObject* gameObject) {
        auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
        if (it != gameObjects.end()) {
            gameObjects.erase(it);
            SPARKY_LOG_DEBUG("GameObject unregistered from RenderSystem: " + gameObject->getName());
        }
    }

    void RenderSystem::update(float deltaTime) {
        // Update all registered game objects
        for (auto& gameObject : gameObjects) {
            gameObject->update(deltaTime);
        }
    }

    void RenderSystem::render() {
        if (!renderer) {
            SPARKY_LOG_ERROR("RenderSystem::render() called without initialized renderer");
            return;
        }
        
        // Render all registered game objects
        for (auto& gameObject : gameObjects) {
            renderGameObject(gameObject);
        }
        
        // Tell the renderer to render all meshes
        renderer->renderMeshes();
    }

    void RenderSystem::renderGameObject(GameObject* gameObject) {
        if (!gameObject) {
            return;
        }
        
        // Get the render component
        RenderComponent* renderComponent = gameObject->getComponent<RenderComponent>();
        if (!renderComponent || !renderComponent->isVisible()) {
            return;
        }
        
        // Get the mesh and material
        Mesh* mesh = renderComponent->getMesh();
        Material* material = renderComponent->getMaterial();
        
        if (!mesh) {
            return;
        }
        
        // In a full implementation, we would:
        // 1. Bind the material's descriptor set
        // 2. Set the push constants for the model matrix
        // 3. Draw the mesh
        // 
        // For now, we'll just log that we're rendering
        SPARKY_LOG_DEBUG("Rendering GameObject: " + gameObject->getName() + 
                        " with " + std::to_string(mesh->getVertexCount()) + " vertices");
    }
}