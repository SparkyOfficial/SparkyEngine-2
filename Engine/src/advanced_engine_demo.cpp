#include "../include/SparkyEngine.h"
#include "../include/GameObject.h"
#include "../include/CharacterController.h"
#include "../include/AdvancedAI.h"
#include "../include/AdvancedWeaponSystem.h"
#include "../include/AdvancedAnimationSystem.h"
#include "../include/AdvancedParticleSystem.h"
#include "../include/PBRMaterial.h"
#include "../include/RenderComponent.h"
#include "../include/Mesh.h"
#include "../include/Light.h"
#include "../include/Camera.h"
#include "../include/InputManager.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <memory>

#ifdef HAS_GLFW
#include <GLFW/glfw3.h>
#endif

/**
 * @brief Advanced Engine Demo
 * 
 * This demo showcases the advanced features of the SparkyEngine3:
 * - Advanced character controller with wall running and double jumping
 * - Sophisticated AI system with perception and group tactics
 * - Advanced weapon system with attachments and ballistics
 * - Skeletal animation with IK and blend trees
 * - Particle systems with forces and modifiers
 * - PBR rendering materials
 */

namespace Sparky {
    class AdvancedDemoGame {
    private:
        Engine m_engine;
        std::unique_ptr<GameObject> m_player;
        std::unique_ptr<GameObject> m_enemy;
        std::unique_ptr<GameObject> m_weapon;
        std::vector<std::unique_ptr<GameObject>> m_particles;
        std::vector<std::unique_ptr<GameObject>> m_environment;
        std::unique_ptr<Light> m_directionalLight;
        std::unique_ptr<Light> m_pointLight;
        
        // Game state
        bool m_initialized;
        bool m_running;
        float m_gameTime;
        
    public:
        AdvancedDemoGame() 
            : m_initialized(false)
            , m_running(false)
            , m_gameTime(0.0f) {
        }
        
        ~AdvancedDemoGame() {
            shutdown();
        }
        
        bool initialize() {
            Logger::getInstance().info("Initializing Advanced Engine Demo");
            
            // Initialize engine
            if (!m_engine.initialize(1280, 720, "Sparky Engine 3 - Advanced Demo")) {
                Logger::getInstance().error("Failed to initialize engine");
                return false;
            }
            
            // Create game objects
            createPlayer();
            createEnemy();
            createEnvironment();
            createLights();
            createWeapons();
            createParticleEffects();
            
            // Set up camera
            setupCamera();
            
            m_initialized = true;
            m_running = true;
            
            Logger::getInstance().info("Advanced Engine Demo initialized successfully");
            return true;
        }
        
        void run() {
            if (!m_initialized) {
                Logger::getInstance().error("Demo not initialized");
                return;
            }
            
            Logger::getInstance().info("Starting main game loop");
            
            float lastTime = 0.0f;
            
            while (m_running && !m_engine.getWindowManager().shouldClose()) {
                // Calculate delta time
#ifdef HAS_GLFW
                float currentTime = static_cast<float>(glfwGetTime());
#else
                static float fallbackTime = 0.0f;
                fallbackTime += 0.016f; // Assume 60 FPS
                float currentTime = fallbackTime;
#endif
                float deltaTime = currentTime - lastTime;
                lastTime = currentTime;
                
                m_gameTime += deltaTime;
                
                // Poll events
                m_engine.getWindowManager().pollEvents();
                
                // Update input
                updateInput(deltaTime);
                
                // Update game objects
                updateGameObjects(deltaTime);
                
                // Render frame
                m_engine.getRenderer().render();
                
                // Update shouldClose flag
                m_running = !m_engine.getWindowManager().shouldClose();
            }
            
            Logger::getInstance().info("Exiting main game loop");
        }
        
        void shutdown() {
            if (!m_initialized) return;
            
            Logger::getInstance().info("Shutting down Advanced Engine Demo");
            
            // Clean up game objects
            m_player.reset();
            m_enemy.reset();
            m_weapon.reset();
            m_particles.clear();
            m_environment.clear();
            m_directionalLight.reset();
            m_pointLight.reset();
            
            // Shutdown engine
            m_engine.shutdown();
            
            m_initialized = false;
            Logger::getInstance().info("Advanced Engine Demo shut down successfully");
        }
        
    private:
        void createPlayer() {
            Logger::getInstance().info("Creating player character");
            
            m_player = std::make_unique<GameObject>("Player");
            m_player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
            m_player->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
            
            // Add character controller
            CharacterController* characterController = m_player->addComponent<CharacterController>();
            characterController->setWalkSpeed(5.0f);
            characterController->setRunSpeed(10.0f);
            characterController->setJumpForce(8.0f);
            characterController->setGravity(-9.81f);
            
            // Add render component
            RenderComponent* renderComponent = m_player->addComponent<RenderComponent>();
            std::unique_ptr<Mesh> playerMesh = Mesh::createSphere(0.5f, 16, 16);
            renderComponent->setMesh(std::move(playerMesh));
            
            // Add PBR material
            std::unique_ptr<PBRMaterial> playerMaterial = std::make_unique<PBRMaterial>("PlayerMaterial");
            playerMaterial->setAlbedo(glm::vec3(0.2f, 0.5f, 1.0f));
            playerMaterial->setMetallic(0.1f);
            playerMaterial->setRoughness(0.7f);
            renderComponent->setMaterial(std::move(playerMaterial));
            
            // Add skeletal animation
            SkeletalAnimation* skeletalAnim = m_player->addComponent<SkeletalAnimation>();
            
            // Add animation controller
            AdvancedAnimationController* animController = m_player->addComponent<AdvancedAnimationController>();
            
            // Add weapon system
            AdvancedWeaponSystem* weaponSystem = m_player->addComponent<AdvancedWeaponSystem>();
            
            // Register with render system
            m_engine.getRenderSystem().registerGameObject(m_player.get());
            
            Logger::getInstance().info("Player character created successfully");
        }
        
        void createEnemy() {
            Logger::getInstance().info("Creating enemy AI");
            
            m_enemy = std::make_unique<GameObject>("Enemy");
            m_enemy->setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
            m_enemy->setScale(glm::vec3(1.0f, 1.8f, 1.0f));
            
            // Add AI component
            AdvancedAI* ai = m_enemy->addComponent<AdvancedAI>();
            ai->setDifficulty(0.8f);
            ai->setAggression(0.7f);
            ai->setTacticalAwareness(0.6f);
            
            // Add perception component
            PerceptionComponent* perception = m_enemy->addComponent<PerceptionComponent>();
            perception->setVisionRange(20.0f);
            perception->setHearingRange(30.0f);
            perception->setFieldOfView(90.0f);
            
            // Add character controller for movement
            CharacterController* characterController = m_enemy->addComponent<CharacterController>();
            characterController->setWalkSpeed(3.0f);
            characterController->setRunSpeed(6.0f);
            characterController->setJumpForce(6.0f);
            
            // Add render component
            RenderComponent* renderComponent = m_enemy->addComponent<RenderComponent>();
            std::unique_ptr<Mesh> enemyMesh = Mesh::createSphere(0.5f, 16, 16);
            renderComponent->setMesh(std::move(enemyMesh));

            // Add PBR material
            std::unique_ptr<PBRMaterial> enemyMaterial = std::make_unique<PBRMaterial>("EnemyMaterial");
            enemyMaterial->setAlbedo(glm::vec3(1.0f, 0.2f, 0.2f));
            enemyMaterial->setMetallic(0.3f);
            enemyMaterial->setRoughness(0.5f);
            renderComponent->setMaterial(std::move(enemyMaterial));
            
            // Register with render system
            m_engine.getRenderSystem().registerGameObject(m_enemy.get());
            
            Logger::getInstance().info("Enemy AI created successfully");
        }
        
        void createEnvironment() {
            Logger::getInstance().info("Creating environment");
            
            // Create ground
            auto ground = std::make_unique<GameObject>("Ground");
            ground->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
            ground->setScale(glm::vec3(20.0f, 1.0f, 20.0f));
            
            RenderComponent* groundRender = ground->addComponent<RenderComponent>();
            std::unique_ptr<Mesh> groundMesh = Mesh::createCube(1.0f);
            groundRender->setMesh(std::move(groundMesh));
            
            std::unique_ptr<PBRMaterial> groundMaterial = std::make_unique<PBRMaterial>("GroundMaterial");
            groundMaterial->setAlbedo(glm::vec3(0.5f, 0.5f, 0.5f));
            groundMaterial->setMetallic(0.0f);
            groundMaterial->setRoughness(0.9f);
            groundRender->setMaterial(std::move(groundMaterial));
            
            m_engine.getRenderSystem().registerGameObject(ground.get());
            m_environment.push_back(std::move(ground));
            
            // Create some platforms for wall running
            for (int i = 0; i < 5; ++i) {
                auto platform = std::make_unique<GameObject>("Platform" + std::to_string(i));
                platform->setPosition(glm::vec3(3.0f, 1.0f + i * 2.0f, 2.0f + i * 1.0f));
                platform->setScale(glm::vec3(2.0f, 0.2f, 1.0f));
                
                RenderComponent* platformRender = platform->addComponent<RenderComponent>();
                std::unique_ptr<Mesh> platformMesh = Mesh::createCube(1.0f);
                platformRender->setMesh(std::move(platformMesh));
                
                std::unique_ptr<PBRMaterial> platformMaterial = std::make_unique<PBRMaterial>("PlatformMaterial");
                platformMaterial->setAlbedo(glm::vec3(0.3f, 0.7f, 0.3f));
                platformMaterial->setMetallic(0.1f);
                platformMaterial->setRoughness(0.8f);
                platformRender->setMaterial(std::move(platformMaterial));
                
                m_engine.getRenderSystem().registerGameObject(platform.get());
                m_environment.push_back(std::move(platform));
            }
            
            Logger::getInstance().info("Environment created successfully");
        }
        
        void createLights() {
            Logger::getInstance().info("Creating lights");
            
            // Directional light (sun)
            m_directionalLight = std::make_unique<Light>("Sun");
            m_directionalLight->setDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
            m_directionalLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
            m_directionalLight->setAmbient(glm::vec3(0.2f, 0.2f, 0.2f));
            
            // Point light
            m_pointLight = std::make_unique<Light>("PointLight");
            m_pointLight->setPosition(glm::vec3(0.0f, 3.0f, 0.0f));
            m_pointLight->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
            m_pointLight->setAmbient(glm::vec3(0.1f, 0.1f, 0.1f));
            m_pointLight->setConstant(1.0f);
            m_pointLight->setLinear(0.09f);
            m_pointLight->setQuadratic(0.032f);
            
            Logger::getInstance().info("Lights created successfully");
        }
        
        void createWeapons() {
            Logger::getInstance().info("Creating weapons");
            
            // Create a weapon for the player
            auto rifle = std::make_unique<AdvancedWeapon>("Rifle", WeaponType::RIFLE);
            rifle->setDamage(25.0f);
            rifle->setFireRate(8.0f);
            rifle->setMagazineSize(30);
            rifle->setSpread(0.05f);
            rifle->setRecoil(2.0f);
            
            // Add scope attachment
            auto scope = std::make_unique<ScopeAttachment>("ACOG Scope", 4.0f, 0.7f);
            rifle->addAttachment(std::move(scope));
            
            // Add the weapon to the player's weapon system
            if (m_player) {
                AdvancedWeaponSystem* weaponSystem = m_player->getComponent<AdvancedWeaponSystem>();
                if (weaponSystem) {
                    weaponSystem->addWeapon(std::move(rifle));
                }
            }
            
            Logger::getInstance().info("Weapons created successfully");
        }
        
        void createParticleEffects() {
            Logger::getInstance().info("Creating particle effects");
            
            // Create explosion particle system
            auto explosion = std::make_unique<GameObject>("Explosion");
            explosion->setPosition(glm::vec3(0.0f, 2.0f, 5.0f));
            
            AdvancedParticleSystem* particleSystem = explosion->addComponent<AdvancedParticleSystem>(500);
            particleSystem->setDuration(2.0f);
            particleSystem->setLooping(false);
            particleSystem->setPlayOnAwake(true);
            
            // Add emitter
            auto emitter = std::make_unique<ParticleEmitter>(EmitterType::SPHERE);
            emitter->setEmissionRate(100.0f);
            emitter->setLifetimeRange(0.5f, 2.0f);
            emitter->setSpeedRange(2.0f, 10.0f);
            emitter->setSizeRange(glm::vec2(0.1f, 0.1f), glm::vec2(0.5f, 0.5f));
            emitter->setColorRange(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
            particleSystem->addEmitter(std::move(emitter));
            
            // Add gravity force
            auto gravity = std::make_unique<GravityForce>(glm::vec3(0.0f, -5.0f, 0.0f));
            particleSystem->addForce(std::move(gravity));
            
            // Add color modifier
            auto colorModifier = std::make_unique<ColorOverLifetimeModifier>();
            colorModifier->setColorRange(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
            particleSystem->addModifier(std::move(colorModifier));
            
            // Add size modifier
            auto sizeModifier = std::make_unique<SizeOverLifetimeModifier>();
            sizeModifier->setSizeRange(glm::vec2(0.1f, 0.1f), glm::vec2(1.0f, 1.0f));
            particleSystem->addModifier(std::move(sizeModifier));
            
            m_engine.getRenderSystem().registerGameObject(explosion.get());
            m_particles.push_back(std::move(explosion));
            
            Logger::getInstance().info("Particle effects created successfully");
        }
        
        void setupCamera() {
            Logger::getInstance().info("Setting up camera");
            
            Camera& camera = m_engine.getCamera();
            camera.setPosition(glm::vec3(0.0f, 3.0f, 10.0f));
            camera.setFront(glm::vec3(0.0f, 0.0f, -1.0f));
            camera.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
            camera.setYaw(-90.0f);
            camera.setPitch(0.0f);
            
            Logger::getInstance().info("Camera set up successfully");
        }
        
        void updateInput(float deltaTime) {
            InputManager& inputManager = m_engine.getInputManager();
            
            // Handle player movement
            if (m_player) {
                CharacterController* characterController = m_player->getComponent<CharacterController>();
                if (characterController) {
                    glm::vec3 moveDirection(0.0f, 0.0f, 0.0f);
                    
                    if (inputManager.isKeyPressed(GLFW_KEY_W)) {
                        moveDirection.z -= 1.0f;
                    }
                    if (inputManager.isKeyPressed(GLFW_KEY_S)) {
                        moveDirection.z += 1.0f;
                    }
                    if (inputManager.isKeyPressed(GLFW_KEY_A)) {
                        moveDirection.x -= 1.0f;
                    }
                    if (inputManager.isKeyPressed(GLFW_KEY_D)) {
                        moveDirection.x += 1.0f;
                    }
                    
                    characterController->move(moveDirection);
                    
                    if (inputManager.isKeyJustPressed(GLFW_KEY_SPACE)) {
                        characterController->jump();
                    }
                    
                    if (inputManager.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
                        characterController->sprint(true);
                    } else {
                        characterController->sprint(false);
                    }
                }
                
                // Handle weapon firing
#ifdef HAS_GLFW
                if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
                    AdvancedWeaponSystem* weaponSystem = m_player->getComponent<AdvancedWeaponSystem>();
                    if (weaponSystem) {
                        Camera& camera = m_engine.getCamera();
                        weaponSystem->fireCurrentWeapon(camera.getFront());
                    }
                }
#endif

                // Handle weapon reloading
                if (inputManager.isKeyJustPressed(GLFW_KEY_R)) {
                    AdvancedWeaponSystem* weaponSystem = m_player->getComponent<AdvancedWeaponSystem>();
                    if (weaponSystem) {
                        weaponSystem->reloadCurrentWeapon();
                    }
                }
            }
            
            // Handle camera movement
            Camera& camera = m_engine.getCamera();
            float cameraSpeed = 5.0f * deltaTime;
            
            if (inputManager.isKeyPressed(GLFW_KEY_UP)) {
                camera.processKeyboard(Camera::FORWARD, cameraSpeed);
            }
            if (inputManager.isKeyPressed(GLFW_KEY_DOWN)) {
                camera.processKeyboard(Camera::BACKWARD, cameraSpeed);
            }
            if (inputManager.isKeyPressed(GLFW_KEY_LEFT)) {
                camera.processKeyboard(Camera::LEFT, cameraSpeed);
            }
            if (inputManager.isKeyPressed(GLFW_KEY_RIGHT)) {
                camera.processKeyboard(Camera::RIGHT, cameraSpeed);
            }

            // Handle mouse look
#ifdef HAS_GLFW
            if (inputManager.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
                double xpos, ypos;
                glfwGetCursorPos(static_cast<GLFWwindow*>(m_engine.getWindowManager().getWindowHandle()), &xpos, &ypos);
                camera.processMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
            }
#endif

            // Handle exit
            if (inputManager.isKeyJustPressed(GLFW_KEY_ESCAPE)) {
                m_running = false;
            }
        }
        
        void updateGameObjects(float deltaTime) {
            // Update player
            if (m_player) {
                m_player->update(deltaTime);
            }
            
            // Update enemy
            if (m_enemy) {
                m_enemy->update(deltaTime);
                
                // Simple AI behavior
                AdvancedAI* ai = m_enemy->getComponent<AdvancedAI>();
                if (ai && m_player) {
                    // Set player as target
                    ai->attack(m_player.get());
                    
                    // Move toward player
                    glm::vec3 playerPos = m_player->getPosition();
                    glm::vec3 enemyPos = m_enemy->getPosition();
                    glm::vec3 direction = glm::normalize(playerPos - enemyPos);
                    ai->moveTo(enemyPos + direction * 2.0f);
                }
            }
            
            // Update particles
            for (auto& particle : m_particles) {
                particle->update(deltaTime);
            }
            
            // Update environment
            for (auto& env : m_environment) {
                env->update(deltaTime);
            }
            
            // Animate lights
            if (m_pointLight) {
                float lightIntensity = 0.5f + 0.5f * sin(m_gameTime * 2.0f);
                m_pointLight->setDiffuse(glm::vec3(lightIntensity, lightIntensity * 0.8f, lightIntensity * 0.6f));
            }
        }
    };
}

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    Sparky::Logger::getInstance().info("Starting Sparky Engine 3 - Advanced Demo");
    
    Sparky::AdvancedDemoGame demo;
    
    if (!demo.initialize()) {
        Sparky::Logger::getInstance().error("Failed to initialize demo");
        return -1;
    }
    
    demo.run();
    
    Sparky::Logger::getInstance().info("Demo finished successfully");
    return 0;
}