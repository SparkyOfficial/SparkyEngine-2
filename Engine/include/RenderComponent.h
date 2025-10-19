#pragma once

#include "Component.h"
#include "Mesh.h"
#include "Material.h"
#include <memory>

#ifdef HAS_GLFW
#include <vulkan/vulkan.h>
#endif

namespace Sparky {
    class RenderComponent : public Component {
    public:
        RenderComponent();
        ~RenderComponent();

        void update(float deltaTime) override;
        void render() override;

        // Mesh management
        void setMesh(std::unique_ptr<Mesh> mesh);
        Mesh* getMesh() const { return mesh.get(); }
        
        // Material management
        void setMaterial(std::unique_ptr<Material> mat);
        Material* getMaterial() const { return material.get(); }
        
        // Getters and setters for rendering properties
        bool isVisible() const { return visible; }
        void setVisible(bool vis) { visible = vis; }
        
        // Vulkan-specific methods
#ifdef HAS_GLFW
        void createVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
        void createIndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue);
        void cleanup(VkDevice device);
#endif

    private:
        std::unique_ptr<Mesh> mesh;
        std::unique_ptr<Material> material;
        bool visible;
    };
}