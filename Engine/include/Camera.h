#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {
    class Camera {
    public:
        Camera();
        ~Camera();

        // Camera attributes
        void setPosition(const glm::vec3& position);
        void setFront(const glm::vec3& front);
        void setUp(const glm::vec3& up);
        void setSpeed(float speed);
        void setSensitivity(float sensitivity);

        // Getters
        glm::vec3 getPosition() const { return position; }
        glm::vec3 getFront() const { return front; }
        glm::vec3 getUp() const { return up; }
        glm::vec3 getRight() const { return right; }
        glm::vec3 getWorldUp() const { return worldUp; }
        float getYaw() const { return yaw; }
        float getPitch() const { return pitch; }
        float getSpeed() const { return speed; }
        float getSensitivity() const { return sensitivity; }

        // View matrix
        glm::mat4 getViewMatrix() const;

        // Movement
        void processKeyboard(int direction, float deltaTime);
        void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void processMouseScroll(float yoffset);

    private:
        // Camera attributes
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        // Euler angles
        float yaw;
        float pitch;

        // Camera options
        float speed;
        float sensitivity;
        float zoom;

        // Calculate front vector
        void updateCameraVectors();
    };
}