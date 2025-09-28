#include "../include/GUIManager.h"
#include "../include/GUIElement.h"
#include "../include/Button.h"
#include "../include/Logger.h"

namespace Sparky {

    GUIManager::GUIManager() : hoveredElement(nullptr), pressedElement(nullptr), 
                              visible(true), mouseX(0.0f), mouseY(0.0f) {
    }

    GUIManager::~GUIManager() {
    }

    GUIManager& GUIManager::getInstance() {
        static GUIManager instance;
        return instance;
    }

    void GUIManager::addElement(std::unique_ptr<GUIElement> element) {
        if (element) {
            const std::string& name = element->getName();
            elements[name] = std::move(element);
            SPARKY_LOG_DEBUG("Added GUI element: " + name);
        }
    }

    void GUIManager::removeElement(const std::string& name) {
        auto it = elements.find(name);
        if (it != elements.end()) {
            // Handle case where this element is currently hovered or pressed
            if (it->second.get() == hoveredElement) {
                hoveredElement = nullptr;
            }
            if (it->second.get() == pressedElement) {
                pressedElement = nullptr;
            }
            
            elements.erase(it);
            SPARKY_LOG_DEBUG("Removed GUI element: " + name);
        }
    }

    GUIElement* GUIManager::getElement(const std::string& name) {
        auto it = elements.find(name);
        if (it != elements.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    Button* GUIManager::createButton(const std::string& name) {
        auto button = std::make_unique<Button>(name);
        Button* buttonPtr = static_cast<Button*>(button.get());
        addElement(std::move(button));
        return buttonPtr;
    }

    void GUIManager::onMouseMove(float x, float y) {
        if (!visible) return;
        
        mouseX = x;
        mouseY = y;
        
        // Check if mouse is over any elements
        GUIElement* newHoveredElement = nullptr;
        
        for (auto& pair : elements) {
            GUIElement* element = pair.second.get();
            
            if (element->isVisible() && element->isEnabled()) {
                glm::vec2 pos = element->getPosition();
                glm::vec2 size = element->getSize();
                
                // Simple bounding box check
                if (x >= pos.x && x <= pos.x + size.x &&
                    y >= pos.y && y <= pos.y + size.y) {
                    newHoveredElement = element;
                    break;
                }
            }
        }
        
        // Handle hover state changes
        if (newHoveredElement != hoveredElement) {
            if (hoveredElement) {
                hoveredElement->onMouseExit();
            }
            
            hoveredElement = newHoveredElement;
            
            if (hoveredElement) {
                hoveredElement->onMouseEnter();
            }
        }
    }

    void GUIManager::onMousePress(int button) {
        if (!visible) return;
        
        if (hoveredElement) {
            pressedElement = hoveredElement;
            pressedElement->onMousePress();
        }
    }

    void GUIManager::onMouseRelease(int button) {
        if (!visible) return;
        
        if (pressedElement) {
            pressedElement->onMouseRelease();
            
            // If the same element is still hovered, trigger click
            if (pressedElement == hoveredElement) {
                pressedElement->onClick();
            }
            
            pressedElement = nullptr;
        }
    }

    void GUIManager::onKeyPress(int key) {
        if (!visible) return;
        
        // Handle keyboard navigation if needed
        // For now, just log the event
        SPARKY_LOG_DEBUG("Key pressed in GUI: " + std::to_string(key));
    }

    void GUIManager::update(float deltaTime) {
        if (!visible) return;
        
        // Update all visible elements
        for (auto& pair : elements) {
            GUIElement* element = pair.second.get();
            if (element->isVisible()) {
                element->update(deltaTime);
            }
        }
    }

    void GUIManager::render() {
        if (!visible) return;
        
        // Render all visible elements
        for (auto& pair : elements) {
            GUIElement* element = pair.second.get();
            if (element->isVisible()) {
                element->render();
            }
        }
    }

    void GUIManager::setVisible(bool visible) {
        this->visible = visible;
        SPARKY_LOG_DEBUG("GUI visibility set to: " + std::string(visible ? "true" : "false"));
    }
}