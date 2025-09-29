#pragma once

#include <memory>
#include <vector>
#include "../../Engine/include/SparkyEngine.h"

namespace Sparky {
    class GameObject;
    class Mesh;

    class PhysicsDemo {
    public:
        PhysicsDemo();
        ~PhysicsDemo();

        bool initialize(Engine* engine);
        void update(float deltaTime);
        void render();
        void cleanup();

    private:
        Engine* engine;
        
        // Game objects
        std::unique_ptr<GameObject> player;
        std::unique_ptr<GameObject> ground;
        std::vector<std::unique_ptr<GameObject>> objects;
        
        // Meshes
        std::vector<std::unique_ptr<Mesh>> meshes;
        
        void createScene();
    };
}