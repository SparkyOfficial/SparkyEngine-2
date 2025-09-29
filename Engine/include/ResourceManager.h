#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <iostream>

namespace Sparky {
    template<typename T>
    class Resource {
    public:
        Resource(const std::string& name, std::unique_ptr<T> data) 
            : name(name), data(std::move(data)) {}
        
        const std::string& getName() const { return name; }
        T* get() const { return data.get(); }
        
    private:
        std::string name;
        std::unique_ptr<T> data;
    };

    class ResourceManager {
    public:
        static ResourceManager& getInstance();

        template<typename T>
        void loadResource(const std::string& name, const std::string& filepath);

        template<typename T>
        T* getResource(const std::string& name);

        template<typename T>
        bool hasResource(const std::string& name);

        void unloadResource(const std::string& name);
        void unloadAllResources();

    private:
        ResourceManager();
        ~ResourceManager();

        struct ResourceBase {
            virtual ~ResourceBase() = default;
        };

        template<typename T>
        struct TypedResource : ResourceBase {
            std::unique_ptr<T> data;
            TypedResource(std::unique_ptr<T> data) : data(std::move(data)) {}
        };

        std::unordered_map<std::string, std::unique_ptr<ResourceBase>> resources;
    };

    // Template implementations need to be in header
    template<typename T>
    void ResourceManager::loadResource(const std::string& name, const std::string& filepath) {
        // Load the resource from the file
        auto resource = std::make_unique<T>();
        
        // Attempt to load resource data from the file
        // This is a generic implementation that can be specialized for specific types
        std::ifstream file(filepath, std::ios::binary);
        if (file.is_open()) {
            // For now, we'll just mark that we attempted to load the file
            // In a complete implementation, each resource type would have its own loading logic
            file.close();
        }
        
        auto typedResource = std::make_unique<TypedResource<T>>(std::move(resource));
        resources[name] = std::move(typedResource);
    }

    template<typename T>
    T* ResourceManager::getResource(const std::string& name) {
        auto it = resources.find(name);
        if (it != resources.end()) {
            auto* typedResource = dynamic_cast<TypedResource<T>*>(it->second.get());
            if (typedResource) {
                return typedResource->data.get();
            }
        }
        return nullptr;
    }

    template<typename T>
    bool ResourceManager::hasResource(const std::string& name) {
        return getResource<T>(name) != nullptr;
    }
}