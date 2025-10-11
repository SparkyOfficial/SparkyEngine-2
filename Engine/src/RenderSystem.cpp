#include "../include/RenderSystem.h"
#include "../include/RenderComponent.h"
#include "../include/Logger.h"
#include "../include/MeshRenderer.h"
#include "../include/Material.h"
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
        
        SPARKY_LOG_DEBUG("RenderSystem::render() called with " + std::to_string(gameObjects.size()) + " game objects");
        
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
        
        SPARKY_LOG_DEBUG("Processing GameObject: " + gameObject->getName());
        
        // Get the render component
        RenderComponent* renderComponent = gameObject->getComponent<RenderComponent>();
        if (!renderComponent || !renderComponent->isVisible()) {
            SPARKY_LOG_DEBUG("No render component or not visible for GameObject: " + gameObject->getName());
            return;
        }
        
        // Get the mesh and material
        Mesh* mesh = renderComponent->getMesh();
        Material* material = renderComponent->getMaterial();
        
        if (!mesh) {
            SPARKY_LOG_DEBUG("No mesh found for GameObject: " + gameObject->getName());
            return;
        }
        
        SPARKY_LOG_DEBUG("GameObject " + gameObject->getName() + " has mesh with " + std::to_string(mesh->getVertexCount()) + " vertices");
        
        // Initialize material with descriptor sets if needed (only when GLFW is available)
#ifdef HAS_GLFW
        if (material && renderer) {
            SPARKY_LOG_DEBUG("GameObject " + gameObject->getName() + " has material: " + material->getName());
            // Check if material needs to be initialized
            if (material->descriptorSets.empty()) {
                SPARKY_LOG_DEBUG("Initializing material descriptor sets for: " + material->getName());
                renderer->createMaterialDescriptorSets(material);
            } else {
                SPARKY_LOG_DEBUG("Material descriptor sets already initialized for: " + material->getName());
            }
        } else {
            if (!material) {
                SPARKY_LOG_DEBUG("No material found for GameObject: " + gameObject->getName());
            }
            if (!renderer) {
                SPARKY_LOG_DEBUG("No renderer found for GameObject: " + gameObject->getName());
            }
        }
#endif
    
        // In a full implementation, we would:
        // 1. Bind the material's descriptor set
        // 2. Set the push constants for the model matrix
        // 3. Draw the mesh
        // 
        // For now, we'll just log that we're rendering
        SPARKY_LOG_DEBUG("Rendering GameObject: " + gameObject->getName() + 
                        " with " + std::to_string(mesh->getVertexCount()) + " vertices");
    }

    void RenderSystem::initializeMaterial(Material* material) {
        if (!material || !renderer) {
            return;
        }
        
        // Initialize material with descriptor sets if needed (only when GLFW is available)
#ifdef HAS_GLFW
        // Check if material needs to be initialized
        if (material->descriptorSets.empty()) {
            SPARKY_LOG_DEBUG("Initializing material descriptor sets for: " + material->getName());
            renderer->createMaterialDescriptorSets(material);
        } else {
            SPARKY_LOG_DEBUG("Material descriptor sets already initialized for: " + material->getName());
        }
#endif
    }
}