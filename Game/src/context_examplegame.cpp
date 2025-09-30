#include "../../Engine/include/SparkyEngine.h"
#include "../../Engine/include/Mesh.h"
#include "../../Engine/include/RenderComponent.h"
#include <memory>

namespace Sparky {
    class ContextExampleGame {
    private:
        RenderSystem* renderSystem;
        
    public:
        ContextExampleGame() : renderSystem(nullptr) {}
        
        void createLevel() {
            // Test the same methods that fail in ExampleGame
            auto cube = Sparky::Mesh::createCube(1.0f);
            auto plane = Sparky::Mesh::createPlane(20.0f, 20.0f);
            
            Sparky::RenderComponent renderComponent;
            auto mesh = Sparky::Mesh::createCube(1.0f);
            renderComponent.setMesh(std::move(mesh));
        }
        
        void createPlayer() {
            // Test the same methods that fail in ExampleGame
            Sparky::RenderComponent renderComponent;
            auto playerMesh = Sparky::Mesh::createCube(1.0f);
            renderComponent.setMesh(std::move(playerMesh));
        }
        
        void createEnemy() {
            // Test the same methods that fail in ExampleGame
            Sparky::RenderComponent renderComponent;
            auto enemyMesh = Sparky::Mesh::createCube(1.0f);
            renderComponent.setMesh(std::move(enemyMesh));
        }
    };
}

int main() {
    Sparky::ContextExampleGame game;
    game.createLevel();
    game.createPlayer();
    game.createEnemy();
    return 0;
}