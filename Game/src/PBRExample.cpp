#include "PBRExample.h"
#include <iostream>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    PBRExample::PBRExample() : isRunning(false), rotationAngle(0.0f) {
    }

    PBRExample::~PBRExample() {
        cleanup();
    }

    bool PBRExample::initialize() {
        // Initialize the engine
        if (!engine.initialize(1280, 720, "Sparky Engine - PBR Example")) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return false;
        }

        setupScene();
        isRunning = true;
        return true;
    }

    void PBRExample::setupScene() {
        // Create meshes
        sphereMesh = Mesh::createSphere(1.0f, 32, 32);
        cubeMesh = Mesh::createCube(2.0f);
        planeMesh = Mesh::createPlane(10.0f, 10.0f);

        // Create PBR material
        pbrMaterial = std::make_unique<Material>("PBR_Material");
        pbrMaterial->setDiffuse(glm::vec3(0.8f, 0.1f, 0.1f)); // Red diffuse
        pbrMaterial->setRoughness(0.2f); // Smooth surface
        pbrMaterial->setMetalness(0.8f); // Metallic

        // Create game objects
        sphereObject = std::make_unique<GameObject>("Sphere");
        sphereObject->addComponent<RenderComponent>(sphereMesh.get(), pbrMaterial.get());
        sphereObject->setPosition(glm::vec3(-2.0f, 1.0f, 0.0f));

        cubeObject = std::make_unique<GameObject>("Cube");
        cubeObject->addComponent<RenderComponent>(cubeMesh.get(), pbrMaterial.get());
        cubeObject->setPosition(glm::vec3(2.0f, 1.0f, 0.0f));

        planeObject = std::make_unique<GameObject>("Plane");
        planeObject->addComponent<RenderComponent>(planeMesh.get(), pbrMaterial.get());
        planeObject->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        planeObject->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));

        // Register objects with the render system
        // Note: The Engine class doesn't have addGameObject method
        // In a full implementation, we would register with the render system directly

        // Create lights
        pointLight = std::make_unique<PointLight>("PointLight");
        pointLight->setPosition(glm::vec3(0.0f, 5.0f, 5.0f));
        pointLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLight->setConstant(1.0f);
        pointLight->setLinear(0.09f);
        pointLight->setQuadratic(0.032f);

        directionalLight = std::make_unique<DirectionalLight>("DirectionalLight");
        directionalLight->setDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
        directionalLight->setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));

        // Configure camera
        Camera& camera = engine.getCamera();
        camera.SetPosition(glm::vec3(0.0f, 3.0f, 8.0f));
        camera.SetFront(glm::vec3(0.0f, 0.0f, -1.0f));
    }

    void PBRExample::run() {
        // Note: The Engine class doesn't have shouldClose method
        // In a full implementation, we would check the window manager
        while (isRunning) {
            float deltaTime = 0.016f; // Assume 60 FPS
            
            handleInput(deltaTime);
            update(deltaTime);
            
            engine.update();
            engine.render();
        }
    }

    void PBRExample::update(float deltaTime) {
        // Rotate objects
        rotationAngle += 20.0f * deltaTime;
        
        if (sphereObject) {
            sphereObject->setRotation(glm::vec3(0.0f, rotationAngle, 0.0f));
        }
        
        if (cubeObject) {
            cubeObject->setRotation(glm::vec3(rotationAngle, rotationAngle, rotationAngle));
        }
    }

    void PBRExample::handleInput(float deltaTime) {
        // Note: The Engine class doesn't have getInputManager method
        // In a full implementation, we would get the input manager differently
        
        // For now, we'll just check for ESC key to close
        // In a full implementation, we would check actual input
        static bool escapePressed = false;
        if (!escapePressed) {
            // Simulate ESC key press after some time
            escapePressed = true;
            isRunning = false;
        }
    }

    void PBRExample::cleanup() {
        // Clean up is handled by unique_ptr destructors
        engine.shutdown();
    }
}