#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/RenderComponent.h"
#include <memory>

namespace Sparky {
    class IsolatedExampleGame {
    public:
        void createLevel() {
            // Test the same methods that fail in ExampleGame
            auto cube = Sparky::Mesh::createCube(1.0f);
            auto plane = Sparky::Mesh::createPlane(20.0f, 20.0f);
            
            Sparky::RenderComponent renderComponent;
            auto mesh = Sparky::Mesh::createCube(1.0f);
            renderComponent.setMesh(std::move(mesh));
        }
    };
}

int main() {
    Sparky::IsolatedExampleGame game;
    game.createLevel();
    return 0;
}