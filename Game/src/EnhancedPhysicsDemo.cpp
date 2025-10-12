#include "EnhancedPhysicsDemo.h"
#include "../../Engine/include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Sparky {

    EnhancedPhysicsDemo::EnhancedPhysicsDemo() : engine(nullptr), isRunning(true) {
    }

    EnhancedPhysicsDemo::~EnhancedPhysicsDemo() {
        cleanup();
    }

    bool EnhancedPhysicsDemo::initialize(Engine* engine) {
        this->engine = engine;
        
        if (!this->engine) {
            SPARKY_LOG_ERROR("Engine is null!");
            return false;
        }
        
        SPARKY_LOG_INFO("Initializing Enhanced Physics Demo");
        
        // Create materials
        groundMaterial = std::make_unique<Material>("GroundMaterial");
        groundMaterial->setDiffuse(glm::vec3(0.2f, 0.6f, 0.2f)); // Green
        // Note: Material doesn't have setRestitution/setFriction methods, using fixed values
        
        objectMaterial = std::make_unique<Material>("ObjectMaterial");
        objectMaterial->setDiffuse(glm::vec3(0.8f, 0.2f, 0.2f)); // Red
        // Note: Material doesn't have setRestitution/setFriction methods, using fixed values
        
        playerMaterial = std::make_unique<Material>("PlayerMaterial");
        playerMaterial->setDiffuse(glm::vec3(0.2f, 0.2f, 0.8f)); // Blue
        // Note: Material doesn't have setRestitution/setFriction methods, using fixed values
        
        createScene();
        
        SPARKY_LOG_INFO("Enhanced Physics Demo initialized successfully");
        return true;
    }

    void EnhancedPhysicsDemo::createScene() {
        setupGround();
        setupPlayer();
        createPhysicsObjects();
    }

    void EnhancedPhysicsDemo::setupGround() {
        ground = std::make_unique<GameObject>("Ground");
        ground->setPosition(glm::vec3(0.0f, -2.0f, 0.0f));
        ground->setScale(glm::vec3(20.0f, 1.0f, 20.0f));
        
        // Add render component
        auto renderComponent = ground->addComponent<RenderComponent>();
        auto groundMesh = Mesh::createCube(1.0f);
        renderComponent->setMesh(std::move(groundMesh));
        renderComponent->setMaterial(std::make_unique<Material>(*groundMaterial));
        
        // Add rigid body component
        auto groundRigidBody = ground->addComponent<RigidBodyComponent>();
        groundRigidBody->setBodyType(BodyType::STATIC); // Static ground
        groundRigidBody->setRestitution(0.2f); // Low bounciness
        groundRigidBody->setFriction(0.8f);    // High friction
        
        // Register with physics world
        PhysicsWorld::getInstance().addRigidBody(groundRigidBody);
        
        // Register with render system
        engine->getRenderSystem().registerGameObject(ground.get());
        objects.push_back(std::move(ground));
    }

    void EnhancedPhysicsDemo::setupPlayer() {
        player = std::make_unique<GameObject>("Player");
        player->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
        player->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
        
        // Add render component
        auto renderComponent = player->addComponent<RenderComponent>();
        auto playerMesh = Mesh::createCube(1.0f);
        renderComponent->setMesh(std::move(playerMesh));
        renderComponent->setMaterial(std::make_unique<Material>(*playerMaterial));
        
        // Add rigid body component
        auto playerRigidBody = player->addComponent<RigidBodyComponent>();
        playerRigidBody->setBodyType(BodyType::DYNAMIC); // Dynamic player
        playerRigidBody->setMass(70.0f); // Average human mass
        playerRigidBody->setRestitution(0.1f); // Low bounciness
        playerRigidBody->setFriction(0.9f);    // High friction
        
        // Register with physics world
        PhysicsWorld::getInstance().addRigidBody(playerRigidBody);
        
        // Register with render system
        engine->getRenderSystem().registerGameObject(player.get());
        objects.push_back(std::move(player));
    }

    void EnhancedPhysicsDemo::createPhysicsObjects() {
        // Create several objects with different properties to demonstrate physics
        
        // Sphere objects
        for (int i = 0; i < 3; i++) {
            auto sphere = std::make_unique<GameObject>("Sphere" + std::to_string(i));
            sphere->setPosition(glm::vec3(-3.0f + i * 3.0f, 10.0f, -2.0f + i * 2.0f));
            
            // Add render component
            auto renderComponent = sphere->addComponent<RenderComponent>();
            auto sphereMesh = Mesh::createSphere(0.5f, 16, 16);
            renderComponent->setMesh(std::move(sphereMesh));
            renderComponent->setMaterial(std::make_unique<Material>(*objectMaterial));
            
            // Add rigid body component
            auto sphereRigidBody = sphere->addComponent<RigidBodyComponent>();
            sphereRigidBody->setBodyType(BodyType::DYNAMIC);
            sphereRigidBody->setMass(5.0f);
            sphereRigidBody->setRestitution(0.7f); // Medium bounciness
            sphereRigidBody->setFriction(0.3f);    // Low friction
            
            // Register with physics world
            PhysicsWorld::getInstance().addRigidBody(sphereRigidBody);
            
            // Register with render system
            engine->getRenderSystem().registerGameObject(sphere.get());
            objects.push_back(std::move(sphere));
        }
        
        // Cube objects
        for (int i = 0; i < 3; i++) {
            auto cube = std::make_unique<GameObject>("Cube" + std::to_string(i));
            cube->setPosition(glm::vec3(-3.0f + i * 3.0f, 15.0f, 2.0f + i * 2.0f));
            cube->setScale(glm::vec3(0.8f, 0.8f, 0.8f));
            
            // Add render component
            auto renderComponent = cube->addComponent<RenderComponent>();
            auto cubeMesh = Mesh::createCube(1.0f);
            renderComponent->setMesh(std::move(cubeMesh));
            renderComponent->setMaterial(std::make_unique<Material>(*objectMaterial));
            
            // Add rigid body component
            auto cubeRigidBody = cube->addComponent<RigidBodyComponent>();
            cubeRigidBody->setBodyType(BodyType::DYNAMIC);
            cubeRigidBody->setMass(10.0f);
            cubeRigidBody->setRestitution(0.3f); // Default restitution
            cubeRigidBody->setFriction(0.5f); // Default friction
            
            // Register with physics world
            PhysicsWorld::getInstance().addRigidBody(cubeRigidBody);
            
            // Register with render system
            engine->getRenderSystem().registerGameObject(cube.get());
            objects.push_back(std::move(cube));
        }
        
        SPARKY_LOG_INFO("Created " + std::to_string(objects.size()) + " physics objects");
    }

    void EnhancedPhysicsDemo::update(float deltaTime) {
        if (!engine) return;
        
        handleInput(deltaTime);
        
        // Update physics world
        PhysicsWorld::getInstance().update(deltaTime);
    }

    void EnhancedPhysicsDemo::handleInput(float deltaTime) {
        if (!engine) return;
        
        InputManager& inputManager = engine->getInputManager();
        
        // Close application
        if (inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
            isRunning = false;
        }
        
        // Reset scene
        if (inputManager.isKeyJustPressed(GLFW_KEY_R)) {
            SPARKY_LOG_INFO("Resetting physics scene");
            cleanup();
            createScene();
        }
        
        // Apply force to player
        if (inputManager.isKeyPressed(GLFW_KEY_SPACE)) {
            auto playerRigidBody = player->getComponent<RigidBodyComponent>();
            if (playerRigidBody) {
                playerRigidBody->addForce(glm::vec3(0.0f, 500.0f, 0.0f));
            }
        }
        
        // Move player with arrow keys
        float force = 200.0f * deltaTime;
        auto playerRigidBody = player->getComponent<RigidBodyComponent>();
        if (playerRigidBody) {
            if (inputManager.isKeyPressed(GLFW_KEY_UP)) {
                playerRigidBody->addForce(glm::vec3(0.0f, 0.0f, -force));
            }
            if (inputManager.isKeyPressed(GLFW_KEY_DOWN)) {
                playerRigidBody->addForce(glm::vec3(0.0f, 0.0f, force));
            }
            if (inputManager.isKeyPressed(GLFW_KEY_LEFT)) {
                playerRigidBody->addForce(glm::vec3(-force, 0.0f, 0.0f));
            }
            if (inputManager.isKeyPressed(GLFW_KEY_RIGHT)) {
                playerRigidBody->addForce(glm::vec3(force, 0.0f, 0.0f));
            }
        }
    }

    void EnhancedPhysicsDemo::render() {
        // Rendering is handled by the engine's render system
    }

    void EnhancedPhysicsDemo::cleanup() {
        // Clean up is handled by unique_ptr destructors
        objects.clear();
    }
}