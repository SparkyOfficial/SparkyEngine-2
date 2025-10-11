#include "../include/Camera.h"
#include "../include/Logger.h"
#include <iostream>

// Simple test to verify mouse look enhancements
int main() {
    // Initialize logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    SPARKY_LOG_INFO("Starting Mouse Look Enhancement Test");
    
    // Create a camera
    Sparky::Camera camera;
    
    // Test default settings
    SPARKY_LOG_INFO("Default sensitivity: " + std::to_string(camera.getSensitivity()));
    SPARKY_LOG_INFO("Default FOV: " + std::to_string(camera.getFOV()));
    
    // Test enhanced features
    camera.setMouseSmoothing(true);
    camera.setSmoothingFactor(0.8f);
    camera.setFOV(90.0f);
    camera.setInvertY(true);
    
    SPARKY_LOG_INFO("Enhanced settings:");
    SPARKY_LOG_INFO("  Mouse smoothing: " + std::string(camera.isMouseSmoothingEnabled() ? "enabled" : "disabled"));
    SPARKY_LOG_INFO("  Smoothing factor: " + std::to_string(camera.getSmoothingFactor()));
    SPARKY_LOG_INFO("  FOV: " + std::to_string(camera.getFOV()));
    SPARKY_LOG_INFO("  Y inversion: " + std::string(camera.isInvertY() ? "enabled" : "disabled"));
    
    // Test mouse movement processing
    SPARKY_LOG_INFO("Testing mouse movement processing...");
    camera.ProcessMouseMovement(10.0f, 5.0f);
    
    SPARKY_LOG_INFO("Camera Yaw: " + std::to_string(camera.getYaw()));
    SPARKY_LOG_INFO("Camera Pitch: " + std::to_string(camera.getPitch()));
    
    SPARKY_LOG_INFO("Mouse Look Enhancement Test completed successfully");
    
    return 0;
}