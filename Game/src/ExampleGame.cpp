#include "ExampleGame.h"
#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/GameObject.h"
#include "Player.h"
#include "Platform.h"
#include "../../Engine/include/Light.h"
#include "../../Engine/include/ParticleSystem.h"
#include "../../Engine/include/Inventory.h"
#include "../../Engine/include/Quest.h"
#include "../../Engine/include/GUIManager.h"
#include "../../Engine/include/BehaviorTreeExample.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/Button.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/RenderSystem.h"
#include <glm/glm.hpp>

namespace Sparky {

    ExampleGame::ExampleGame() : engine(nullptr), renderSystem(nullptr), initialized(false), paused(false),
                               score(0), health(100) {
        SPARKY_LOG_INFO("Creating example game");
    }

    ExampleGame::~ExampleGame() {
        shutdown();
    }

    bool ExampleGame::initialize(Engine* engine) {
        if (!engine) {
            SPARKY_LOG_ERROR("Engine is null");
            return false;
        }

        this->engine = engine;
        this->renderSystem = &engine->getRenderSystem();
        
        SPARKY_LOG_DEBUG("Engine and renderSystem pointers set");
        
        try {
            createPlayer();
            createLevel();
            createEnemy();
            createLights();
            createQuests();
            setupAudio();
            setupGUI();
            
            // Set up initial camera position to see the level
            if (this->engine) {
                Camera& camera = this->engine->getCamera();
                // Position camera to see the entire level
                camera.setPosition(glm::vec3(0.0f, 5.0f, 10.0f)); // Higher and further back
                // Look at the center of the level
                camera.setFront(glm::normalize(glm::vec3(0.0f, -0.3f, -1.0f))); // Look slightly downward
                SPARKY_LOG_DEBUG("Camera initialized at position (0, 5, 10) looking toward level");
            }
            
            initialized = true;
            SPARKY_LOG_INFO("Example game initialized successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to initialize example game: " + std::string(e.what()));
            return false;
        }
    }

    void ExampleGame::update(float deltaTime) {
        if (!initialized || paused) return;

        handleInput(deltaTime);
        updateAI(deltaTime);
        checkCollisions();
        updateUI();

        // Update game systems
        if (particleSystem) {
            particleSystem->update(deltaTime);
        }

        SPARKY_LOG_DEBUG("Game updated with delta time: " + std::to_string(deltaTime));
    }

    void ExampleGame::render() {
        if (!initialized || paused) return;

        // The rendering is now handled by the RenderSystem automatically
        // We don't need to manually render each object anymore
        // The RenderSystem will iterate through all registered objects and render them
        
        // Actually call the RenderSystem to render
        if (renderSystem) {
            // Debug: Log render system call
            static int frameCount = 0;
            frameCount++;
            if (frameCount % 60 == 0) {
                SPARKY_LOG_DEBUG("Calling RenderSystem::render() with " + std::to_string(renderSystem->getGameObjects().size()) + " objects");
            }
            renderSystem->render();
        }

        // Render game systems
        if (particleSystem) {
            particleSystem->render();
        }

        // Debug: Log that game render completed
        static int frameCount = 0;
        frameCount++;
        if (frameCount % 60 == 0) {
            SPARKY_LOG_DEBUG("Game render completed");
        }
    }

    void ExampleGame::shutdown() {
        if (!initialized) return;

        // Clean up game objects
        platforms.clear();
        player.reset();
        enemy.reset();
        
        // Clean up systems
        particleSystem.reset();
        playerInventory.reset();
        mainQuest.reset();
        enemyAI.reset();
        
        // Clean up lights
        directionalLight.reset();
        pointLight.reset();
        
        initialized = false;
        SPARKY_LOG_INFO("Example game shut down");
    }

    void ExampleGame::startGame() {
        if (!initialized) return;
        
        paused = false;
        score = 0;
        health = 100;
        
        // Reset player position
        if (player) {
            player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        }
        
        // Start particle system
        if (particleSystem) {
            particleSystem->start();
        }
        
        // Start quest
        if (mainQuest) {
            mainQuest->start();
        }
        
        SPARKY_LOG_INFO("Game started");
    }

    void ExampleGame::pauseGame() {
        paused = true;
        SPARKY_LOG_INFO("Game paused");
    }

    void ExampleGame::resumeGame() {
        paused = false;
        SPARKY_LOG_INFO("Game resumed");
    }

    void ExampleGame::endGame() {
        // Stop particle system
        if (particleSystem) {
            particleSystem->stop();
        }
        
        SPARKY_LOG_INFO("Game ended with score: " + std::to_string(score));
    }

    void ExampleGame::createLevel() {
        SPARKY_LOG_DEBUG("Creating game level");
        
        // Clear existing meshes
        levelMeshes.clear();
        
        // Create a floor platform with a mesh
        auto floor = std::make_unique<Platform>("Floor");
        floor->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
        floor->setSize(glm::vec3(20.0f, 1.0f, 20.0f));
        
        // Add a render component with a plane mesh
        RenderComponent* renderComponent = floor->addComponent<RenderComponent>();
        auto planeMesh = Mesh::createPlane(20.0f, 20.0f);
        levelMeshes.push_back(std::move(planeMesh));
        renderComponent->setMesh(levelMeshes.back().get());
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(floor.get());
            SPARKY_LOG_DEBUG("Registered floor with render system");
        }
        
        platforms.push_back(std::move(floor));
        
        // Create some platforms with cube meshes
        auto platform1 = std::make_unique<Platform>("Platform1");
        platform1->setPosition(glm::vec3(5.0f, 1.0f, 0.0f)); // Raise platforms higher
        platform1->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
        
        // Add a render component with a cube mesh
        RenderComponent* cubeRender1 = platform1->addComponent<RenderComponent>();
        auto cubeMesh1 = Mesh::createCube(1.0f);
        levelMeshes.push_back(std::move(cubeMesh1));
        cubeRender1->setMesh(levelMeshes.back().get());
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(platform1.get());
            SPARKY_LOG_DEBUG("Registered platform1 with render system");
        }
        
        platforms.push_back(std::move(platform1));
        
        auto platform2 = std::make_unique<Platform>("Platform2");
        platform2->setPosition(glm::vec3(-5.0f, 2.0f, 0.0f)); // Raise platforms higher
        platform2->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
        
        // Add a render component with a cube mesh
        RenderComponent* cubeRender2 = platform2->addComponent<RenderComponent>();
        auto cubeMesh2 = Mesh::createCube(1.0f);
        levelMeshes.push_back(std::move(cubeMesh2));
        cubeRender2->setMesh(levelMeshes.back().get());
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(platform2.get());
        }
        
        platforms.push_back(std::move(platform2));
        
        // Create a ramp with a mesh
        auto ramp = std::make_unique<Platform>("Ramp");
        ramp->setPosition(glm::vec3(0.0f, 1.0f, 5.0f));
        ramp->setSize(glm::vec3(8.0f, 1.0f, 3.0f));
        
        // Add a render component with a plane mesh
        RenderComponent* rampRender = ramp->addComponent<RenderComponent>();
        auto rampMesh = Mesh::createPlane(8.0f, 3.0f);
        levelMeshes.push_back(std::move(rampMesh));
        rampRender->setMesh(levelMeshes.back().get());
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(ramp.get());
        }
        
        platforms.push_back(std::move(ramp));
        
        // Create stairs with individual steps
        for (int i = 0; i < 5; i++) {
            auto step = std::make_unique<Platform>("Step" + std::to_string(i));
            step->setPosition(glm::vec3(8.0f, 0.5f + i * 1.0f, -5.0f + i * 1.0f));
            step->setSize(glm::vec3(2.0f, 1.0f, 1.0f));
            
            // Add a render component with a cube mesh
            RenderComponent* stepRender = step->addComponent<RenderComponent>();
            auto stepMesh = Mesh::createCube(1.0f);
            levelMeshes.push_back(std::move(stepMesh));
            stepRender->setMesh(levelMeshes.back().get());
            
            // Register with render system
            if (renderSystem) {
                renderSystem->registerGameObject(step.get());
            }
            
            platforms.push_back(std::move(step));
        }
        
        SPARKY_LOG_DEBUG("Level created with " + std::to_string(platforms.size()) + " platforms");
    }

    void ExampleGame::createPlayer() {
        SPARKY_LOG_DEBUG("Creating player");
        
        player = std::make_unique<Player>();
        player->setName("Player");
        player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        player->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
        
        // Add a render component with a cube mesh for the player
        RenderComponent* renderComponent = player->addComponent<RenderComponent>();
        playerMesh = Mesh::createCube(1.0f);
        renderComponent->setMesh(playerMesh.get());
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(player.get());
            SPARKY_LOG_DEBUG("Registered player with render system");
        }
        
        // Set up player camera
        if (engine) {
            player->setCamera(&engine->getCamera());
            SPARKY_LOG_DEBUG("Player camera set up");
        }
        
        SPARKY_LOG_DEBUG("Player created");
    }

    void ExampleGame::createEnemy() {
        SPARKY_LOG_DEBUG("Creating enemy");
        
        enemy = std::make_unique<GameObject>();
        enemy->setName("Enemy");
        enemy->setPosition(glm::vec3(3.0f, 0.0f, 3.0f)); // Position enemy where it can be seen
        enemy->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
        
        // Add a render component with a cube mesh for the enemy
        RenderComponent* renderComponent = enemy->addComponent<RenderComponent>();
        enemyMesh = Mesh::createCube(1.0f);
        renderComponent->setMesh(enemyMesh.get());
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(enemy.get());
        }
        
        // Create enemy AI behavior
        enemyAI = ExampleAIBehavior::createPatrolBehavior(enemy.get());
        
        SPARKY_LOG_DEBUG("Enemy created");
    }

    void ExampleGame::createLights() {
        SPARKY_LOG_DEBUG("Creating lights");
        
        // Create directional light (sun)
        directionalLight = std::make_unique<Light>("Sun");
        directionalLight->setDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
        directionalLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f)); // Color
        directionalLight->setAmbient(glm::vec3(0.1f, 0.1f, 0.1f)); // Intensity-like effect
        
        // Create point light
        pointLight = std::make_unique<Light>("PointLight");
        pointLight->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
        pointLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f)); // Color
        pointLight->setAmbient(glm::vec3(0.05f, 0.05f, 0.05f)); // Intensity-like effect
        pointLight->setConstant(1.0f);
        pointLight->setLinear(0.09f);
        pointLight->setQuadratic(0.032f);
        
        SPARKY_LOG_DEBUG("Lights created");
    }

    void ExampleGame::createQuests() {
        SPARKY_LOG_DEBUG("Creating quests");
        
        playerInventory = std::make_unique<Inventory>(20);
        
        mainQuest = std::make_unique<Quest>("Main Quest", "Defeat the enemy and collect items");
        mainQuest->addObjective(std::make_unique<Sparky::QuestObjective>("Find the key"));
        mainQuest->addObjective(std::make_unique<Sparky::QuestObjective>("Defeat the enemy"));
        mainQuest->addObjective(std::make_unique<Sparky::QuestObjective>("Return to base"));
        
        SPARKY_LOG_DEBUG("Quests created");
    }

    void ExampleGame::setupAudio() {
        SPARKY_LOG_DEBUG("Setting up audio");
        // Audio setup would go here if we had a working audio system
        SPARKY_LOG_DEBUG("Audio setup completed");
    }

    void ExampleGame::setupGUI() {
        SPARKY_LOG_DEBUG("Setting up GUI");
        // GUI setup would go here
        SPARKY_LOG_DEBUG("GUI setup completed");
    }

    void ExampleGame::handleInput(float deltaTime) {
        if (!engine || !player) return;
        
        InputManager& inputManager = engine->getInputManager();
        
        // Player movement
        glm::vec3 movement(0.0f);
        if (inputManager.isKeyPressed(87)) { // W key
            movement.z -= 1.0f;
        }
        if (inputManager.isKeyPressed(83)) { // S key
            movement.z += 1.0f;
        }
        if (inputManager.isKeyPressed(65)) { // A key
            movement.x -= 1.0f;
        }
        if (inputManager.isKeyPressed(68)) { // D key
            movement.x += 1.0f;
        }
        
        if (glm::length(movement) > 0.0f) {
            movement = glm::normalize(movement);
            glm::vec3 position = player->getPosition();
            position += movement * 5.0f * deltaTime;
            player->setPosition(position);
            
            // Update camera position
            if (player->getCamera()) {
                player->getCamera()->setPosition(position + glm::vec3(0.0f, 1.5f, 0.0f));
            }
        }
    }

    void ExampleGame::updateAI(float deltaTime) {
        if (enemyAI) {
            enemyAI->update(deltaTime);
        }
    }

    void ExampleGame::checkCollisions() {
        // Simple collision detection would go here
    }

    void ExampleGame::updateUI() {
        // UI updates would go here
    }
}