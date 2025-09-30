#include "../include/OBJLoader.h"
#include "../include/Logger.h"
#include <iostream>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    SPARKY_LOG_INFO("Starting OBJ Loader Test");
    
    // Try to load the cube model
    auto mesh = Sparky::OBJLoader::loadFromFile("Engine/assets/cube.obj");
    
    if (!mesh) {
        SPARKY_LOG_ERROR("Failed to load cube model");
        return -1;
    }
    
    SPARKY_LOG_INFO("Cube model loaded successfully");
    SPARKY_LOG_INFO("Vertices: " + std::to_string(mesh->vertices.size()));
    SPARKY_LOG_INFO("Indices: " + std::to_string(mesh->indices.size()));
    
    // Print first few vertices
    for (size_t i = 0; i < std::min(mesh->vertices.size(), size_t(5)); ++i) {
        const auto& vertex = mesh->vertices[i];
        SPARKY_LOG_INFO("Vertex " + std::to_string(i) + ": Pos(" + 
                       std::to_string(vertex.position.x) + ", " + 
                       std::to_string(vertex.position.y) + ", " + 
                       std::to_string(vertex.position.z) + ")");
    }
    
    SPARKY_LOG_INFO("OBJ Loader test completed successfully");
    return 0;
}