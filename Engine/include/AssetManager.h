#pragma once

#include "Mesh.h"
#include <memory>

namespace Sparky {
    class Engine;
    class RenderSystem;
    
    class AssetManager {
    public:
        AssetManager(Engine* engine);
        ~AssetManager();

        // Mesh creation
        std::unique_ptr<Mesh> createCube(float size);
        std::unique_ptr<Mesh> createSphere(float radius, int segments, int rings);
        std::unique_ptr<Mesh> createCapsule(float radius, float height, int segments, int rings);
        std::unique_ptr<Mesh> createCylinder(float radius, float height, int segments);
        std::unique_ptr<Mesh> createPlane(float width, float height, int segments);
        
        // Buffer management (abstracted from direct renderer access)
        void createMeshBuffers(const Mesh& mesh);
        void destroyMeshBuffers(const Mesh& mesh);

    private:
        Engine* engine;
    };
}