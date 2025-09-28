#pragma once

#include "Component.h"
#include "Mesh.h"
#include <string>

namespace Sparky {
    class RenderComponent : public Component {
    public:
        RenderComponent();
        virtual ~RenderComponent();

        void update(float deltaTime) override;
        void render() override;

        void setMesh(Mesh* mesh);
        Mesh* getMesh() const { return mesh; }

        void setMaterial(const std::string& material);
        const std::string& getMaterial() const { return material; }

    private:
        Mesh* mesh;
        std::string material;
    };
}