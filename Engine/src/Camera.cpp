#include "../include/Camera.h"
#include "../include/Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

namespace Sparky {

    // Constructor with vectors
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
        SPARKY_LOG_DEBUG("Camera created at position (" + std::to_string(Position.x) + ", " + 
                        std::to_string(Position.y) + ", " + std::to_string(Position.z) + ")");
    }

    // Constructor with scalar values
    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : 
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
        SPARKY_LOG_DEBUG("Camera created at position (" + std::to_string(Position.x) + ", " + 
                        std::to_string(Position.y) + ", " + std::to_string(Position.z) + ")");
    }

    Camera::~Camera() {
    }

    void Camera::setPosition(const glm::vec3& position) {
        this->Position = position;
        SPARKY_LOG_DEBUG("Camera position set to (" + std::to_string(Position.x) + ", " + 
                        std::to_string(Position.y) + ", " + std::to_string(Position.z) + ")");
    }

    void Camera::setFront(const glm::vec3& front) {
        this->Front = front;
    }

    void Camera::setUp(const glm::vec3& up) {
        this->Up = up;
    }

    void Camera::setSpeed(float speed) {
        this->MovementSpeed = speed;
    }

    void Camera::setSensitivity(float sensitivity) {
        this->MouseSensitivity = sensitivity;
    }

    void Camera::setZoom(float zoom) {
        this->Zoom = zoom;
    }

    glm::mat4 Camera::GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
        if (direction == UP)
            Position += Up * velocity;
        if (direction == DOWN)
            Position -= Up * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset) {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

    void Camera::updateCameraVectors() {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
    
    // Additional FPS camera methods
    void Camera::LookAt(const glm::vec3& target) {
        glm::vec3 direction = glm::normalize(target - Position);
        Front = direction;
        
        // Calculate pitch and yaw from direction
        Pitch = glm::degrees(asin(direction.y));
        Yaw = glm::degrees(atan2(direction.z, direction.x));
        
        // Update the other vectors
        updateCameraVectors();
    }
    
    glm::vec3 Camera::GetRight() const {
        return Right;
    }
    
    glm::vec3 Camera::GetUp() const {
        return Up;
    }
    
    glm::vec3 Camera::GetPosition() const {
        return Position;
    }
    
    float Camera::GetYaw() const {
        return Yaw;
    }
    
    float Camera::GetPitch() const {
        return Pitch;
    }
    
    float Camera::GetZoom() const {
        return Zoom;
    }
    
    float Camera::GetMovementSpeed() const {
        return MovementSpeed;
    }
    
    float Camera::GetMouseSensitivity() const {
        return MouseSensitivity;
    }
}