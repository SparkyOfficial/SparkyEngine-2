#pragma once

#include "GUIElement.h"
#include <functional>

namespace Sparky {
    class Button : public GUIElement {
    public:
        Button(const std::string& name = "Button");
        ~Button();

        // Button-specific properties
        void setTexture(int textureId);
        void setHoverTexture(int textureId);
        void setPressedTexture(int textureId);

        int getTexture() const { return textureId; }
        int getHoverTexture() const { return hoverTextureId; }
        int getPressedTexture() const { return pressedTextureId; }

        // Callback function
        void setOnClickCallback(std::function<void()> callback);

        // Events
        void onMouseEnter() override;
        void onMouseExit() override;
        void onMousePress() override;
        void onMouseRelease() override;
        void onClick() override;

        // Update and render
        void update(float deltaTime) override;
        void render() override;

    private:
        int textureId;
        int hoverTextureId;
        int pressedTextureId;
        
        std::function<void()> onClickCallback;
    };
}