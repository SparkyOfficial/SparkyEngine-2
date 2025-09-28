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
        // Load the mesh from the file
        std::cout << "Loading mesh: " << name << " from " << filepath << std::endl;
        
        auto mesh = std::make_unique<Mesh>();
        // In a complete implementation, we would parse the file format and populate the mesh data
        
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
        // Load the texture from the file
        std::cout << "Loading texture: " << name << " from " << filepath << std::endl;
        // In a complete implementation, we would load and decode the image file
    }
}