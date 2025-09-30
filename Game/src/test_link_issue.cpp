#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/RenderComponent.h"
#include <memory>

int main() {
    // Test the exact same calls as in ExampleGame.cpp
    auto planeMesh = Sparky::Mesh::createPlane(20.0f, 20.0f);
    
    Sparky::RenderComponent renderComponent;
    renderComponent.setMesh(std::move(planeMesh));
    
    return 0;
}