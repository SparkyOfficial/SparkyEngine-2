#include "../include/VulkanRenderer.h"
#include "../include/WindowManager.h"
#include "../include/Logger.h"
#include "../include/Mesh.h"
#include <memory>

int main() {
    // Test Mesh::createCube
    auto cube = Sparky::Mesh::createCube(1.0f);
    
    return 0;
}