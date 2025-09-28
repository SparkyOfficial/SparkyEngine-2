#include "../include/VulkanRenderer.h"
#include "../include/WindowManager.h"
#include "../include/Logger.h"
#include "../include/Mesh.h"
#include "../include/GameObject.h"
#include "../include/RenderComponent.h"
#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/AudioEngine.h"
#include "../include/PhysicsWorld.h"
#include "../include/LightManager.h"
#include "../include/GUIManager.h"
#include "../include/Button.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <memory>

class EngineDemo {
private:
    Sparky::WindowManager windowManager;
    Sparky::VulkanRenderer renderer;
    Sparky::InputManager inputManager;
    Sparky::Camera camera;
    Sparky::AudioEngine audioEngine;
    Sparky::PhysicsWorld physicsWorld;
    Sparky::LightManager lightManager;
    Sparky::GUIManager guiManager;
    
    std::vector<std::unique_ptr<Sparky::GameObject>> sceneObjects;
    bool running;

public:
    EngineDemo() : running(false) {}
    
    bool initialize() {
        try {
            // Initialize window
            if (!windowManager.initialize(1280, 720, "Sparky Engine Demo")) {
                std::cerr << "Failed to initialize window" << std::endl;
                return false;
            }
            
            // Initialize input manager
            inputManager.initialize(static_cast<GLFWwindow*>(windowManager.getWindowHandle()));
            
            // Initialize renderer
            if (!renderer.initialize(windowManager.getWindowHandle())) {
                std::cerr << "Failed to initialize renderer" << std::endl;
                return false;
            }
            
            // Initialize audio
            if (!audioEngine.initialize()) {
                std::cerr << "Failed to initialize audio engine" << std::endl;
            }
            
            // Create scene
            createScene();
            
            running = true;
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error during initialization: " << e.what() << std::endl;
            return false;
        }
    }
    
    void createScene() {
        // Create a floor
        auto floor = std::make_unique<Sparky::GameObject>("Floor");
        floor->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
        auto floorMesh = Sparky::Mesh::createPlane(20.0f, 20.0f);
        auto floorRender = std::make_unique<Sparky::RenderComponent>();
        floorRender->setMesh(std::move(floorMesh));
        floor->addComponent(std::move(floorRender));
        sceneObjects.push_back(std::move(floor));
        
        // Create a cube
        auto cube = std::make_unique<Sparky::GameObject>("Cube");
        cube->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        auto cubeMesh = Sparky::Mesh::createCube(1.0f);
        auto cubeRender = std::make_unique<Sparky::RenderComponent>();
        cubeRender->setMesh(std::move(cubeMesh));
        cube->addComponent(std::move(cubeRender));
        sceneObjects.push_back(std::move(cube));
        
        // Create a sphere
        auto sphere = std::make_unique<Sparky::GameObject>("Sphere");
        sphere->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
        auto sphereMesh = Sparky::Mesh::createSphere(0.5f, 20, 20);
        auto sphereRender = std::make_unique<Sparky::RenderComponent>();
        sphereRender->setMesh(std::move(sphereMesh));
        sphere->addComponent(std::move(sphereRender));
        sceneObjects.push_back(std::move(sphere));
        
        // Initialize meshes with the renderer
        auto& meshRenderer = renderer.getMeshRenderer();
        for (auto& obj : sceneObjects) {
            auto renderComp = obj->getComponent<Sparky::RenderComponent>();
            if (renderComp && renderComp->getMesh()) {
                meshRenderer.createVertexBuffer(*renderComp->getMesh());
                meshRenderer.createIndexBuffer(*renderComp->getMesh());
            }
        }
        
        // Set up camera
        camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
        camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        
        // Create GUI
        createGUI();
    }
    
    void createGUI() {
        // Create a title
        auto title = guiManager.createButton("Title");
        if (title) {
            title->setPosition(glm::vec2(10.0f, 10.0f));
            title->setSize(glm::vec2(300.0f, 30.0f));
            title->setText("Sparky Engine Demo");
        }
        
        // Create FPS counter
        auto fpsCounter = guiManager.createButton("FPS");
        if (fpsCounter) {
            fpsCounter->setPosition(glm::vec2(10.0f, 50.0f));
            fpsCounter->setSize(glm::vec2(200.0f, 30.0f));
            fpsCounter->setText("FPS: 60");
        }
    }
    
    void run() {
        auto lastTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;
        auto lastFpsUpdate = lastTime;
        
        std::cout << "Engine demo running. Press ESC to exit." << std::endl;
        
        while (running && !windowManager.shouldClose()) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;
            
            // Update FPS counter
            frameCount++;
            if (std::chrono::duration<float>(currentTime - lastFpsUpdate).count() >= 1.0f) {
                auto fpsCounter = static_cast<Sparky::Button*>(guiManager.getElement("FPS"));
                if (fpsCounter) {
                    fpsCounter->setText("FPS: " + std::to_string(frameCount));
                }
                frameCount = 0;
                lastFpsUpdate = currentTime;
            }
            
            // Handle input
            handleInput(deltaTime);
            
            // Update systems
            inputManager.update();
            physicsWorld.update(deltaTime);
            guiManager.update(deltaTime);
            
            // Check for exit
            if (inputManager.isKeyJustPressed(256)) { // ESC key
                running = false;
            }
            
            // Render frame
            renderer.render();
            
            // Poll window events
            windowManager.pollEvents();
        }
    }
    
    void handleInput(float deltaTime) {
        // Camera movement
        if (inputManager.isKeyPressed(87)) { // W
            camera.moveForward(deltaTime * 5.0f);
        }
        if (inputManager.isKeyPressed(83)) { // S
            camera.moveBackward(deltaTime * 5.0f);
        }
        if (inputManager.isKeyPressed(65)) { // A
            camera.moveLeft(deltaTime * 5.0f);
        }
        if (inputManager.isKeyPressed(68)) { // D
            camera.moveRight(deltaTime * 5.0f);
        }
        
        // Camera rotation
        if (inputManager.isMouseButtonPressed(1)) { // Right mouse button
            glm::vec2 mouseDelta = inputManager.getMouseDelta();
            camera.rotate(mouseDelta.x * deltaTime * 0.1f, mouseDelta.y * deltaTime * 0.1f);
        }
    }
    
    void shutdown() {
        sceneObjects.clear();
        renderer.cleanup();
        windowManager.cleanup();
        audioEngine.cleanup();
        running = false;
    }
};

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::INFO);
    
    EngineDemo demo;
    if (!demo.initialize()) {
        std::cerr << "Failed to initialize engine demo" << std::endl;
        return -1;
    }
    
    demo.run();
    demo.shutdown();
    
    std::cout << "Engine demo completed successfully!" << std::endl;
    return 0;
}