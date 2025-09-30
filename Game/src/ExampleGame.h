#pragma once

#include <memory>
#include <vector>
#include "../../Engine/include/BehaviorTree.h"
#include "../../Engine/include/GUIManager.h"
#include "../../Engine/include/Mesh.h"  // Add this include for Mesh class

namespace Sparky {
    class Engine;
    class RenderSystem;
    class GameObject;
    class Player;
    class Platform;
    class Light;
    class ParticleSystem;
    class Inventory;
    class Quest;
    class Mesh;
    class BehaviorTree;  // Forward declaration

    class ExampleGame {
    public:
        ExampleGame();
        ~ExampleGame();

        bool initialize(Engine* engine);
        void update(float deltaTime);
        void render();
        void shutdown();

        // Game state management
        void startGame();
        void pauseGame();
        void resumeGame();
        void endGame();

        // Getters
        bool isInitialized() const { return initialized; }
        bool isPaused() const { return paused; }
        int getScore() const { return score; }
        int getHealth() const { return health; }

    private:
        // Engine systems
        Engine* engine;
        RenderSystem* renderSystem;

        // Skybox
        void loadSkybox();

        // Game objects
        std::unique_ptr<Player> player;
        std::unique_ptr<GameObject> enemy;
        std::vector<std::unique_ptr<Platform>> platforms;
        std::unique_ptr<Mesh> playerMesh;
        std::unique_ptr<Mesh> enemyMesh;
        std::vector<std::unique_ptr<Mesh>> levelMeshes;

        // Game systems
        std::unique_ptr<ParticleSystem> particleSystem;
        std::unique_ptr<Inventory> playerInventory;
        std::unique_ptr<Quest> mainQuest;
        std::unique_ptr<Light> directionalLight;
        std::unique_ptr<Light> pointLight;
        GUIManager* guiManager;  // Use pointer to access singleton

        // AI
        std::unique_ptr<BehaviorTree> enemyAI;

        // Game state
        bool initialized;
        bool paused;
        int score;
        int health;

        // Game objects list for mesh initialization
        std::vector<GameObject*> allGameObjects; // Vector for storing all game objects

        // Initialization methods
        void createLevel();
        void createPlayer();
        void createEnemy();
        void createLights();
        void createQuests();
        void setupAudio();
        void setupGUI();
        void initializeMeshes(); // Method for initializing meshes

        // Game loop methods
        void handleInput(float deltaTime);
        void updateAI(float deltaTime);
        void checkCollisions();
        void updateUI();
    };
}