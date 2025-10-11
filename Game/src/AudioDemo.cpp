#include "AudioDemo.h"
#include "../../Engine/include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace Sparky {

    AudioDemo::AudioDemo() : engine(nullptr), isRunning(true), soundTimer1(0.0f), soundTimer2(0.0f) {
    }

    AudioDemo::~AudioDemo() {
        cleanup();
    }

    bool AudioDemo::initialize(SparkyEngine* engine) {
        this->engine = engine;
        
        if (!this->engine) {
            SPARKY_LOG_ERROR("Engine is null!");
            return false;
        }
        
        SPARKY_LOG_INFO("Initializing Audio Demo");
        
        createScene();
        
        SPARKY_LOG_INFO("Audio Demo initialized successfully");
        return true;
    }

    void AudioDemo::createScene() {
        setupPlayer();
        setupSoundSources();
    }

    void AudioDemo::setupPlayer() {
        player = std::make_unique<GameObject>("Player");
        player->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        
        // Add audio component to player
        playerAudio = player->addComponent<AudioComponent>();
        
        // Pre-load some sounds
        playerAudio->loadSound("beep", "beep.wav");
        playerAudio->loadSound("tone", "tone.wav");
        
        objects.push_back(std::move(player));
    }

    void AudioDemo::setupSoundSources() {
        // Create first sound source
        soundSource1 = std::make_unique<GameObject>("SoundSource1");
        soundSource1->setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
        
        // Add audio component
        source1Audio = soundSource1->addComponent<AudioComponent>();
        source1Audio->loadSound("loop1", "loop1.wav");
        
        // Set up advanced audio properties
        AudioSourceProperties props1;
        props1.minDistance = 2.0f;
        props1.maxDistance = 20.0f;
        props1.rolloffFactor = 1.0f;
        props1.enableDoppler = true;
        props1.dopplerFactor = 1.0f;
        
        // Play looping sound
        ALuint source1 = source1Audio->playSound("loop1", true);
        source1Audio->setSoundProperties(source1, props1);
        source1Audio->setSoundPosition(source1, soundSource1->getPosition());
        source1Audio->setSoundVolume(source1, 0.7f);
        
        objects.push_back(std::move(soundSource1));
        
        // Create second sound source
        soundSource2 = std::make_unique<GameObject>("SoundSource2");
        soundSource2->setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
        
        // Add audio component
        source2Audio = soundSource2->addComponent<AudioComponent>();
        source2Audio->loadSound("loop2", "loop2.wav");
        
        // Set up advanced audio properties
        AudioSourceProperties props2;
        props2.minDistance = 1.0f;
        props2.maxDistance = 15.0f;
        props2.rolloffFactor = 0.8f;
        props2.enableDoppler = false;
        
        // Play looping sound
        ALuint source2 = source2Audio->playSound("loop2", true);
        source2Audio->setSoundProperties(source2, props2);
        source2Audio->setSoundPosition(source2, soundSource2->getPosition());
        source2Audio->setSoundVolume(source2, 0.5f);
        
        objects.push_back(std::move(soundSource2));
    }

    void AudioDemo::update(float deltaTime) {
        if (!engine) return;
        
        handleInput(deltaTime);
        
        // Update sound positions based on player movement
        if (player && playerAudio) {
            glm::vec3 playerPos = player->getPosition();
            AudioEngine::getInstance().setListenerPosition(playerPos);
            
            // Set listener orientation (forward and up vectors)
            glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f); // Looking forward
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);       // Up vector
            AudioEngine::getInstance().setListenerOrientation(forward, up);
        }
        
        // Periodically play sounds from sources
        soundTimer1 += deltaTime;
        soundTimer2 += deltaTime;
        
        if (soundTimer1 > 3.0f && source1Audio) {
            source1Audio->playGunshot();
            soundTimer1 = 0.0f;
        }
        
        if (soundTimer2 > 5.0f && source2Audio) {
            source2Audio->playExplosion();
            soundTimer2 = 0.0f;
        }
    }

    void AudioDemo::handleInput(float deltaTime) {
        if (!engine) return;
        
        InputManager& inputManager = engine->getInputManager();
        
        // Close application
        if (inputManager.isKeyPressed(GLFW_KEY_ESCAPE)) {
            isRunning = false;
        }
        
        // Reset scene
        if (inputManager.isKeyJustPressed(GLFW_KEY_R)) {
            SPARKY_LOG_INFO("Resetting audio scene");
            cleanup();
            createScene();
        }
        
        // Move player with arrow keys
        float speed = 5.0f * deltaTime;
        glm::vec3 movement(0.0f);
        
        if (inputManager.isKeyPressed(GLFW_KEY_UP)) {
            movement.z -= speed;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_DOWN)) {
            movement.z += speed;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_LEFT)) {
            movement.x -= speed;
        }
        if (inputManager.isKeyPressed(GLFW_KEY_RIGHT)) {
            movement.x += speed;
        }
        
        if (glm::length(movement) > 0.0f && player) {
            glm::vec3 position = player->getPosition();
            position += movement;
            player->setPosition(position);
        }
        
        // Play sounds from player
        if (inputManager.isKeyJustPressed(GLFW_KEY_SPACE) && playerAudio) {
            playerAudio->playFootstep();
        }
        
        if (inputManager.isKeyJustPressed(GLFW_KEY_ENTER) && playerAudio) {
            ALuint source = playerAudio->playSound("beep");
            playerAudio->setSoundVolume(source, 0.8f);
        }
    }

    void AudioDemo::render() {
        // Rendering is handled by the engine's render system
    }

    void AudioDemo::cleanup() {
        // Clean up is handled by unique_ptr destructors
        objects.clear();
    }
}