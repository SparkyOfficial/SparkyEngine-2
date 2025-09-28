#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Sparky {
    enum class GUIElementType {
        WINDOW,
        BUTTON,
        LABEL,
        IMAGE,
        SLIDER,
        CHECKBOX,
        TEXTBOX
    };

    class GUIElement {
    public:
        GUIElement(GUIElementType type, const std::string& name = "GUIElement");
        virtual ~GUIElement();

        // Position and size
        void setPosition(const glm::vec2& position);
        void setSize(const glm::vec2& size);
        void setAnchorPoint(const glm::vec2& anchor);

        const glm::vec2& getPosition() const { return position; }
        const glm::vec2& getSize() const { return size; }
        const glm::vec2& getAnchorPoint() const { return anchorPoint; }

        // Visibility and enabled state
        void setVisible(bool visible);
        void setEnabled(bool enabled);

        bool isVisible() const { return visible; }
        bool isEnabled() const { return enabled; }

        // Color properties
        void setColor(const glm::vec4& color);
        void setHoverColor(const glm::vec4& color);
        void setPressedColor(const glm::vec4& color);

        const glm::vec4& getColor() const { return color; }
        const glm::vec4& getHoverColor() const { return hoverColor; }
        const glm::vec4& getPressedColor() const { return pressedColor; }

        // Text properties
        void setText(const std::string& text);
        const std::string& getText() const { return text; }

        // Element type
        GUIElementType getType() const { return type; }
        const std::string& getName() const { return name; }

        // Events
        virtual void onMouseEnter();
        virtual void onMouseExit();
        virtual void onMousePress();
        virtual void onMouseRelease();
        virtual void onClick();

        // Update and render
        virtual void update(float deltaTime);
        virtual void render();

    protected:
        std::string name;
        GUIElementType type;

        glm::vec2 position;
        glm::vec2 size;
        glm::vec2 anchorPoint;

        bool visible;
        bool enabled;
        bool hovered;
        bool pressed;

        glm::vec4 color;
        glm::vec4 hoverColor;
        glm::vec4 pressedColor;

        std::string text;
    };
}