#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "Mesh.h"

namespace Sparky {
    class AssetManager {
    public:
        static AssetManager& getInstance();

        // Mesh management
        void loadMesh(const std::string& name, const std::string& filepath);
        Mesh* getMesh(const std::string& name);
        bool hasMesh(const std::string& name);

        // Texture management (placeholder)
        void loadTexture(const std::string& name, const std::string& filepath);
        // Texture* getTexture(const std::string& name);

    private:
        AssetManager();
        ~AssetManager();

        std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
        // std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    };
}