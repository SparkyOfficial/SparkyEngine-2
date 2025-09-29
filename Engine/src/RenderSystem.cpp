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
            SPARKY_LOG_INFO("Registered game object: " + gameObject->getName());
            
            // Create vertex and index buffers for the mesh if it has a render component
            if (renderer) {
                RenderComponent* renderComponent = gameObject->getComponent<RenderComponent>();
                if (renderComponent && renderComponent->getMesh()) {
                    MeshRenderer& meshRenderer = renderer->getMeshRenderer();
                    const Mesh* mesh = renderComponent->getMesh();
                    
                    SPARKY_LOG_INFO("Creating vertex buffer for mesh with " + std::to_string(mesh->getVertices().size()) + " vertices");
                    
                    // Create vertex buffer
                    meshRenderer.createVertexBuffer(*mesh);
                    
                    // Create index buffer if the mesh has indices
                    if (!mesh->getIndices().empty()) {
                        SPARKY_LOG_INFO("Creating index buffer for mesh with " + std::to_string(mesh->getIndices().size()) + " indices");
                        meshRenderer.createIndexBuffer(*mesh);
                    }
                }
            }
        }
    }

    void RenderSystem::unregisterGameObject(GameObject* gameObject) {
        if (!gameObject) {
            return;
        }

        auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
        if (it != gameObjects.end()) {
            gameObjects.erase(it);
            SPARKY_LOG_INFO("Unregistered game object: " + gameObject->getName());
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
        
        // Log the number of game objects
        static int frameCount = 0;
        frameCount++;
        if (frameCount % 60 == 0) { // Log every 60 frames
            SPARKY_LOG_DEBUG("RenderSystem has " + std::to_string(gameObjects.size()) + " game objects registered");
        }

        // Render all registered game objects that have render components
        int objectsProcessed = 0;
        for (GameObject* gameObject : gameObjects) {
            if (gameObject) {
                objectsProcessed++;
                renderGameObject(gameObject);
            }
        }
        
        if (frameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("RenderSystem processed " + std::to_string(objectsProcessed) + " game objects");
        }
    }

    void RenderSystem::renderGameObject(GameObject* gameObject) {
        if (!gameObject || !renderer) {
            return;
        }

        // Get the render component from the game object
        RenderComponent* renderComponent = gameObject->getComponent<RenderComponent>();
        if (renderComponent && renderComponent->getMesh()) {
            // Log object rendering
            static int frameCount = 0;
            frameCount++;
            if (frameCount % 300 == 0) { // Log every 300 frames
                SPARKY_LOG_DEBUG("Rendering game object: " + gameObject->getName() + 
                               " with " + std::to_string(renderComponent->getMesh()->getVertices().size()) + " vertices");
            }
            // We don't actually render here anymore - the VulkanRenderer handles this in recordCommandBuffer
            // This method is kept for future expansion if needed
        } else {
            // Log if object doesn't have a render component
            static int frameCount = 0;
            frameCount++;
            if (frameCount % 300 == 0) {
                SPARKY_LOG_DEBUG("Skipping game object: " + gameObject->getName() + " (no render component or mesh)");
            }
        }
    }
}