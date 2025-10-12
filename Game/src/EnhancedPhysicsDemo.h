#pragma once

#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/RigidBodyComponent.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/Material.h"
#include "../../Engine/include/Camera.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/PhysicsWorld.h"
#include <vector>
#include <memory>

namespace Sparky {
    class EnhancedPhysicsDemo {
    public:
        EnhancedPhysicsDemo();
        ~EnhancedPhysicsDemo();

        bool initialize(Engine* engine);
        void update(float deltaTime);
        void render();
        void cleanup();

        void createScene();

    private:
        Engine* engine;
        std::vector<std::unique_ptr<GameObject>> objects;
        std::vector<std::unique_ptr<Mesh>> meshes;
        std::unique_ptr<GameObject> player;
        std::unique_ptr<GameObject> ground;
        
        // Materials
        std::unique_ptr<Material> groundMaterial;
        std::unique_ptr<Material> objectMaterial;
        std::unique_ptr<Material> playerMaterial;
        
        // Controls
        bool isRunning;
        
        void setupPlayer();
        void setupGround();
        void createPhysicsObjects();
        void handleInput(float deltaTime);
    };
}