#include "PhysicsDemo.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/RigidBodyComponent.h"
#include "../../Engine/include/CharacterController.h"
#include "../../Engine/include/RenderComponent.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/Logger.h"
#include "../../Engine/include/PhysicsWorld.h"
#include <glm/glm.hpp>

namespace Sparky {

    PhysicsDemo::PhysicsDemo() : player(nullptr), ground(nullptr) {
    }

    PhysicsDemo::~PhysicsDemo() {
        cleanup();
    }

    bool PhysicsDemo::initialize(Engine* engine) {
        if (!engine) {
            SPARKY_LOG_ERROR("Engine is null");
            return false;
        }

        this->engine = engine;
        
        try {
            createScene();
            SPARKY_LOG_INFO("Physics demo initialized successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to initialize physics demo: " + std::string(e.what()));
            return false;
        }
    }

    void PhysicsDemo::update(float deltaTime) {
        // Update physics world
        PhysicsWorld::getInstance().update(deltaTime);
    }

    void PhysicsDemo::render() {
        // Rendering is handled by the engine
    }

    void PhysicsDemo::cleanup() {
        // Clean up game objects
        if (player) {
            PhysicsWorld::getInstance().removePhysicsComponent(
                player->getComponent<RigidBodyComponent>()
            );
        }
        
        if (ground) {
            PhysicsWorld::getInstance().removePhysicsComponent(
                ground->getComponent<RigidBodyComponent>()
            );
        }
        
        objects.clear();
    }

    void PhysicsDemo::createScene() {
        // Create ground
        ground = std::make_unique<GameObject>("Ground");
        ground->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
        ground->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
        
        // Add render component
        RenderComponent* groundRender = ground->addComponent<RenderComponent>();
        auto groundMesh = Mesh::createCube(1.0f);
        groundRender->setMesh(Mesh::createCube(1.0f));
        meshes.push_back(std::move(groundMesh));
        
        // Add rigid body component
        RigidBodyComponent* groundRigidBody = ground->addComponent<RigidBodyComponent>();
        groundRigidBody->setBodyType(BodyType::STATIC); // Static ground
        PhysicsWorld::getInstance().addPhysicsComponent(groundRigidBody);
        
        // Register with render system
        engine->getRenderSystem().registerGameObject(ground.get());
        objects.push_back(std::move(ground));
        
        // Create player
        player = std::make_unique<GameObject>("Player");
        player->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
        player->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
        
        // Add render component
        RenderComponent* playerRender = player->addComponent<RenderComponent>();
        auto playerMesh = Mesh::createCube(1.0f);
        playerRender->setMesh(Mesh::createCube(1.0f));
        meshes.push_back(std::move(playerMesh));
        
        // Add rigid body component
        RigidBodyComponent* playerRigidBody = player->addComponent<RigidBodyComponent>();
        playerRigidBody->setBodyType(BodyType::DYNAMIC); // Dynamic player
        PhysicsWorld::getInstance().addPhysicsComponent(playerRigidBody);
        
        // Add character controller
        CharacterController* characterController = player->addComponent<CharacterController>();
        
        // Register with render system
        engine->getRenderSystem().registerGameObject(player.get());
        objects.push_back(std::move(player));
        
        SPARKY_LOG_INFO("Physics demo scene created with " + std::to_string(objects.size()) + " objects");
    }
}