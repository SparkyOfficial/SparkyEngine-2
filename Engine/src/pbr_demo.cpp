#include "../include/SparkyEngine.h"
#include "../include/GameObject.h"
#include "../include/PBRMaterial.h"
#include "../include/PBRRenderer.h"
#include "../include/RenderComponent.h"
#include "../include/Mesh.h"
#include "../include/Light.h"
#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

/**
 * @brief PBR Demo
 * 
 * This demo showcases the enhanced PBR features of the SparkyEngine3:
 * - Physically Based Rendering materials
 * - Image-Based Lighting (IBL)
 * - HDR rendering with tone mapping
 * - Normal mapping
 * - Roughness and metalness workflows
 * - Shadow mapping
 */

namespace Sparky {
    class PBRDemo {
    private:
        Engine m_engine;
        std::unique_ptr<GameObject> m_sphere;
        std::unique_ptr<GameObject> m_plane;
        std::unique_ptr<GameObject> m_cube;
        std::vector<std::unique_ptr<GameObject>> m_objects;
        std::unique_ptr<Light> m_directionalLight;
        std::vector<std::unique_ptr<Light>> m_pointLights;
        
        // Game state
        bool m_initialized;
        bool m_running;
        float m_gameTime;
        
    public:
        PBRDemo() 
            : m_initialized(false)
            , m_running(false)
            , m_gameTime(0.0f) {
        }
        
        ~PBRDemo() {
            shutdown();
        }
        
        bool initialize() {
            Logger::getInstance().info("Initializing PBR Demo");
            
            // Initialize engine
            if (!m_engine.initialize(1280, 720, "Sparky Engine 3 - PBR Demo")) {
                Logger::getInstance().error("Failed to initialize engine");
                return false;
            }
            
            // Create game objects
            createScene();
            createLights();
            
            // Set up camera
            setupCamera();
            
            m_initialized = true;
            m_running = true;
            
            Logger::getInstance().info("PBR Demo initialized successfully");
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
                
                m_gameTime += deltaTime;
                
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
            
            Logger::getInstance().info("Shutting down PBR Demo");
            
            // Clean up game objects
            m_sphere.reset();
            m_plane.reset();
            m_cube.reset();
            m_objects.clear();
            m_directionalLight.reset();
            m_pointLights.clear();
            
            // Shutdown engine
            m_engine.shutdown();
            
            m_initialized = false;
            Logger::getInstance().info("PBR Demo shut down successfully");
        }
        
    private:
        void createScene() {
            Logger::getInstance().info("Creating PBR scene");
            
            // Create a sphere with PBR material
            m_sphere = std::make_unique<GameObject>("Sphere");
            m_sphere->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
            m_sphere->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
            
            RenderComponent* sphereRender = m_sphere->addComponent<RenderComponent>();
            std::unique_ptr<Mesh> sphereMesh = Mesh::createSphere(1.0f, 32, 32);
            sphereRender->setMesh(std::move(sphereMesh));
            
            // Create PBR material for the sphere
            std::unique_ptr<PBRMaterial> sphereMaterial = std::make_unique<PBRMaterial>("SphereMaterial");
            sphereMaterial->setAlbedo(glm::vec3(1.0f, 0.5f, 0.31f)); // Copper-like color
            sphereMaterial->setMetallic(1.0f);
            sphereMaterial->setRoughness(0.1f);
            sphereMaterial->setNormalMapEnabled(true);
            sphereRender->setMaterial(std::move(sphereMaterial));
            
            m_engine.getRenderSystem().registerGameObject(m_sphere.get());
            
            // Create a plane with PBR material
            m_plane = std::make_unique<GameObject>("Plane");
            m_plane->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
            m_plane->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
            
            RenderComponent* planeRender = m_plane->addComponent<RenderComponent>();
            std::unique_ptr<Mesh> planeMesh = Mesh::createPlane(2.0f, 2.0f);
            planeRender->setMesh(std::move(planeMesh));
            
            // Create PBR material for the plane
            std::unique_ptr<PBRMaterial> planeMaterial = std::make_unique<PBRMaterial>("PlaneMaterial");
            planeMaterial->setAlbedo(glm::vec3(0.5f, 0.5f, 0.5f)); // Gray color
            planeMaterial->setMetallic(0.0f);
            planeMaterial->setRoughness(0.8f);
            planeMaterial->setNormalMapEnabled(false);
            planeRender->setMaterial(std::move(planeMaterial));
            
            m_engine.getRenderSystem().registerGameObject(m_plane.get());
            
            // Create several spheres with different materials to showcase PBR properties
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    auto sphere = std::make_unique<GameObject>("Sphere_" + std::to_string(i) + "_" + std::to_string(j));
                    sphere->setPosition(glm::vec3(-4.0f + i * 2.0f, 0.5f, -4.0f + j * 2.0f));
                    sphere->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
                    
                    RenderComponent* render = sphere->addComponent<RenderComponent>();
                    std::unique_ptr<Mesh> mesh = Mesh::createSphere(1.0f, 16, 16);
                    render->setMesh(std::move(mesh));
                    
                    // Create PBR material with varying roughness and metalness
                    std::unique_ptr<PBRMaterial> material = std::make_unique<PBRMaterial>("SphereMaterial_" + std::to_string(i) + "_" + std::to_string(j));
                    material->setAlbedo(glm::vec3(0.8f, 0.1f, 0.1f)); // Red color
                    material->setMetallic(static_cast<float>(i) / 4.0f); // Vary metallic from 0 to 1
                    material->setRoughness(static_cast<float>(j) / 4.0f); // Vary roughness from 0 to 1
                    material->setNormalMapEnabled(false);
                    render->setMaterial(std::move(material));
                    
                    m_engine.getRenderSystem().registerGameObject(sphere.get());
                    m_objects.push_back(std::move(sphere));
                }
            }
            
            Logger::getInstance().info("PBR scene created successfully");
        }
        
        void createLights() {
            Logger::getInstance().info("Creating lights");
            
            // Directional light (sun)
            m_directionalLight = std::make_unique<Light>("Sun");
            m_directionalLight->setDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
            m_directionalLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
            m_directionalLight->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
            
            // Add some point lights for more interesting lighting
            auto pointLight1 = std::make_unique<Light>("PointLight1");
            pointLight1->setPosition(glm::vec3(3.0f, 2.0f, 0.0f));
            pointLight1->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
            pointLight1->setAmbient(glm::vec3(0.1f, 0.1f, 0.1f));
            pointLight1->setConstant(1.0f);
            pointLight1->setLinear(0.09f);
            pointLight1->setQuadratic(0.032f);
            m_pointLights.push_back(std::move(pointLight1));
            
            auto pointLight2 = std::make_unique<Light>("PointLight2");
            pointLight2->setPosition(glm::vec3(-3.0f, 2.0f, 0.0f));
            pointLight2->setDiffuse(glm::vec3(0.5f, 0.5f, 1.0f));
            pointLight2->setAmbient(glm::vec3(0.05f, 0.05f, 0.1f));
            pointLight2->setConstant(1.0f);
            pointLight2->setLinear(0.09f);
            pointLight2->setQuadratic(0.032f);
            m_pointLights.push_back(std::move(pointLight2));
            
            Logger::getInstance().info("Lights created successfully");
        }
        
        void setupCamera() {
            Logger::getInstance().info("Setting up camera");
            
            Camera& camera = m_engine.getCamera();
            camera.setPosition(glm::vec3(0.0f, 3.0f, 10.0f));
            camera.setFront(glm::vec3(0.0f, 0.0f, -1.0f));
            camera.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
            camera.setYaw(-90.0f);
            camera.setPitch(0.0f);
            
            Logger::getInstance().info("Camera set up successfully");
        }
        
        void updateInput(float deltaTime) {
            InputManager& inputManager = m_engine.getInputManager();
            
            // Handle camera movement
            Camera& camera = m_engine.getCamera();
            float cameraSpeed = 5.0f * deltaTime;
            
            if (inputManager.isKeyPressed(GLFW_KEY_W)) {
                camera.processKeyboard(Camera::Movement::FORWARD, cameraSpeed);
            }
            if (inputManager.isKeyPressed(GLFW_KEY_S)) {
                camera.processKeyboard(Camera::Movement::BACKWARD, cameraSpeed);
            }
            if (inputManager.isKeyPressed(GLFW_KEY_A)) {
                camera.processKeyboard(Camera::Movement::LEFT, cameraSpeed);
            }
            if (inputManager.isKeyPressed(GLFW_KEY_D)) {
                camera.processKeyboard(Camera::Movement::RIGHT, cameraSpeed);
            }
            
            // Handle mouse look
            if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
                double xpos, ypos;
#ifdef HAS_GLFW
                glfwGetCursorPos(static_cast<GLFWwindow*>(m_engine.getWindowManager().getWindowHandle()), &xpos, &ypos);
                camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
#endif
            }
            
            // Handle exit
            if (inputManager.isKeyJustPressed(GLFW_KEY_ESCAPE)) {
                m_running = false;
            }
        }
        
        void updateGameObjects(float deltaTime) {
            // Animate the main sphere
            if (m_sphere) {
                glm::vec3 position = m_sphere->getPosition();
                position.y = 1.0f + sin(m_gameTime) * 0.5f;
                m_sphere->setPosition(position);
                m_sphere->update(deltaTime);
            }
            
            // Update plane
            if (m_plane) {
                m_plane->update(deltaTime);
            }
            
            // Update other objects
            for (auto& obj : m_objects) {
                obj->update(deltaTime);
            }
            
            // Animate lights
            if (!m_pointLights.empty()) {
                float lightIntensity = 0.5f + 0.5f * sin(m_gameTime * 2.0f);
                m_pointLights[0]->setDiffuse(glm::vec3(lightIntensity, lightIntensity * 0.8f, lightIntensity * 0.6f));
                
                // Move the second light in a circle
                float radius = 3.0f;
                float angle = m_gameTime;
                glm::vec3 newPosition(
                    radius * cos(angle),
                    2.0f,
                    radius * sin(angle)
                );
                m_pointLights[1]->setPosition(newPosition);
            }
        }
    };
}

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    Sparky::Logger::getInstance().info("Starting Sparky Engine 3 - PBR Demo");
    
    Sparky::PBRDemo demo;
    
    if (!demo.initialize()) {
        Sparky::Logger::getInstance().error("Failed to initialize demo");
        return -1;
    }
    
    demo.run();
    
    Sparky::Logger::getInstance().info("Demo finished successfully");
    return 0;
}