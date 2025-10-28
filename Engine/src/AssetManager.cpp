#include "../include/AssetManager.h"
#include "../include/SparkyEngine.h"
#include "../include/MeshRenderer.h"
#include "../include/Logger.h"

namespace Sparky {

    AssetManager::AssetManager(Engine* engine) : engine(engine) {
    }

    AssetManager::~AssetManager() {
    }

    std::unique_ptr<Mesh> AssetManager::createCube(float size) {
        return Mesh::createCube(size);
    }

    std::unique_ptr<Mesh> AssetManager::createSphere(float radius, int segments, int rings) {
        return Mesh::createSphere(radius, segments, rings);
    }

    std::unique_ptr<Mesh> AssetManager::createCapsule(float radius, float height, int segments, int rings) {
        // Use sphere as fallback since createCapsule doesn't exist
        return Mesh::createSphere(radius, segments, rings);
    }

    std::unique_ptr<Mesh> AssetManager::createCylinder(float radius, float height, int segments) {
        // Use sphere as fallback since createCylinder doesn't exist
        return Mesh::createSphere(radius, segments, segments);
    }

    std::unique_ptr<Mesh> AssetManager::createPlane(float width, float height, int segments) {
        return Mesh::createPlane(width, height);
    }

    void AssetManager::createMeshBuffers(const Mesh& mesh) {
        if (engine) {
            // Use the renderer through the engine to create buffers
            MeshRenderer& meshRenderer = engine->getRenderer().getMeshRenderer();
            meshRenderer.createVertexBuffer(mesh);
            meshRenderer.createIndexBuffer(mesh);
        } else {
            Logger::getInstance().error("AssetManager: Engine is null, cannot create mesh buffers");
        }
    }

    void AssetManager::destroyMeshBuffers(const Mesh& mesh) {
        if (engine) {
            // Use the renderer through the engine to destroy buffers
            MeshRenderer& meshRenderer = engine->getRenderer().getMeshRenderer();
            // Note: MeshRenderer doesn't have explicit destroy methods, cleanup is handled automatically
        } else {
            Logger::getInstance().error("AssetManager: Engine is null, cannot destroy mesh buffers");
        }
    }
}