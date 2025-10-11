#pragma once

#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/Material.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/Camera.h"
#include "../../Engine/include/Light.h"
#include "../../Engine/include/InputManager.h"

namespace Sparky {
    class PBRExample {
    public:
        PBRExample();
        ~PBRExample();

        bool initialize();
        void run();
        void cleanup();

    private:
        SparkyEngine engine;
        std::unique_ptr<GameObject> sphereObject;
        std::unique_ptr<GameObject> cubeObject;
        std::unique_ptr<GameObject> planeObject;
        std::unique_ptr<Material> pbrMaterial;
        std::unique_ptr<Mesh> sphereMesh;
        std::unique_ptr<Mesh> cubeMesh;
        std::unique_ptr<Mesh> planeMesh;
        std::unique_ptr<PointLight> pointLight;
        std::unique_ptr<DirectionalLight> directionalLight;
        
        bool isRunning;
        float rotationAngle;

        void setupScene();
        void update(float deltaTime);
        void handleInput(float deltaTime);
    };
}