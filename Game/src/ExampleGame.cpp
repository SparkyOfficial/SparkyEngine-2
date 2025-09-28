#include "ExampleGame.h"
#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/Player.h"
#include "../../Engine/include/Platform.h"
#include "../../Engine/include/Light.h"
#include "../../Engine/include/ParticleSystem.h"
#include "../../Engine/include/Inventory.h"
#include "../../Engine/include/Quest.h"
#include "../../Engine/include/AudioEngine.h"
#include "../../Engine/include/GUIManager.h"
#include "../../Engine/include/BehaviorTreeExample.h"
#include "../../Engine/include/Logger.h"
#include <glm/glm.hpp>

namespace Sparky {

    ExampleGame::ExampleGame() : engine(nullptr), initialized(false), paused(false),
                               score(0), health(100), audioEngine(nullptr) {
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
        
        try {
            createPlayer();
            createLevel();
            createEnemy();
            createLights();
            createQuests();
            setupAudio();
            setupGUI();
            
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

        // Render game systems
        if (particleSystem) {
            particleSystem->render();
        }

        SPARKY_LOG_DEBUG("Game rendered");
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
        
        // Create a floor platform with a mesh
        auto floor = std::make_unique<Platform>("Floor");
        floor->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
        floor->setSize(glm::vec3(20.0f, 1.0f, 20.0f));
        
        // Add a render component with a plane mesh
        auto renderComponent = std::make_unique<RenderComponent>();
        auto planeMesh = Mesh::createPlane(20.0f, 20.0f);
        renderComponent->setMesh(std::move(planeMesh));
        floor->addComponent(std::move(renderComponent));
        
        platforms.push_back(std::move(floor));
        
        // Create some platforms with cube meshes
        auto platform1 = std::make_unique<Platform>("Platform1");
        platform1->setPosition(glm::vec3(5.0f, 2.0f, 0.0f));
        platform1->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
        
        // Add a render component with a cube mesh
        auto cubeRender1 = std::make_unique<RenderComponent>();
        auto cubeMesh1 = Mesh::createCube(1.0f);
        cubeRender1->setMesh(std::move(cubeMesh1));
        platform1->addComponent(std::move(cubeRender1));
        
        platforms.push_back(std::move(platform1));
        
        auto platform2 = std::make_unique<Platform>("Platform2");
        platform2->setPosition(glm::vec3(-5.0f, 4.0f, 0.0f));
        platform2->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
        
        // Add a render component with a cube mesh
        auto cubeRender2 = std::make_unique<RenderComponent>();
        auto cubeMesh2 = Mesh::createCube(1.0f);
        cubeRender2->setMesh(std::move(cubeMesh2));
        platform2->addComponent(std::move(cubeRender2));
        
        platforms.push_back(std::move(platform2));
        
        // Create a ramp with a mesh
        auto ramp = std::make_unique<Platform>("Ramp");
        ramp->setPosition(glm::vec3(0.0f, 1.0f, 5.0f));
        ramp->setSize(glm::vec3(8.0f, 1.0f, 3.0f));
        
        // Add a render component with a plane mesh
        auto rampRender = std::make_unique<RenderComponent>();
        auto rampMesh = Mesh::createPlane(8.0f, 3.0f);
        rampRender->setMesh(std::move(rampMesh));
        ramp->addComponent(std::move(rampRender));
        
        platforms.push_back(std::move(ramp));
        
        // Create stairs with individual steps
        for (int i = 0; i < 5; i++) {
            auto step = std::make_unique<Platform>("Step" + std::to_string(i));
            step->setPosition(glm::vec3(8.0f, 0.5f + i * 1.0f, -5.0f + i * 1.0f));
            step->setSize(glm::vec3(2.0f, 1.0f, 1.0f));
            
            // Add a render component with a cube mesh
            auto stepRender = std::make_unique<RenderComponent>();
            auto stepMesh = Mesh::createCube(1.0f);
            stepRender->setMesh(std::move(stepMesh));
            step->addComponent(std::move(stepRender));
            
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
        auto renderComponent = std::make_unique<RenderComponent>();
        auto playerMesh = Mesh::createCube(1.0f);
        renderComponent->setMesh(std::move(playerMesh));
        player->addComponent(std::move(renderComponent));
        
        // Set up player camera
        if (engine) {
            player->setCamera(&engine->getCamera());
        }
        
        SPARKY_LOG_DEBUG("Player created");
    }

    void ExampleGame::createEnemy() {
        SPARKY_LOG_DEBUG("Creating enemy");
        
        enemy = std::make_unique<GameObject>();
        enemy->setName("Enemy");
        enemy->setPosition(glm::vec3(10.0f, 0.0f, 0.0f));
        enemy->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
        
        // Add a render component with a cube mesh for the enemy
        auto renderComponent = std::make_unique<RenderComponent>();
        auto enemyMesh = Mesh::createCube(1.0f);
        renderComponent->setMesh(std::move(enemyMesh));
        enemy->addComponent(std::move(renderComponent));
        
        // Create enemy AI behavior
        enemyAI = ExampleAIBehavior::createPatrolBehavior(enemy.get());
        
        SPARKY_LOG_DEBUG("Enemy created");
    }

    void ExampleGame::createLights() {
        SPARKY_LOG_DEBUG("Creating lights");
        
        // Create directional light (sun)
        directionalLight = std::make_unique<Light>("Sun");
        directionalLight->setDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
        directionalLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        directionalLight->setIntensity(1.0f);
        
        // Create point light
        pointLight = std::make_unique<Light>("PointLight");
        pointLight->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
        pointLight->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLight->setIntensity(0.5f);
        pointLight->setConstant(1.0f);
        pointLight->setLinear(0.09f);
        pointLight->setQuadratic(0.032f);
        
        SPARKY_LOG_DEBUG("Lights created");
    }

    void ExampleGame::createQuests() {
        SPARKY_LOG_DEBUG("Creating quests");
        
        playerInventory = std::make_unique<Inventory>(20);
        
        mainQuest = std::make_unique<Quest>("MainQuest", "Defeat the enemy and collect items");
        
        auto objective1 = std::make_unique<QuestObjective>("Defeat the enemy", 1);
        mainQuest->addObjective(std::move(objective1));
        
        auto objective2 = std::make_unique<QuestObjective>("Collect 5 coins", 5);
        mainQuest->addObjective(std::move(objective2));
        
        mainQuest->setRewardExperience(100);
        mainQuest->setRewardCurrency(50);
        
        SPARKY_LOG_DEBUG("Quests created");
    }

    void ExampleGame::setupAudio() {
        SPARKY_LOG_DEBUG("Setting up audio");
        
        audioEngine = &AudioEngine::getInstance();
        if (audioEngine->initialize()) {
            SPARKY_LOG_DEBUG("Audio engine initialized");
        } else {
            SPARKY_LOG_WARNING("Failed to initialize audio engine");
        }
    }

    void ExampleGame::setupGUI() {
        SPARKY_LOG_DEBUG("Setting up GUI");
        
        auto& guiManager = GUIManager::getInstance();
        
        // Create a health bar
        auto healthBar = guiManager.createButton("HealthBar");
        if (healthBar) {
            healthBar->setPosition(glm::vec2(10.0f, 10.0f));
            healthBar->setSize(glm::vec2(200.0f, 30.0f));
            healthBar->setText("Health: " + std::to_string(health));
        }
        
        // Create a score display
        auto scoreDisplay = guiManager.createButton("ScoreDisplay");
        if (scoreDisplay) {
            scoreDisplay->setPosition(glm::vec2(10.0f, 50.0f));
            scoreDisplay->setSize(glm::vec2(200.0f, 30.0f));
            scoreDisplay->setText("Score: " + std::to_string(score));
        }
        
        SPARKY_LOG_DEBUG("GUI setup complete");
    }

    void ExampleGame::handleInput(float deltaTime) {
        if (!engine || !player) return;
        
        // Get input manager
        InputManager& inputManager = engine->getInputManager();
        
        // Handle player input
        if (inputManager.isKeyPressed(87)) { // W key
            player->moveForward(deltaTime);
        }
        if (inputManager.isKeyPressed(83)) { // S key
            player->moveBackward(deltaTime);
        }
        if (inputManager.isKeyPressed(65)) { // A key
            player->moveLeft(deltaTime);
        }
        if (inputManager.isKeyPressed(68)) { // D key
            player->moveRight(deltaTime);
        }
        if (inputManager.isKeyJustPressed(32)) { // Space key
            player->jump();
        }
    }

    void ExampleGame::updateAI(float deltaTime) {
        if (enemyAI) {
            enemyAI->update(deltaTime);
        }
    }

    void ExampleGame::checkCollisions() {
        // Simple ground collision detection
        if (player) {
            glm::vec3 playerPos = player->getPosition();
            if (playerPos.y < 0.0f) {
                playerPos.y = 0.0f;
                player->setPosition(playerPos);
                // In a real implementation, we would get the player's physics component
                // and set it to grounded
            }
        }
    }

    void ExampleGame::updateUI() {
        auto& guiManager = GUIManager::getInstance();
        
        // Update health bar
        auto healthBar = static_cast<Button*>(guiManager.getElement("HealthBar"));
        if (healthBar) {
            healthBar->setText("Health: " + std::to_string(health));
        }
        
        // Update score display
        auto scoreDisplay = static_cast<Button*>(guiManager.getElement("ScoreDisplay"));
        if (scoreDisplay) {
            scoreDisplay->setText("Score: " + std::to_string(score));
        }
        
        // Update GUI
        guiManager.update(0.016f); // Assuming 60 FPS
    }
}