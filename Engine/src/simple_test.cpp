#include "../include/Mesh.h"
#include "../include/RenderComponent.h"
#include <memory>

int main() {
    // Test Mesh::createCube
    auto cube = Sparky::Mesh::createCube(1.0f);
    
    // Test RenderComponent::setMesh
    Sparky::RenderComponent renderComponent;
    auto mesh = Sparky::Mesh::createCube(1.0f);
    renderComponent.setMesh(std::move(mesh));
    
    return 0;
}