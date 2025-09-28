#pragma once

#include <string>
#include <unordered_map>
#include <memory>

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
        // In a real implementation, we would load the resource from the file
        // For now, we'll just create a placeholder
        auto resource = std::make_unique<T>();
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