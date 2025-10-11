#include "PBRExample.h"
#include <iostream>
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

        // Add objects to the engine
        engine.addGameObject(sphereObject.get());
        engine.addGameObject(cubeObject.get());
        engine.addGameObject(planeObject.get());

        // Create lights
        pointLight = std::make_unique<PointLight>("PointLight");
        pointLight->setPosition(glm::vec3(0.0f, 5.0f, 5.0f));
        pointLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
        pointLight->setAttenuation(1.0f, 0.09f, 0.032f);

        directionalLight = std::make_unique<DirectionalLight>("DirectionalLight");
        directionalLight->setDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
        directionalLight->setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));

        // Add lights to the engine
        engine.addLight(pointLight.get());
        engine.addLight(directionalLight.get());

        // Configure camera
        Camera& camera = engine.getCamera();
        camera.SetPosition(glm::vec3(0.0f, 3.0f, 8.0f));
        camera.SetFront(glm::vec3(0.0f, 0.0f, -1.0f));
    }

    void PBRExample::run() {
        while (isRunning && !engine.shouldClose()) {
            float deltaTime = engine.getDeltaTime();
            
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
        InputManager& inputManager = engine.getInputManager();
        
        // Close application
        if (inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
            isRunning = false;
        }

        // Camera movement
        Camera& camera = engine.getCamera();
        float cameraSpeed = 5.0f * deltaTime;
        
        if (inputManager.isKeyPressed(GLFW_KEY_W)) {
            camera.ProcessKeyboard(FORWARD, cameraSpeed);
        }
        if (inputManager.isKeyPressed(GLFW_KEY_S)) {
            camera.ProcessKeyboard(BACKWARD, cameraSpeed);
        }
        if (inputManager.isKeyPressed(GLFW_KEY_A)) {
            camera.ProcessKeyboard(LEFT, cameraSpeed);
        }
        if (inputManager.isKeyPressed(GLFW_KEY_D)) {
            camera.ProcessKeyboard(RIGHT, cameraSpeed);
        }

        // Mouse look
        if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            double mouseX, mouseY;
            inputManager.getMousePosition(mouseX, mouseY);
            static double lastX = mouseX;
            static double lastY = mouseY;
            
            float xoffset = static_cast<float>(mouseX - lastX);
            float yoffset = static_cast<float>(lastY - mouseY);
            lastX = mouseX;
            lastY = mouseY;
            
            camera.ProcessMouseMovement(xoffset, yoffset);
        }
    }

    void PBRExample::cleanup() {
        // Clean up is handled by unique_ptr destructors
        engine.shutdown();
    }
}