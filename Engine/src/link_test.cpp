#include "../include/Mesh.h"
#include "../include/RenderComponent.h"
#include <memory>
#include <iostream>

int main() {
    // Test creating a cube mesh
    auto cube = Sparky::Mesh::createCube(1.0f);
    std::cout << "Created cube mesh" << std::endl;

    // Test creating a plane mesh
    auto plane = Sparky::Mesh::createPlane(1.0f, 1.0f);
    std::cout << "Created plane mesh" << std::endl;

    // Test RenderComponent setMesh
    Sparky::RenderComponent renderComponent;
    renderComponent.setMesh(std::move(cube));
    std::cout << "Set mesh in RenderComponent" << std::endl;

    return 0;
}