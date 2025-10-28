#include "../include/SparkyEngine.h"
#include "../include/AssetManager.h"
#include "../include/GameObject.h"
#include "../include/RenderComponent.h"
#include "../include/Mesh.h"
#include "../include/Material.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

/**
 * @brief Asset Manager Demo
 * 
 * This demo showcases the AssetManager abstraction layer that hides
 * the details of the underlying rendering system (Vulkan, OpenGL, etc.)
 * from the game code.
 * 
 * Instead of directly accessing engine->getRenderer().getMeshRenderer(),
 * game code can use AssetManager to create meshes and buffers.
 */

namespace Sparky {
    class AssetManagerDemo {
    private:
        Engine m_engine;
        std::unique_ptr<GameObject> m_cubeObject;
        std::unique_ptr<GameObject> m_sphereObject;
        AssetManager m_assetManager;
        bool m_initialized;
        bool m_running;
        
    public:
        AssetManagerDemo() : m_assetManager(&m_engine), m_initialized(false), m_running(false) {
        }
        
        ~AssetManagerDemo() {
            shutdown();
        }
        
        bool initialize() {
            Logger::getInstance().info("Initializing Asset Manager Demo");
            
            // Initialize engine
            if (!m_engine.initialize(1280, 720, "Sparky Engine - Asset Manager Demo")) {
                Logger::getInstance().error("Failed to initialize engine");
                return false;
            }
            
            // Create game objects using AssetManager
            createObjects();
            
            m_initialized = true;
            m_running = true;
            
            Logger::getInstance().info("Asset Manager Demo initialized successfully");
            return true;
        }
        
        void run() {
            if (!m_initialized) {
                Logger::getInstance().error("Demo not initialized");
                return;
            }
            
            Logger::getInstance().info("Starting main game loop");
            
            float lastTime = 0.0f;
            
            while (m_running && !m_engine.getWindowManager().shouldClose()) {
                // Calculate delta time
#ifdef HAS_GLFW
                float currentTime = static_cast<float>(glfwGetTime());
#else
                static float fallbackTime = 0.0f;
                fallbackTime += 0.016f; // Assume 60 FPS
                float currentTime = fallbackTime;
#endif
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;
                
                // Poll events
                m_engine.getWindowManager().pollEvents();
                
                // Update input
                updateInput(deltaTime);
                
                // Update game objects
                updateGameObjects(deltaTime);
                
                // Render frame
                m_engine.getRenderer().render();
                
                // Update shouldClose flag
                m_running = !m_engine.getWindowManager().shouldClose();
            }
            
            Logger::getInstance().info("Exiting main game loop");
        }
        
        void shutdown() {
            if (!m_initialized) return;
            
            Logger::getInstance().info("Shutting down Asset Manager Demo");
            
            // Clean up game objects
            m_cubeObject.reset();
            m_sphereObject.reset();
            
            // Shutdown engine
            m_engine.shutdown();
            
            m_initialized = false;
            Logger::getInstance().info("Asset Manager Demo shut down successfully");
        }
        
    private:
        void createObjects() {
            Logger::getInstance().info("Creating objects using AssetManager");
            
            // Create cube object
            m_cubeObject = std::make_unique<GameObject>("Cube");
            m_cubeObject->setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
            m_cubeObject->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
            
            // Add render component
            RenderComponent* cubeRender = m_cubeObject->addComponent<RenderComponent>();
            
            // Create cube mesh using AssetManager
            std::unique_ptr<Mesh> cubeMesh = m_assetManager.createCube(1.0f);
            if (cubeMesh) {
                // Create buffers using AssetManager (abstracted from renderer details)
                m_assetManager.createMeshBuffers(*cubeMesh);
                cubeRender->setMesh(std::move(cubeMesh));
            }
            
            // Add material
            std::unique_ptr<Material> cubeMaterial = std::make_unique<Material>("CubeMaterial");
            cubeMaterial->setDiffuse(glm::vec3(1.0f, 0.0f, 0.0f)); // Red
            cubeRender->setMaterial(std::move(cubeMaterial));
            
            // Register with render system
            m_engine.getRenderSystem().registerGameObject(m_cubeObject.get());
            
            // Create sphere object
            m_sphereObject = std::make_unique<GameObject>("Sphere");
            m_sphereObject->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
            m_sphereObject->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
            
            // Add render component
            RenderComponent* sphereRender = m_sphereObject->addComponent<RenderComponent>();
            
            // Create sphere mesh using AssetManager
            std::unique_ptr<Mesh> sphereMesh = m_assetManager.createSphere(1.0f, 16, 16);
            if (sphereMesh) {
                // Create buffers using AssetManager (abstracted from renderer details)
                m_assetManager.createMeshBuffers(*sphereMesh);
                sphereRender->setMesh(std::move(sphereMesh));
            }
            
            // Add material
            std::unique_ptr<Material> sphereMaterial = std::make_unique<Material>("SphereMaterial");
            sphereMaterial->setDiffuse(glm::vec3(0.0f, 0.0f, 1.0f)); // Blue
            sphereRender->setMaterial(std::move(sphereMaterial));
            
            // Register with render system
            m_engine.getRenderSystem().registerGameObject(m_sphereObject.get());
            
            Logger::getInstance().info("Objects created successfully using AssetManager");
        }
        
        void updateInput(float deltaTime) {
            InputManager& inputManager = m_engine.getInputManager();
            
            // Handle exit
            if (inputManager.isKeyJustPressed(GLFW_KEY_ESCAPE)) {
                m_running = false;
            }
        }
        
        void updateGameObjects(float deltaTime) {
            // Rotate objects
            if (m_cubeObject) {
                glm::vec3 rotation = m_cubeObject->getRotation();
                rotation.y += 45.0f * deltaTime;
                m_cubeObject->setRotation(rotation);
                m_cubeObject->update(deltaTime);
            }
            
            if (m_sphereObject) {
                glm::vec3 rotation = m_sphereObject->getRotation();
                rotation.y += 45.0f * deltaTime;
                m_sphereObject->setRotation(rotation);
                m_sphereObject->update(deltaTime);
            }
        }
    };
}

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    Sparky::Logger::getInstance().info("Starting Sparky Engine - Asset Manager Demo");
    
    Sparky::AssetManagerDemo demo;
    
    if (!demo.initialize()) {
        Sparky::Logger::getInstance().error("Failed to initialize demo");
        return -1;
    }
    
    demo.run();
    
    Sparky::Logger::getInstance().info("Demo finished successfully");
    return 0;
}