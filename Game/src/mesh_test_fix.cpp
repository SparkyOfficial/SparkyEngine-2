#include <iostream>
#include <memory>
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/RenderComponent.h"

int main() {
    // Test creating a cube mesh
    auto cube = Sparky::Mesh::createCube(1.0f);
    if (cube) {
        std::cout << "Successfully created cube mesh with " << cube->getVertexCount() << " vertices" << std::endl;
    } else {
        std::cout << "Failed to create cube mesh" << std::endl;
        return 1;
    }

    // Test creating a plane mesh
    auto plane = Sparky::Mesh::createPlane(1.0f, 1.0f);
    if (plane) {
        std::cout << "Successfully created plane mesh with " << plane->getVertexCount() << " vertices" << std::endl;
    } else {
        std::cout << "Failed to create plane mesh" << std::endl;
        return 1;
    }

    // Test RenderComponent setMesh
    Sparky::RenderComponent renderComponent;
    renderComponent.setMesh(std::move(cube));
    std::cout << "Successfully set mesh in RenderComponent" << std::endl;

    std::cout << "All tests passed!" << std::endl;
    return 0;
}