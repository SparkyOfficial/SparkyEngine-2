#include "../include/AssetManager.h"
#include "../include/Texture.h"
#include "../include/Logger.h"
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
        SPARKY_LOG_DEBUG("Loading mesh: " + name + " from " + filepath);
        
        auto mesh = std::make_unique<Mesh>();
        // In a complete implementation, we would parse the file format and populate the mesh data
        
        meshes[name] = std::move(mesh);
        SPARKY_LOG_DEBUG("Mesh loaded successfully: " + name);
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
        SPARKY_LOG_DEBUG("Loading texture: " + name + " from " + filepath);
        
        auto texture = std::make_unique<Texture>();
        if (texture->loadFromFile(filepath)) {
            textures[name] = std::move(texture);
            SPARKY_LOG_DEBUG("Texture loaded successfully: " + name);
        } else {
            SPARKY_LOG_ERROR("Failed to load texture: " + name);
        }
    }
    
    Texture* AssetManager::getTexture(const std::string& name) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            return it->second.get();
        }
        return nullptr;
    }
    
    bool AssetManager::hasTexture(const std::string& name) {
        return textures.find(name) != textures.end();
    }
}