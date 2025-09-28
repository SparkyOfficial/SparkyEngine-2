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
        // Render the button with the current texture
        // In a complete implementation, this would render the button using the graphics API
    }

}