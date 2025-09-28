#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Sparky {

    struct KeyFrame {
        float time;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
    };

    class Animation {
    private:
        std::string name;
        std::vector<KeyFrame> keyFrames;
        float duration;
        float currentTime;
        bool isPlaying;
        bool loop;

    public:
        Animation(const std::string& name = "Animation");
        ~Animation();

        void addKeyFrame(const KeyFrame& keyFrame);
        void play();
        void stop();
        void update(float deltaTime);
        bool isFinished() const;

        const std::string& getName() const { return name; }
        float getDuration() const { return duration; }
        float getCurrentTime() const { return currentTime; }
        bool getIsPlaying() const { return isPlaying; }
        void setLoop(bool loop) { this->loop = loop; }
        bool getLoop() const { return loop; }

        // Interpolation methods
        glm::vec3 interpolatePosition(float time) const;
        glm::vec3 interpolateRotation(float time) const;
        glm::vec3 interpolateScale(float time) const;
    };

}