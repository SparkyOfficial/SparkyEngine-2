#include "../include/Button.h"
#include "../include/Logger.h"

namespace Sparky {

    Button::Button(const std::string& name) : GUIElement(GUIElementType::BUTTON, name),
                                             textureId(-1), hoverTextureId(-1), pressedTextureId(-1) {
    }

    Button::~Button() {
    }

    void Button::setTexture(int textureId) {
        this->textureId = textureId;
    }

    void Button::setHoverTexture(int textureId) {
        this->hoverTextureId = textureId;
    }

    void Button::setPressedTexture(int textureId) {
        this->pressedTextureId = textureId;
    }

    void Button::setOnClickCallback(std::function<void()> callback) {
        this->onClickCallback = callback;
    }

    void Button::onMouseEnter() {
        GUIElement::onMouseEnter();
        // Additional button-specific behavior on mouse enter
    }

    void Button::onMouseExit() {
        GUIElement::onMouseExit();
        // Additional button-specific behavior on mouse exit
    }

    void Button::onMousePress() {
        GUIElement::onMousePress();
        // Additional button-specific behavior on mouse press
    }

    void Button::onMouseRelease() {
        GUIElement::onMouseRelease();
        // Additional button-specific behavior on mouse release
    }

    void Button::onClick() {
        GUIElement::onClick();
        
        // Execute callback if set
        if (onClickCallback) {
            onClickCallback();
        }
    }

    void Button::update(float deltaTime) {
        GUIElement::update(deltaTime);
        // Button-specific update logic
    }

    void Button::render() {
        GUIElement::render();
        
        // Render button with appropriate texture based on state
        int currentTexture = textureId;
        if (pressed && pressedTextureId >= 0) {
            currentTexture = pressedTextureId;
        } else if (hovered && hoverTextureId >= 0) {
            currentTexture = hoverTextureId;
        }
        
        // In a real implementation, we would render the button with the current texture
        SPARKY_LOG_DEBUG("Rendering button with texture ID: " + std::to_string(currentTexture));
    }
}