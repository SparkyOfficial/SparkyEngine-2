#include "../include/EngineDemo.h"
#include "../include/VulkanRenderer.h"
#include "../include/WindowManager.h"
#include "../include/InputManager.h"
#include "../include/Camera.h"
#include "../include/GameObject.h"
#include "../include/PhysicsWorld.h"
#include "../include/AudioEngine.h"
#include "../include/LightManager.h"
#include "../include/GUIManager.h"
#include "../include/QuestManager.h"
#include "../include/Logger.h"
#include "../include/Config.h"
#include <GLFW/glfw3.h>

namespace Sparky {

    EngineDemo::EngineDemo() : running(false), initialized(false) {
        SPARKY_LOG_INFO("Creating Engine Demo");
    }

    EngineDemo::~EngineDemo() {
        shutdown();
    }

    bool EngineDemo::initialize() {
        try {
            // Initialize configuration
            Config::getInstance().loadFromFile("engine.cfg");
            
            // Initialize core systems
            windowManager = std::make_unique<WindowManager>();
            if (!windowManager->initialize(1280, 720, "Sparky Engine Demo")) {
                SPARKY_LOG_ERROR("Failed to initialize window manager");
                return false;
            }
            
            renderer = std::make_unique<VulkanRenderer>();
            if (!renderer->initialize(windowManager->getWindowHandle())) {
                SPARKY_LOG_ERROR("Failed to initialize renderer");
                return false;
            }
            
            inputManager = std::make_unique<InputManager>();
            inputManager->initialize(static_cast<GLFWwindow*>(windowManager->getWindowHandle()));
            
            camera = std::make_unique<Camera>();
            
            // Initialize game systems
            physicsWorld = std::make_unique<PhysicsWorld>();
            audioEngine = std::make_unique<AudioEngine>();
            lightManager = std::make_unique<LightManager>();
            guiManager = std::make_unique<GUIManager>();
            questManager = std::make_unique<QuestManager>();
            
            // Setup scene
            setupScene();
            setupLighting();
            setupAudio();
            setupGUI();
            setupQuests();
            
            initialized = true;
            SPARKY_LOG_INFO("Engine Demo initialized successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to initialize Engine Demo: " + std::string(e.what()));
            return false;
        }
    }

    void EngineDemo::run() {
        if (!initialized) {
            SPARKY_LOG_ERROR("Engine Demo not initialized");
            return;
        }
        
        running = true;
        float lastTime = 0.0f;
        
        SPARKY_LOG_INFO("Starting Engine Demo");
        
        while (running && !windowManager->shouldClose()) {
            // Calculate delta time
            float currentTime = static_cast<float>(glfwGetTime());
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;
            
            // Poll events
            windowManager->pollEvents();
            
            // Update input
            inputManager->update();
            
            // Handle input
            handleInput();
            
            // Update systems
            updateSystems(deltaTime);
            
            // Render
            renderSystems();
        }
        
        SPARKY_LOG_INFO("Engine Demo finished");
    }

    void EngineDemo::shutdown() {
        if (!initialized) return;
        
        // Clean up systems
        questManager.reset();
        guiManager.reset();
        lightManager.reset();
        audioEngine.reset();
        physicsWorld.reset();
        
        // Clean up core systems
        camera.reset();
        inputManager.reset();
        renderer.reset();
        windowManager.reset();
        
        initialized = false;
        SPARKY_LOG_INFO("Engine Demo shut down");
    }

    void EngineDemo::setupScene() {
        SPARKY_LOG_DEBUG("Setting up demo scene");
        
        // Create player
        player = std::make_unique<GameObject>();
        player->setName("Player");
        player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        
        // Create enemy
        enemy = std::make_unique<GameObject>();
        enemy->setName("Enemy");
        enemy->setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
        
        SPARKY_LOG_DEBUG("Scene setup complete");
    }

    void EngineDemo::setupLighting() {
        SPARKY_LOG_DEBUG("Setting up lighting");
        
        // Create a directional light
        auto dirLight = lightManager->createDirectionalLight("Sun");
        if (dirLight) {
            dirLight->setDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
            dirLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
            dirLight->setIntensity(1.0f);
        }
        
        SPARKY_LOG_DEBUG("Lighting setup complete");
    }

    void EngineDemo::setupAudio() {
        SPARKY_LOG_DEBUG("Setting up audio");
        
        if (audioEngine->initialize()) {
            SPARKY_LOG_DEBUG("Audio engine initialized");
        } else {
            SPARKY_LOG_WARNING("Failed to initialize audio engine");
        }
        
        SPARKY_LOG_DEBUG("Audio setup complete");
    }

    void EngineDemo::setupGUI() {
        SPARKY_LOG_DEBUG("Setting up GUI");
        
        // Create a simple button
        auto button = guiManager->createButton("DemoButton");
        if (button) {
            button->setPosition(glm::vec2(100.0f, 100.0f));
            button->setSize(glm::vec2(200.0f, 50.0f));
            button->setText("Click Me!");
        }
        
        SPARKY_LOG_DEBUG("GUI setup complete");
    }

    void EngineDemo::setupQuests() {
        SPARKY_LOG_DEBUG("Setting up quests");
        
        // This would create quests in a real implementation
        SPARKY_LOG_DEBUG("Quests setup complete");
    }

    void EngineDemo::handleInput() {
        // Check for exit
        if (inputManager->isKeyJustPressed(256)) { // ESC key
            running = false;
            SPARKY_LOG_INFO("Exit requested");
        }
        
        // Handle player movement
        if (player) {
            glm::vec3 movement(0.0f);
            
            if (inputManager->isKeyPressed(87)) { // W
                movement.z -= 1.0f;
            }
            if (inputManager->isKeyPressed(83)) { // S
                movement.z += 1.0f;
            }
            if (inputManager->isKeyPressed(65)) { // A
                movement.x -= 1.0f;
            }
            if (inputManager->isKeyPressed(68)) { // D
                movement.x += 1.0f;
            }
            
            if (glm::length(movement) > 0.0f) {
                movement = glm::normalize(movement);
                glm::vec3 position = player->getPosition();
                position += movement * 5.0f * 0.016f; // Assuming 60 FPS
                player->setPosition(position);
            }
        }
    }

    void EngineDemo::updateSystems(float deltaTime) {
        // Update physics
        if (physicsWorld) {
            physicsWorld->update(deltaTime);
        }
        
        // Update GUI
        if (guiManager) {
            guiManager->update(deltaTime);
        }
        
        // Update quests
        if (questManager) {
            questManager->update(deltaTime);
        }
        
        // Update camera to follow player
        if (camera && player) {
            camera->setPosition(player->getPosition() + glm::vec3(0.0f, 2.0f, 5.0f));
        }
    }

    void EngineDemo::renderSystems() {
        // Proper scene rendering implementation using the RenderSystem
        if (renderer) {
            // The renderer now handles all scene rendering through the RenderSystem
            renderer->render();
        }
        
        // Render GUI
        if (guiManager) {
            guiManager->render();
        }
    }
}