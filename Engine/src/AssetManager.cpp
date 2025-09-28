#include "../include/AssetManager.h"
#include <iostream>

namespace Sparky {

    AssetManager::AssetManager() {
    }

    AssetManager::~AssetManager() {
    }

    AssetManager& AssetManager::getInstance() {
        static AssetManager instance;
        return instance;
    }

    void AssetManager::loadMesh(const std::string& name, const std::string& filepath) {
        // In a real implementation, we would load the mesh from the file
        // For now, we'll just create a placeholder
        std::cout << "Loading mesh: " << name << " from " << filepath << std::endl;
        
        auto mesh = std::make_unique<Mesh>();
        // Load mesh data from file...
        
        meshes[name] = std::move(mesh);
    }

    Mesh* AssetManager::getMesh(const std::string& name) {
        auto it = meshes.find(name);
        if (it != meshes.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    bool AssetManager::hasMesh(const std::string& name) {
        return meshes.find(name) != meshes.end();
    }

    void AssetManager::loadTexture(const std::string& name, const std::string& filepath) {
        // In a real implementation, we would load the texture from the file
        std::cout << "Loading texture: " << name << " from " << filepath << std::endl;
        // Load texture data from file...
    }
}