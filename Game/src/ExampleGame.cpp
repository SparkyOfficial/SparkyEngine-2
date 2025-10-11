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
#include "../../Engine/include/OBJLoader.h"
#include "../../Engine/include/RenderSystem.h"
#include <glm/glm.hpp>
#include "../../Engine/include/SaveGameManager.h"

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
            
            // Load skybox
            loadSkybox();
            
            // Set up initial camera position to see the level
            if (this->engine) {
                Camera& camera = this->engine->getCamera();
                // Position camera to see the entire level
                camera.setPosition(glm::vec3(0.0f, 5.0f, 10.0f)); // Higher and further back
                // Look at the center of the level
                camera.setFront(glm::normalize(glm::vec3(0.0f, -0.3f, -1.0f))); // Look slightly downward
                SPARKY_LOG_DEBUG("Camera initialized at position (0, 5, 10) looking toward level");
            }
            
            // Initialize meshes for all game objects
            SPARKY_LOG_INFO("About to initialize meshes for " + std::to_string(allGameObjects.size()) + " game objects");
            initializeMeshes();
            SPARKY_LOG_INFO("Mesh initialization completed");
            
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
        
        handleSaveLoadInput();
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
            particleSystem->setActive(true);
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
            particleSystem->setActive(false);
        }
        
        SPARKY_LOG_INFO("Game ended with score: " + std::to_string(score));
    }

    void ExampleGame::createLevel() {
        SPARKY_LOG_DEBUG("Creating game level");
        
        // Clear existing meshes
        levelMeshes.clear();
        
        // Create a larger floor platform with a mesh
        auto floor = std::make_unique<Platform>("Floor");
        floor->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
        floor->setSize(glm::vec3(30.0f, 1.0f, 30.0f)); // Make it larger
    
        // Add a render component with a plane mesh
        RenderComponent* renderComponent = floor->addComponent<RenderComponent>();
        auto planeMesh = Mesh::createPlane(30.0f, 30.0f);
        levelMeshes.push_back(std::move(planeMesh));
        renderComponent->setMesh(std::move(levelMeshes.back()));
    
        // Add simple material for the floor (no texture)
        auto floorMaterial = std::make_unique<Material>("FloorMaterial");
        floorMaterial->setDiffuse(glm::vec3(0.5f, 0.5f, 0.5f)); // Gray color
        floorMaterial->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
        floorMaterial->setSpecular(glm::vec3(0.3f, 0.3f, 0.3f));
        floorMaterial->setShininess(16.0f);
    
        renderComponent->setMaterial(std::move(floorMaterial));
    
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(floor.get());
            SPARKY_LOG_DEBUG("Registered floor with render system");
        }
    
        allGameObjects.push_back(floor.get());
        platforms.push_back(std::move(floor));
    
        // Create some platforms with cube meshes
        auto platform1 = std::make_unique<Platform>("Platform1");
        platform1->setPosition(glm::vec3(5.0f, 1.0f, 0.0f)); // Raise platforms higher
        platform1->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
    
        // Add a render component with a cube mesh
        RenderComponent* cubeRender1 = platform1->addComponent<RenderComponent>();
        auto cubeMesh1 = Mesh::createCube(1.0f);
        levelMeshes.push_back(std::move(cubeMesh1));
        cubeRender1->setMesh(std::move(levelMeshes.back()));
    
        // Add simple material for platform1 (no texture)
        auto platform1Material = std::make_unique<Material>("Platform1Material");
        platform1Material->setDiffuse(glm::vec3(0.2f, 0.4f, 0.8f)); // Blue color
        platform1Material->setAmbient(glm::vec3(0.1f, 0.2f, 0.4f));
        platform1Material->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        platform1Material->setShininess(32.0f);
    
        cubeRender1->setMaterial(std::move(platform1Material));
    
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(platform1.get());
            SPARKY_LOG_DEBUG("Registered platform1 with render system");
        }
    
        allGameObjects.push_back(platform1.get());
        platforms.push_back(std::move(platform1));
    
        auto platform2 = std::make_unique<Platform>("Platform2");
        platform2->setPosition(glm::vec3(-5.0f, 2.0f, 0.0f)); // Raise platforms higher
        platform2->setSize(glm::vec3(3.0f, 1.0f, 3.0f));
    
        // Add a render component with a cube mesh
        RenderComponent* cubeRender2 = platform2->addComponent<RenderComponent>();
        auto cubeMesh2 = Mesh::createCube(1.0f);
        levelMeshes.push_back(std::move(cubeMesh2));
        cubeRender2->setMesh(std::move(levelMeshes.back()));
    
        // Add simple material for platform2 (no texture)
        auto platform2Material = std::make_unique<Material>("Platform2Material");
        platform2Material->setDiffuse(glm::vec3(0.8f, 0.2f, 0.2f)); // Red color
        platform2Material->setAmbient(glm::vec3(0.4f, 0.1f, 0.1f));
        platform2Material->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        platform2Material->setShininess(32.0f);
    
        cubeRender2->setMaterial(std::move(platform2Material));
    
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(platform2.get());
        }
    
        allGameObjects.push_back(platform2.get());
        platforms.push_back(std::move(platform2));
    
        // Create a ramp with a mesh
        auto ramp = std::make_unique<Platform>("Ramp");
        ramp->setPosition(glm::vec3(0.0f, 1.0f, 5.0f));
        ramp->setSize(glm::vec3(8.0f, 1.0f, 3.0f));
    
        // Add a render component with a plane mesh
        RenderComponent* rampRender = ramp->addComponent<RenderComponent>();
        auto rampMesh = Mesh::createPlane(8.0f, 3.0f);
        levelMeshes.push_back(std::move(rampMesh));
        rampRender->setMesh(std::move(levelMeshes.back()));
    
        // Add simple material for ramp (no texture)
        auto rampMaterial = std::make_unique<Material>("RampMaterial");
        rampMaterial->setDiffuse(glm::vec3(0.2f, 0.8f, 0.2f)); // Green color
        rampMaterial->setAmbient(glm::vec3(0.1f, 0.4f, 0.1f));
        rampMaterial->setSpecular(glm::vec3(0.3f, 0.3f, 0.3f));
        rampMaterial->setShininess(16.0f);
    
        rampRender->setMaterial(std::move(rampMaterial));
    
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(ramp.get());
        }
    
        allGameObjects.push_back(ramp.get());
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
            stepRender->setMesh(std::move(levelMeshes.back()));
            
            // Add simple material for steps (no texture)
            auto stepMaterial = std::make_unique<Material>("StepMaterial" + std::to_string(i));
            stepMaterial->setDiffuse(glm::vec3(0.8f, 0.8f, 0.2f)); // Yellow color
            stepMaterial->setAmbient(glm::vec3(0.4f, 0.4f, 0.1f));
            stepMaterial->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
            stepMaterial->setShininess(16.0f);
            
            stepRender->setMaterial(std::move(stepMaterial));
            
            // Register with render system
            if (renderSystem) {
                renderSystem->registerGameObject(step.get());
            }
            
            allGameObjects.push_back(step.get());
            platforms.push_back(std::move(step));
        }
    
        // Create some walls to make it more like a Half-Life level
        // Wall 1 - Back wall
        auto wall1 = std::make_unique<Platform>("BackWall");
        wall1->setPosition(glm::vec3(0.0f, 2.0f, -15.0f));
        wall1->setSize(glm::vec3(30.0f, 6.0f, 1.0f));
    
        RenderComponent* wall1Render = wall1->addComponent<RenderComponent>();
        auto wall1Mesh = Mesh::createCube(1.0f);
        levelMeshes.push_back(std::move(wall1Mesh));
        wall1Render->setMesh(std::move(levelMeshes.back()));
    
        auto wall1Material = std::make_unique<Material>("Wall1Material");
        wall1Material->setDiffuse(glm::vec3(0.7f, 0.7f, 0.7f)); // Light gray
        wall1Material->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
        wall1Material->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
        wall1Material->setShininess(8.0f);
    
        wall1Render->setMaterial(std::move(wall1Material));
    
        if (renderSystem) {
            renderSystem->registerGameObject(wall1.get());
        }
    
        allGameObjects.push_back(wall1.get());
        platforms.push_back(std::move(wall1));
    
        // Wall 2 - Left wall
        auto wall2 = std::make_unique<Platform>("LeftWall");
        wall2->setPosition(glm::vec3(-15.0f, 2.0f, 0.0f));
        wall2->setSize(glm::vec3(1.0f, 6.0f, 30.0f));
    
        RenderComponent* wall2Render = wall2->addComponent<RenderComponent>();
        auto wall2Mesh = Mesh::createCube(1.0f);
        levelMeshes.push_back(std::move(wall2Mesh));
        wall2Render->setMesh(std::move(levelMeshes.back()));
    
        auto wall2Material = std::make_unique<Material>("Wall2Material");
        wall2Material->setDiffuse(glm::vec3(0.7f, 0.7f, 0.7f)); // Light gray
        wall2Material->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
        wall2Material->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
        wall2Material->setShininess(8.0f);
    
        wall2Render->setMaterial(std::move(wall2Material));
    
        if (renderSystem) {
            renderSystem->registerGameObject(wall2.get());
        }
    
        allGameObjects.push_back(wall2.get());
        platforms.push_back(std::move(wall2));
    
        // Wall 3 - Right wall
        auto wall3 = std::make_unique<Platform>("RightWall");
        wall3->setPosition(glm::vec3(15.0f, 2.0f, 0.0f));
        wall3->setSize(glm::vec3(1.0f, 6.0f, 30.0f));
    
        RenderComponent* wall3Render = wall3->addComponent<RenderComponent>();
        auto wall3Mesh = Mesh::createCube(1.0f);
        levelMeshes.push_back(std::move(wall3Mesh));
        wall3Render->setMesh(std::move(levelMeshes.back()));
    
        auto wall3Material = std::make_unique<Material>("Wall3Material");
        wall3Material->setDiffuse(glm::vec3(0.7f, 0.7f, 0.7f)); // Light gray
        wall3Material->setAmbient(glm::vec3(0.3f, 0.3f, 0.3f));
        wall3Material->setSpecular(glm::vec3(0.2f, 0.2f, 0.2f));
        wall3Material->setShininess(8.0f);
    
        wall3Render->setMaterial(std::move(wall3Material));
    
        if (renderSystem) {
            renderSystem->registerGameObject(wall3.get());
        }
    
        allGameObjects.push_back(wall3.get());
        platforms.push_back(std::move(wall3));
    
        SPARKY_LOG_DEBUG("Level created with " + std::to_string(platforms.size()) + " platforms");
    }

    void ExampleGame::createPlayer() {
        SPARKY_LOG_DEBUG("Creating player");
        
        player = std::make_unique<Player>();
        player->setName("Player");
        player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        player->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
        
        // Add a render component with a loaded 3D model for the player
        RenderComponent* renderComponent = player->addComponent<RenderComponent>();
        
        // Try to load a 3D model, fall back to cube if loading fails
        playerMesh = Sparky::OBJLoader::loadFromFile("Engine/assets/cube.obj");
        if (!playerMesh) {
            SPARKY_LOG_WARNING("Failed to load player model, using cube mesh");
            playerMesh = Mesh::createCube(1.0f);
        }
        renderComponent->setMesh(std::move(playerMesh));
        
        // Add material for player (cyan)
        auto playerMaterial = std::make_unique<Material>("PlayerMaterial");
        playerMaterial->setDiffuse(glm::vec3(0.0f, 1.0f, 1.0f)); // Cyan color
        playerMaterial->setAmbient(glm::vec3(0.0f, 0.5f, 0.5f));
        playerMaterial->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        playerMaterial->setShininess(32.0f);
        renderComponent->setMaterial(std::move(playerMaterial));
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(player.get());
            SPARKY_LOG_DEBUG("Registered player with render system");
        }
        
        allGameObjects.push_back(player.get());
        
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
        
        // Add a render component with a loaded 3D model for the enemy
        RenderComponent* renderComponent = enemy->addComponent<RenderComponent>();
        
        // Try to load a 3D model, fall back to cube if loading fails
        enemyMesh = Sparky::OBJLoader::loadFromFile("Engine/assets/cube.obj");
        if (!enemyMesh) {
            SPARKY_LOG_WARNING("Failed to load enemy model, using cube mesh");
            enemyMesh = Mesh::createCube(1.0f);
        }
        renderComponent->setMesh(std::move(enemyMesh));
        
        // Add material for enemy (magenta)
        auto enemyMaterial = std::make_unique<Material>("EnemyMaterial");
        enemyMaterial->setDiffuse(glm::vec3(1.0f, 0.0f, 1.0f)); // Magenta color
        enemyMaterial->setAmbient(glm::vec3(0.5f, 0.0f, 0.5f));
        enemyMaterial->setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
        enemyMaterial->setShininess(32.0f);
        renderComponent->setMaterial(std::move(enemyMaterial));
        
        // Register with render system
        if (renderSystem) {
            renderSystem->registerGameObject(enemy.get());
        }
        
        allGameObjects.push_back(enemy.get());
        
        // Create enemy AI behavior
        enemyAI = Sparky::createPatrolBehaviorTree(enemy.get(), player.get());
        
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

    void ExampleGame::loadSkybox() {
        SPARKY_LOG_DEBUG("Loading skybox");
        
        if (!engine) {
            SPARKY_LOG_ERROR("Cannot load skybox without engine");
            return;
        }
        
        // For now, we'll just log that skybox loading would be implemented
        // In a full implementation, we would:
        // 1. Load 6 textures for the skybox faces
        // 2. Create a cubemap texture
        // 3. Set up the skybox renderer
        SPARKY_LOG_DEBUG("Skybox loading would be implemented here");
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

    // New method for initializing meshes
    void ExampleGame::initializeMeshes() {
        SPARKY_LOG_INFO("Starting mesh initialization");
        if (!engine) {
            SPARKY_LOG_ERROR("Engine is null in initializeMeshes");
            return;
        }
        MeshRenderer& meshRenderer = engine->getRenderer().getMeshRenderer();

        SPARKY_LOG_INFO("Initializing meshes for " + std::to_string(allGameObjects.size()) + " game objects...");
        
        for (size_t i = 0; i < allGameObjects.size(); i++) {
            auto& obj = allGameObjects[i];
            if (!obj) {
                SPARKY_LOG_DEBUG("Object at index " + std::to_string(i) + " is null");
                continue;
            }
            RenderComponent* renderComp = obj->getComponent<RenderComponent>();
            if (renderComp && renderComp->getMesh()) {
                SPARKY_LOG_DEBUG("Creating buffers for mesh of object: " + obj->getName());
                meshRenderer.createVertexBuffer(*renderComp->getMesh());
                meshRenderer.createIndexBuffer(*renderComp->getMesh());
            }
        }
        SPARKY_LOG_INFO("All meshes initialized.");
    }

    void ExampleGame::handleSaveLoadInput() {
        // Handle save/load input
        InputManager& inputManager = InputManager::getInstance();
        
        // Save game (F5)
        if (inputManager.isKeyJustPressed(GLFW_KEY_F5)) {
            SaveGameManager& saveManager = SaveGameManager::getInstance();
            // In a full implementation, we would pass the actual player, level, and inventory
            bool saved = saveManager.saveGame("quicksave", nullptr, nullptr, nullptr);
            if (saved) {
                Logger::getInstance().info("Game saved successfully!");
            } else {
                Logger::getInstance().error("Failed to save game!");
            }
        }
        
        // Load game (F9)
        if (inputManager.isKeyJustPressed(GLFW_KEY_F9)) {
            SaveGameManager& saveManager = SaveGameManager::getInstance();
            // In a full implementation, we would pass the actual player, level, and inventory
            bool loaded = saveManager.loadGame("quicksave", nullptr, nullptr, nullptr);
            if (loaded) {
                Logger::getInstance().info("Game loaded successfully!");
            } else {
                Logger::getInstance().error("Failed to load game!");
            }
        }
        
        // Auto-save every 30 seconds if enabled
        static float autoSaveTimer = 0.0f;
        autoSaveTimer += deltaTime;
        if (autoSaveTimer >= 30.0f) {
            SaveGameManager& saveManager = SaveGameManager::getInstance();
            if (saveManager.getAutoSave()) {
                saveManager.saveGame("autosave", nullptr, nullptr, nullptr);
                Logger::getInstance().info("Auto-saved game");
            }
            autoSaveTimer = 0.0f;
        }
    }
}