#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/RenderComponent.h"
#include <memory>

int main() {
    // Test the same methods that fail in ExampleGame
    auto cube = Sparky::Mesh::createCube(1.0f);
    auto plane = Sparky::Mesh::createPlane(20.0f, 20.0f);
    
    Sparky::RenderComponent renderComponent;
    auto mesh = Sparky::Mesh::createCube(1.0f);
    renderComponent.setMesh(std::move(mesh));
    
    return 0;
}