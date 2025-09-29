#pragma once

#include "GUIElement.h"
#include <vector>
#include <memory>
#include <string>

namespace Sparky {
    class Button;
    
    class GUIManager {
    public:
        static GUIManager& getInstance();
        
        void update(float deltaTime);
        void render();
        
        // Element management
        Button* createButton(const std::string& name);
        GUIElement* getElement(const std::string& name) const;
        void removeElement(const std::string& name);
        
        // HUD (Heads-Up Display) methods
        void createHUD();
        void createHealthBar();
        void createAmmoDisplay();
        void createCrosshair();
        void updateHealthDisplay(int health);
        void updateAmmoDisplay(int currentAmmo, int totalAmmo);
        
        // Menu methods
        void createMainMenu();
        void createPauseMenu();
        void showMenu(const std::string& menuName);
        void hideAllMenus();

    private:
        GUIManager();
        ~GUIManager();
        
        std::vector<std::unique_ptr<GUIElement>> elements;
    };
}