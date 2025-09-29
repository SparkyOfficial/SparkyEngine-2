#include "../include/GUIManager.h"
#include "../include/Button.h"
#include "../include/Logger.h"
#include <algorithm>

namespace Sparky {
    
    GUIManager& GUIManager::getInstance() {
        static GUIManager instance;
        return instance;
    }
    
    GUIManager::GUIManager() {
        SPARKY_LOG_DEBUG("GUIManager created");
    }
    
    GUIManager::~GUIManager() {
        SPARKY_LOG_DEBUG("GUIManager destroyed");
    }
    
    void GUIManager::update(float deltaTime) {
        // Update all GUI elements
        for (auto& element : elements) {
            element->update(deltaTime);
        }
    }
    
    void GUIManager::render() {
        // Render all GUI elements
        for (auto& element : elements) {
            element->render();
        }
    }
    
    Button* GUIManager::createButton(const std::string& name) {
        // Check if button already exists
        auto it = std::find_if(elements.begin(), elements.end(),
                              [&name](const std::unique_ptr<GUIElement>& element) {
                                  return element->getName() == name;
                              });
        
        if (it != elements.end()) {
            // Return existing button
            return dynamic_cast<Button*>(it->get());
        }
        
        // Create new button
        auto button = std::make_unique<Button>(name);
        Button* buttonPtr = button.get();
        elements.push_back(std::move(button));
        
        SPARKY_LOG_DEBUG("Button created: " + name);
        return buttonPtr;
    }
    
    GUIElement* GUIManager::getElement(const std::string& name) const {
        auto it = std::find_if(elements.begin(), elements.end(),
                              [&name](const std::unique_ptr<GUIElement>& element) {
                                  return element->getName() == name;
                              });
        
        if (it != elements.end()) {
            return it->get();
        }
        
        return nullptr;
    }
    
    void GUIManager::removeElement(const std::string& name) {
        auto it = std::find_if(elements.begin(), elements.end(),
                              [&name](const std::unique_ptr<GUIElement>& element) {
                                  return element->getName() == name;
                              });
        
        if (it != elements.end()) {
            elements.erase(it);
            SPARKY_LOG_DEBUG("GUI element removed: " + name);
        }
    }
    
    // HUD (Heads-Up Display) methods
    void GUIManager::createHUD() {
        // Create health bar
        createHealthBar();
        
        // Create ammo display
        createAmmoDisplay();
        
        // Create crosshair
        createCrosshair();
    }
    
    void GUIManager::createHealthBar() {
        // Create a simple health bar using buttons or custom elements
        // For now, we'll create a text element to display health
        auto healthText = std::make_unique<Button>("HealthText");
        healthText->setPosition(glm::vec2(10.0f, 10.0f));
        healthText->setSize(glm::vec2(200.0f, 30.0f));
        healthText->setText("Health: 100");
        healthText->setVisible(true);
        elements.push_back(std::move(healthText));
    }
    
    void GUIManager::createAmmoDisplay() {
        // Create ammo display
        auto ammoText = std::make_unique<Button>("AmmoText");
        ammoText->setPosition(glm::vec2(10.0f, 50.0f));
        ammoText->setSize(glm::vec2(200.0f, 30.0f));
        ammoText->setText("Ammo: 30/120");
        ammoText->setVisible(true);
        elements.push_back(std::move(ammoText));
    }
    
    void GUIManager::createCrosshair() {
        // Create a simple crosshair
        auto crosshair = std::make_unique<Button>("Crosshair");
        crosshair->setPosition(glm::vec2(400.0f, 300.0f)); // Center of screen (assuming 800x600)
        crosshair->setSize(glm::vec2(20.0f, 20.0f));
        crosshair->setText("+");
        crosshair->setVisible(true);
        elements.push_back(std::move(crosshair));
    }
    
    void GUIManager::updateHealthDisplay(int health) {
        Button* healthText = static_cast<Button*>(getElement("HealthText"));
        if (healthText) {
            healthText->setText("Health: " + std::to_string(health));
        }
    }
    
    void GUIManager::updateAmmoDisplay(int currentAmmo, int totalAmmo) {
        Button* ammoText = static_cast<Button*>(getElement("AmmoText"));
        if (ammoText) {
            ammoText->setText("Ammo: " + std::to_string(currentAmmo) + "/" + std::to_string(totalAmmo));
        }
    }
    
    // Menu methods
    void GUIManager::createMainMenu() {
        // Create main menu background
        auto menuBackground = std::make_unique<Button>("MenuBackground");
        menuBackground->setPosition(glm::vec2(0.0f, 0.0f));
        menuBackground->setSize(glm::vec2(800.0f, 600.0f));
        menuBackground->setText("");
        menuBackground->setVisible(true);
        elements.push_back(std::move(menuBackground));
        
        // Create menu title
        auto menuTitle = std::make_unique<Button>("MenuTitle");
        menuTitle->setPosition(glm::vec2(300.0f, 100.0f));
        menuTitle->setSize(glm::vec2(200.0f, 50.0f));
        menuTitle->setText("SPARKY ENGINE");
        menuTitle->setVisible(true);
        elements.push_back(std::move(menuTitle));
        
        // Create start game button
        auto startButton = std::make_unique<Button>("StartButton");
        startButton->setPosition(glm::vec2(300.0f, 200.0f));
        startButton->setSize(glm::vec2(200.0f, 50.0f));
        startButton->setText("Start Game");
        startButton->setVisible(true);
        startButton->setOnClickCallback([]() {
            // In a full implementation, this would start the game
            SPARKY_LOG_INFO("Start Game button clicked");
        });
        elements.push_back(std::move(startButton));
        
        // Create options button
        auto optionsButton = std::make_unique<Button>("OptionsButton");
        optionsButton->setPosition(glm::vec2(300.0f, 270.0f));
        optionsButton->setSize(glm::vec2(200.0f, 50.0f));
        optionsButton->setText("Options");
        optionsButton->setVisible(true);
        optionsButton->setOnClickCallback([]() {
            // In a full implementation, this would open options menu
            SPARKY_LOG_INFO("Options button clicked");
        });
        elements.push_back(std::move(optionsButton));
        
        // Create quit button
        auto quitButton = std::make_unique<Button>("QuitButton");
        quitButton->setPosition(glm::vec2(300.0f, 340.0f));
        quitButton->setSize(glm::vec2(200.0f, 50.0f));
        quitButton->setText("Quit");
        quitButton->setVisible(true);
        quitButton->setOnClickCallback([]() {
            // In a full implementation, this would quit the game
            SPARKY_LOG_INFO("Quit button clicked");
        });
        elements.push_back(std::move(quitButton));
    }
    
    void GUIManager::createPauseMenu() {
        // Create pause menu background
        auto pauseBackground = std::make_unique<Button>("PauseBackground");
        pauseBackground->setPosition(glm::vec2(0.0f, 0.0f));
        pauseBackground->setSize(glm::vec2(800.0f, 600.0f));
        pauseBackground->setText("");
        pauseBackground->setVisible(false); // Hidden by default
        elements.push_back(std::move(pauseBackground));
        
        // Create pause title
        auto pauseTitle = std::make_unique<Button>("PauseTitle");
        pauseTitle->setPosition(glm::vec2(350.0f, 100.0f));
        pauseTitle->setSize(glm::vec2(100.0f, 50.0f));
        pauseTitle->setText("PAUSED");
        pauseTitle->setVisible(false); // Hidden by default
        elements.push_back(std::move(pauseTitle));
        
        // Create resume button
        auto resumeButton = std::make_unique<Button>("ResumeButton");
        resumeButton->setPosition(glm::vec2(300.0f, 200.0f));
        resumeButton->setSize(glm::vec2(200.0f, 50.0f));
        resumeButton->setText("Resume");
        resumeButton->setVisible(false); // Hidden by default
        resumeButton->setOnClickCallback([]() {
            // In a full implementation, this would resume the game
            SPARKY_LOG_INFO("Resume button clicked");
        });
        elements.push_back(std::move(resumeButton));
        
        // Create main menu button
        auto mainMenuButton = std::make_unique<Button>("PauseMenuButton");
        mainMenuButton->setPosition(glm::vec2(300.0f, 270.0f));
        mainMenuButton->setSize(glm::vec2(200.0f, 50.0f));
        mainMenuButton->setText("Main Menu");
        mainMenuButton->setVisible(false); // Hidden by default
        mainMenuButton->setOnClickCallback([]() {
            // In a full implementation, this would return to main menu
            SPARKY_LOG_INFO("Main Menu button clicked");
        });
        elements.push_back(std::move(mainMenuButton));
        
        // Create quit button
        auto quitButton = std::make_unique<Button>("PauseQuitButton");
        quitButton->setPosition(glm::vec2(300.0f, 340.0f));
        quitButton->setSize(glm::vec2(200.0f, 50.0f));
        quitButton->setText("Quit");
        quitButton->setVisible(false); // Hidden by default
        quitButton->setOnClickCallback([]() {
            // In a full implementation, this would quit the game
            SPARKY_LOG_INFO("Quit button clicked");
        });
        elements.push_back(std::move(quitButton));
    }
    
    void GUIManager::showMenu(const std::string& menuName) {
        // Hide all menus
        hideAllMenus();
        
        // Show the specified menu
        if (menuName == "main") {
            getElement("MenuBackground")->setVisible(true);
            getElement("MenuTitle")->setVisible(true);
            getElement("StartButton")->setVisible(true);
            getElement("OptionsButton")->setVisible(true);
            getElement("QuitButton")->setVisible(true);
        } else if (menuName == "pause") {
            getElement("PauseBackground")->setVisible(true);
            getElement("PauseTitle")->setVisible(true);
            getElement("ResumeButton")->setVisible(true);
            getElement("PauseMenuButton")->setVisible(true);
            getElement("PauseQuitButton")->setVisible(true);
        }
    }
    
    void GUIManager::hideAllMenus() {
        // Hide all menu elements
        getElement("MenuBackground")->setVisible(false);
        getElement("MenuTitle")->setVisible(false);
        getElement("StartButton")->setVisible(false);
        getElement("OptionsButton")->setVisible(false);
        getElement("QuitButton")->setVisible(false);
        
        getElement("PauseBackground")->setVisible(false);
        getElement("PauseTitle")->setVisible(false);
        getElement("ResumeButton")->setVisible(false);
        getElement("PauseMenuButton")->setVisible(false);
        getElement("PauseQuitButton")->setVisible(false);
    }
}