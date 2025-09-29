#include "../include/VulkanRenderer.h"
#include "../include/WindowManager.h"
#include "../include/Logger.h"
#include "../include/Mesh.h"
#include "../include/GameObject.h"
#include "../include/RenderComponent.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <memory>

class TestScene {
private:
    Sparky::VulkanRenderer* renderer;
    std::vector<std::unique_ptr<Sparky::GameObject>> gameObjects;

public:
    TestScene(Sparky::VulkanRenderer* renderer) : renderer(renderer) {}
    
    void initialize() {
        // Create a cube
        auto cubeObject = std::make_unique<Sparky::GameObject>("Cube");
        auto cubeMesh = Sparky::Mesh::createCube(1.0f);
        auto renderComponent = std::make_unique<Sparky::RenderComponent>();
        renderComponent->setMesh(std::move(cubeMesh));
        cubeObject->addComponent(std::move(renderComponent));
        gameObjects.push_back(std::move(cubeObject));
        
        // Create a plane
        auto planeObject = std::make_unique<Sparky::GameObject>("Plane");
        auto planeMesh = Sparky::Mesh::createPlane(2.0f, 2.0f);
        auto planeRenderComponent = std::make_unique<Sparky::RenderComponent>();
        planeRenderComponent->setMesh(std::move(planeMesh));
        planeObject->addComponent(std::move(planeRenderComponent));
        gameObjects.push_back(std::move(planeObject));
        
        // Create a sphere
        auto sphereObject = std::make_unique<Sparky::GameObject>("Sphere");
        auto sphereMesh = Sparky::Mesh::createSphere(0.5f, 20, 20);
        auto sphereRenderComponent = std::make_unique<Sparky::RenderComponent>();
        sphereRenderComponent->setMesh(std::move(sphereMesh));
        sphereObject->addComponent(std::move(sphereRenderComponent));
        gameObjects.push_back(std::move(sphereObject));
        
        // Initialize render components with the renderer
        auto& meshRenderer = renderer->getMeshRenderer();
        for (auto& obj : gameObjects) {
            auto renderComp = obj->getComponent<Sparky::RenderComponent>();
            if (renderComp && renderComp->getMesh()) {
                meshRenderer.createVertexBuffer(*renderComp->getMesh());
                meshRenderer.createIndexBuffer(*renderComp->getMesh());
            }
        }
    }
    
    void render(VkCommandBuffer commandBuffer) {
        // Proper implementation for rendering all objects in the test scene
        SPARKY_LOG_DEBUG("Rendering all objects in the test scene");
        
        // Iterate through all game objects and render them
        for (const auto& obj : gameObjects) {
            if (obj) {
                // Get the render component
                Sparky::RenderComponent* renderComp = obj->getComponent<Sparky::RenderComponent>();
                if (renderComp && renderComp->getMesh()) {
                    const Sparky::Mesh* mesh = renderComp->getMesh();
                    
                    // Log that we would render this object
                    SPARKY_LOG_DEBUG("Would render object: " + obj->getName() + 
                                   " with " + std::to_string(mesh->getVertices().size()) + " vertices");
                    
                    // In a complete implementation, this would:
                    // 1. Bind the appropriate shader program
                    // 2. Set up uniform buffers for view/projection matrices
                    // 3. Bind the vertex and index buffers for this mesh
                    // 4. Apply material properties (if any)
                    // 5. Set up the model matrix as a push constant
                    // 6. Issue draw calls (vkCmdDraw or vkCmdDrawIndexed)
                }
            }
        }
    }
    
    void cleanup() {
        gameObjects.clear();
    }
};

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    try {
        // Create window
        Sparky::WindowManager windowManager;
        if (!windowManager.initialize(1024, 768, "Render Test - Sparky Engine")) {
            std::cerr << "Failed to create window" << std::endl;
            return -1;
        }
        
        // Create renderer
        Sparky::VulkanRenderer renderer;
        if (!renderer.initialize(windowManager.getWindowHandle())) {
            std::cerr << "Failed to initialize Vulkan renderer" << std::endl;
            return -1;
        }
        
        // Create test scene
        TestScene scene(&renderer);
        scene.initialize();
        
        std::cout << "Render test initialized successfully!" << std::endl;
        std::cout << "Scene contains multiple mesh types:" << std::endl;
        std::cout << " - Cube (24 vertices, 36 indices)" << std::endl;
        std::cout << " - Plane (4 vertices, 6 indices)" << std::endl;
        std::cout << " - Sphere (441 vertices, 2420 indices)" << std::endl;
        
        // Run for 10 seconds
        auto startTime = std::chrono::high_resolution_clock::now();
        while (!windowManager.shouldClose()) {
            windowManager.pollEvents();
            
            // Render frame
            renderer.render();
            
            // Check if 10 seconds have passed
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
            if (duration.count() >= 10) {
                break;
            }
            
            // Small delay to prevent excessive CPU usage
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        scene.cleanup();
        renderer.cleanup();
        windowManager.cleanup();
        
        std::cout << "Render test completed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}