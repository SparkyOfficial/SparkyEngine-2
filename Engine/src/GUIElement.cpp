#include "../include/GUIElement.h"
#include "../include/Logger.h"

namespace Sparky {

    GUIElement::GUIElement(GUIElementType type, const std::string& name) : 
        name(name), type(type),
        position(0.0f, 0.0f), size(100.0f, 100.0f), anchorPoint(0.0f, 0.0f),
        visible(true), enabled(true), hovered(false), pressed(false),
        color(1.0f, 1.0f, 1.0f, 1.0f), 
        hoverColor(0.8f, 0.8f, 0.8f, 1.0f),
        pressedColor(0.6f, 0.6f, 0.6f, 1.0f),
        text("") {
    }

    GUIElement::~GUIElement() {
    }

    void GUIElement::setPosition(const glm::vec2& position) {
        this->position = position;
    }

    void GUIElement::setSize(const glm::vec2& size) {
        this->size = size;
    }

    void GUIElement::setAnchorPoint(const glm::vec2& anchor) {
        this->anchorPoint = anchor;
    }

    void GUIElement::setVisible(bool visible) {
        this->visible = visible;
    }

    void GUIElement::setEnabled(bool enabled) {
        this->enabled = enabled;
    }

    void GUIElement::setColor(const glm::vec4& color) {
        this->color = color;
    }

    void GUIElement::setHoverColor(const glm::vec4& color) {
        this->hoverColor = color;
    }

    void GUIElement::setPressedColor(const glm::vec4& color) {
        this->pressedColor = color;
    }

    void GUIElement::setText(const std::string& text) {
        this->text = text;
    }

    void GUIElement::onMouseEnter() {
        hovered = true;
        SPARKY_LOG_DEBUG("Mouse entered GUI element: " + name);
    }

    void GUIElement::onMouseExit() {
        hovered = false;
        pressed = false;
        SPARKY_LOG_DEBUG("Mouse exited GUI element: " + name);
    }

    void GUIElement::onMousePress() {
        if (enabled) {
            pressed = true;
            SPARKY_LOG_DEBUG("Mouse pressed on GUI element: " + name);
        }
    }

    void GUIElement::onMouseRelease() {
        if (enabled) {
            pressed = false;
            SPARKY_LOG_DEBUG("Mouse released on GUI element: " + name);
        }
    }

    void GUIElement::onClick() {
        if (enabled) {
            SPARKY_LOG_DEBUG("GUI element clicked: " + name);
        }
    }

    void GUIElement::update(float deltaTime) {
        // Base implementation - can be overridden by derived classes
        // Handle animations, state changes, etc.
    }

    void GUIElement::render() {
        // Base implementation - can be overridden by derived classes
        // Render the element using the current graphics API
        SPARKY_LOG_DEBUG("Rendering GUI element: " + name);
    }
}