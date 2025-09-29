#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Sparky {
    class Camera {
    public:
        // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
        enum Camera_Movement {
            FORWARD,
            BACKWARD,
            LEFT,
            RIGHT,
            UP,
            DOWN
        };

        // Default camera values
        const float YAW = -90.0f;
        const float PITCH = 0.0f;
        const float SPEED = 2.5f;
        const float SENSITIVITY = 0.1f;
        const float ZOOM = 45.0f;

        // Camera Attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        
        // Euler Angles
        float Yaw;
        float Pitch;
        
        // Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        // Constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f, float pitch = 0.0f);
        
        // Constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
        
        // Destructor
        ~Camera();
        
        // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix();
        
        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
        
        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        
        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset);
        
        // Additional FPS camera method
        void LookAt(const glm::vec3& target);

        // Getters
        glm::vec3 getPosition() const { return Position; }
        glm::vec3 getFront() const { return Front; }
        glm::vec3 getUp() const { return Up; }
        glm::vec3 getRight() const { return Right; }
        glm::vec3 getWorldUp() const { return WorldUp; }
        float getYaw() const { return Yaw; }
        float getPitch() const { return Pitch; }
        float getSpeed() const { return MovementSpeed; }
        float getSensitivity() const { return MouseSensitivity; }
        float getZoom() const { return Zoom; }

        // Setters
        void setPosition(const glm::vec3& position);
        void setFront(const glm::vec3& front);
        void setUp(const glm::vec3& up);
        void setSpeed(float speed);
        void setSensitivity(float sensitivity);
        void setZoom(float zoom);

    private:
        // Calculates the front vector from the Camera's (updated) Euler Angles
        void updateCameraVectors();
    };
}