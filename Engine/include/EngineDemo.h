#pragma once

#include <memory>

namespace Sparky {
    // Forward declarations
    class VulkanRenderer;
    class WindowManager;
    class InputManager;
    class Camera;
    class GameObject;
    class PhysicsWorld;
    class AudioEngine;
    class LightManager;
    class GUIManager;
    class QuestManager;

    class EngineDemo {
    public:
        EngineDemo();
        ~EngineDemo();

        bool initialize();
        void run();
        void shutdown();

    private:
        // Core engine systems
        std::unique_ptr<VulkanRenderer> renderer;
        std::unique_ptr<WindowManager> windowManager;
        std::unique_ptr<InputManager> inputManager;
        std::unique_ptr<Camera> camera;
        
        // Game systems
        std::unique_ptr<PhysicsWorld> physicsWorld;
        std::unique_ptr<AudioEngine> audioEngine;
        std::unique_ptr<LightManager> lightManager;
        std::unique_ptr<GUIManager> guiManager;
        std::unique_ptr<QuestManager> questManager;
        
        // Game objects
        std::unique_ptr<GameObject> player;
        std::unique_ptr<GameObject> enemy;
        
        // State
        bool running;
        bool initialized;
        
        // Private methods
        void setupScene();
        void setupLighting();
        void setupAudio();
        void setupGUI();
        void setupQuests();
        void handleInput();
        void updateSystems(float deltaTime);
        void renderSystems();
    };
}