#include "../include/Scene.h"
#include "../include/RenderSystem.h"
#include "../include/PhysicsWorld.h"
#include "../include/Logger.h"
#include "../include/PhysicsComponent.h"
#include "../include/RigidBodyComponent.h"

namespace Sparky {

    Scene::Scene() : renderSystem(nullptr), physicsWorld(nullptr), active(true), physicsEnabled(true), gravity(0.0f, -9.81f, 0.0f) {
    }

    Scene::~Scene() {
        cleanup();
    }

    void Scene::initialize(RenderSystem* renderSystem, PhysicsWorld* physicsWorld) {
        this->renderSystem = renderSystem;
        this->physicsWorld = physicsWorld;
    }

    void Scene::cleanup() {
        // Clean up all game objects
        gameObjects.clear();
    }

    void Scene::update(float deltaTime) {
        if (!active) return;
        
        // Update all game objects
        for (auto& object : gameObjects) {
            object->update(deltaTime);
        }
        
        // Update physics if enabled
        if (physicsEnabled && physicsWorld) {
            physicsWorld->update(deltaTime);
        }
    }

    void Scene::render() {
        if (!active) return;
        
        // Render all game objects
        for (auto& object : gameObjects) {
            object->render();
        }
    }

    void Scene::addGameObject(std::unique_ptr<GameObject> object) {
        if (!object) return;
        
        registerGameObjectWithSystems(object.get());
        gameObjects.push_back(std::move(object));
    }

    void Scene::removeGameObject(const std::string& name) {
        gameObjects.erase(
            std::remove_if(gameObjects.begin(), gameObjects.end(),
                [&name](const std::unique_ptr<GameObject>& object) {
                    return object->getName() == name;
                }),
            gameObjects.end()
        );
    }

    GameObject* Scene::getGameObject(const std::string& name) {
        for (auto& object : gameObjects) {
            if (object->getName() == name) {
                return object.get();
            }
        }
        return nullptr;
    }

    void Scene::setActive(bool active) {
        this->active = active;
    }

    bool Scene::isActive() const {
        return active;
    }

    void Scene::setPhysicsEnabled(bool enabled) {
        physicsEnabled = enabled;
    }

    bool Scene::isPhysicsEnabled() const {
        return physicsEnabled;
    }

    void Scene::setGravity(const glm::vec3& gravity) {
        this->gravity = gravity;
        if (physicsWorld) {
            physicsWorld->setGravity(gravity);
        }
    }

    glm::vec3 Scene::getGravity() const {
        return gravity;
    }

    void Scene::setTag(const std::string& tag) {
        this->tag = tag;
    }

    const std::string& Scene::getTag() const {
        return tag;
    }

    size_t Scene::getGameObjectCount() const {
        return gameObjects.size();
    }

    void Scene::registerGameObjectWithSystems(GameObject* object) {
        if (!object) return;
        
        if (renderSystem) {
            // Register with render system
            renderSystem->registerGameObject(object);
        }
        
        if (physicsWorld && physicsEnabled) {
            // Register with physics world if the object has physics components
            PhysicsComponent* physicsComp = object->getComponent<PhysicsComponent>();
            if (physicsComp) {
                physicsWorld->addPhysicsComponent(physicsComp);
            }
            
            RigidBodyComponent* rigidBodyComp = object->getComponent<RigidBodyComponent>();
            if (rigidBodyComp) {
                physicsWorld->addRigidBody(rigidBodyComp);
            }
        }
    }

    void Scene::unregisterGameObjectFromSystems(GameObject* object) {
        if (!object) return;
        
        if (renderSystem) {
            // Unregister from render system
            renderSystem->unregisterGameObject(object);
        }
        
        if (physicsWorld && physicsEnabled) {
            // Unregister from physics world if the object has physics components
            PhysicsComponent* physicsComp = object->getComponent<PhysicsComponent>();
            if (physicsComp) {
                physicsWorld->removePhysicsComponent(physicsComp);
            }
            
            RigidBodyComponent* rigidBodyComp = object->getComponent<RigidBodyComponent>();
            if (rigidBodyComp) {
                physicsWorld->removeRigidBody(rigidBodyComp);
            }
        }
    }
}