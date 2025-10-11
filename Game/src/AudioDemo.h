#pragma once

#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/GameObject.h"
#include "../../Engine/include/AudioComponent.h"
#include "../../Engine/include/InputManager.h"
#include "../../Engine/include/Camera.h"
#include <memory>
#include <vector>

namespace Sparky {
    class AudioDemo {
    public:
        AudioDemo();
        ~AudioDemo();

        bool initialize(SparkyEngine* engine);
        void update(float deltaTime);
        void render();
        void cleanup();

        void createScene();

    private:
        SparkyEngine* engine;
        std::vector<std::unique_ptr<GameObject>> objects;
        std::unique_ptr<GameObject> player;
        std::unique_ptr<GameObject> soundSource1;
        std::unique_ptr<GameObject> soundSource2;
        
        // Audio components
        AudioComponent* playerAudio;
        AudioComponent* source1Audio;
        AudioComponent* source2Audio;
        
        // Controls
        bool isRunning;
        float soundTimer1;
        float soundTimer2;
        
        void setupPlayer();
        void setupSoundSources();
        void handleInput(float deltaTime);
    };
}