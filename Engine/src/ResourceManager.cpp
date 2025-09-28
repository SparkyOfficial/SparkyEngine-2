#include "../include/ResourceManager.h"
#include "../include/Logger.h"

namespace Sparky {

    ResourceManager::ResourceManager() {
    }

    ResourceManager::~ResourceManager() {
        unloadAllResources();
    }

    ResourceManager& ResourceManager::getInstance() {
        static ResourceManager instance;
        return instance;
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