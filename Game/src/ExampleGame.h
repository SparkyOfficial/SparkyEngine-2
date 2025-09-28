#pragma once

#include <memory>
#include <vector>

namespace Sparky {
    class Engine;
    class GameObject;
    class Player;
    class Platform;
    class Light;
    class ParticleSystem;
    class Quest;
    class Inventory;
    class GUIManager;
    class AudioEngine;
    class BehaviorTree;

    class ExampleGame {
    public:
        ExampleGame();
        ~ExampleGame();

        bool initialize(Engine* engine);
        void update(float deltaTime);
        void render();
        void shutdown();

        // Game states
        void startGame();
        void pauseGame();
        void resumeGame();
        void endGame();

    private:
        Engine* engine;
        bool initialized;
        bool paused;

        // Game objects
        std::unique_ptr<Player> player;
        std::vector<std::unique_ptr<Platform>> platforms;
        std::unique_ptr<GameObject> enemy;
        
        // Systems
        std::unique_ptr<ParticleSystem> particleSystem;
        std::unique_ptr<Inventory> playerInventory;
        std::unique_ptr<Quest> mainQuest;
        std::unique_ptr<BehaviorTree> enemyAI;
        
        // Lighting
        std::unique_ptr<Light> directionalLight;
        std::unique_ptr<Light> pointLight;
        
        // Audio
        AudioEngine* audioEngine;
        
        // Game state
        int score;
        int health;
        
        // Private methods
        void createLevel();
        void createPlayer();
        void createEnemy();
        void createLights();
        void createQuests();
        void setupAudio();
        void setupGUI();
        void handleInput(float deltaTime);
        void updateAI(float deltaTime);
        void checkCollisions();
        void updateUI();
    };
}