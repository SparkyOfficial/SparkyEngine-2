#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace Sparky {
    class GUIElement;
    class Button;

    class GUIManager {
    public:
        static GUIManager& getInstance();

        // Element management
        void addElement(std::unique_ptr<GUIElement> element);
        void removeElement(const std::string& name);
        GUIElement* getElement(const std::string& name);

        // Specific element creation
        Button* createButton(const std::string& name);

        // Input handling
        void onMouseMove(float x, float y);
        void onMousePress(int button);
        void onMouseRelease(int button);
        void onKeyPress(int key);

        // Update and render
        void update(float deltaTime);
        void render();

        // Visibility
        void setVisible(bool visible);
        bool isVisible() const { return visible; }

    private:
        GUIManager();
        ~GUIManager();

        std::unordered_map<std::string, std::unique_ptr<GUIElement>> elements;
        GUIElement* hoveredElement;
        GUIElement* pressedElement;
        bool visible;
        
        float mouseX, mouseY;
    };
}