#include "../include/ResourceManager.h"
#include "../include/Logger.h"
#include <memory>

namespace Sparky {

    // Default constructor
    ResourceManager::ResourceManager() : resourcePath("") {
    }

    // Constructor for dependency injection
    ResourceManager::ResourceManager(const std::string& resourcePath) : resourcePath(resourcePath) {
        SPARKY_LOG_DEBUG("ResourceManager created with resource path: " + resourcePath);
    }

    ResourceManager::~ResourceManager() {
        unloadAllResources();
    }

    // Singleton instance accessor (backward compatibility)
    ResourceManager& ResourceManager::getInstance() {
        static ResourceManager instance;
        return instance;
    }

    // Factory method for dependency injection
    std::unique_ptr<ResourceManager> ResourceManager::create(const std::string& resourcePath) {
        return std::make_unique<ResourceManager>(resourcePath);
    }

    void ResourceManager::unloadResource(const std::string& name) {
        auto it = resources.find(name);
        if (it != resources.end()) {
            resources.erase(it);
            SPARKY_LOG_DEBUG("Unloaded resource: " + name);
        }
    }

    void ResourceManager::unloadAllResources() {
        size_t count = resources.size();
        resources.clear();
        SPARKY_LOG_DEBUG("Unloaded all resources (" + std::to_string(count) + " resources)");
    }
}