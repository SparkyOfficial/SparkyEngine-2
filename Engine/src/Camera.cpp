#include "../include/Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {

    Camera::Camera() : position(0.0f, 0.0f, 0.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), 
                       right(1.0f, 0.0f, 0.0f), worldUp(0.0f, 1.0f, 0.0f), yaw(-90.0f), pitch(0.0f),
                       speed(2.5f), sensitivity(0.1f), zoom(45.0f) {
        updateCameraVectors();
    }

    Camera::~Camera() {
    }

    void Camera::setPosition(const glm::vec3& position) {
        this->position = position;
    }

    void Camera::setFront(const glm::vec3& front) {
        this->front = front;
    }

    void Camera::setUp(const glm::vec3& up) {
        this->up = up;
    }

    void Camera::setSpeed(float speed) {
        this->speed = speed;
    }

    void Camera::setSensitivity(float sensitivity) {
        this->sensitivity = sensitivity;
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::processKeyboard(int direction, float deltaTime) {
        float velocity = speed * deltaTime;
        if (direction == 0) // Forward
            position += front * velocity;
        if (direction == 1) // Backward
            position -= front * velocity;
        if (direction == 2) // Left
            position -= right * velocity;
        if (direction == 3) // Right
            position += right * velocity;
    }

    void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::processMouseScroll(float yoffset) {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    void Camera::updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
}