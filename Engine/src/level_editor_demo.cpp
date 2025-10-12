/*
 * level_editor_demo.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple demo to demonstrate the level editor
 */

#include "../include/SparkyEngine.h"
#include "../include/LevelEditor.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>

using namespace Sparky;

class LevelEditorDemo {
private:
    std::unique_ptr<LevelEditor> levelEditor;

public:
    LevelEditorDemo() {
        levelEditor = std::make_unique<LevelEditor>();
    }

    ~LevelEditorDemo() {
    }

    bool initialize() {
        SPARKY_LOG_INFO("Initializing Level Editor Demo...");

        // Initialize the level editor
        if (!levelEditor->initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize LevelEditor");
            return false;
        }

        // Set up editor settings
        GridSettings gridSettings;
        gridSettings.enabled = true;
        gridSettings.size = 1.0f;
        gridSettings.snapThreshold = 0.1f;
        gridSettings.showGrid = true;
        levelEditor->setGridSettings(gridSettings);

        EditorCameraSettings cameraSettings;
        cameraSettings.moveSpeed = 5.0f;
        cameraSettings.rotationSpeed = 0.1f;
        cameraSettings.zoomSpeed = 1.0f;
        cameraSettings.invertY = false;
        levelEditor->setCameraSettings(cameraSettings);

        // Set up editor callbacks
        levelEditor->setOnLevelChangeCallback([]() {
            SPARKY_LOG_INFO("Level changed");
        });

        levelEditor->setOnSelectionChangeCallback([]() {
            SPARKY_LOG_INFO("Selection changed");
        });

        levelEditor->setOnObjectModifyCallback([]() {
            SPARKY_LOG_INFO("Object modified");
        });

        // Enable auto-save
        levelEditor->setAutoSave(true);

        SPARKY_LOG_INFO("Level Editor Demo initialized successfully!");
        return true;
    }

    void update(float deltaTime) {
        // Update the level editor
        levelEditor->update(deltaTime);
        
        // Print editor status
        SPARKY_LOG_INFO("Editor mode: " + std::to_string(static_cast<int>(levelEditor->getMode())));
        SPARKY_LOG_INFO("Selected object index: " + std::to_string(levelEditor->getSelectedObjectIndex()));
        
        // Print level statistics
        if (levelEditor->getCurrentLevel()) {
            SPARKY_LOG_INFO("Objects: " + std::to_string(levelEditor->getObjectCount()));
            SPARKY_LOG_INFO("Interactive elements: " + std::to_string(levelEditor->getInteractiveElementCount()));
            SPARKY_LOG_INFO("Trigger volumes: " + std::to_string(levelEditor->getTriggerVolumeCount()));
        }
    }

    void run() {
        if (!initialize()) {
            SPARKY_LOG_ERROR("Failed to initialize Level Editor Demo!");
            return;
        }

        // Create some objects in the level
        ObjectCreationParams params;
        params.type = "Cube";
        params.name = "TestCube1";
        params.position[0] = 0.0f; params.position[1] = 0.0f; params.position[2] = 0.0f;
        params.rotation[0] = 0.0f; params.rotation[1] = 0.0f; params.rotation[2] = 0.0f;
        params.scale[0] = 1.0f; params.scale[1] = 1.0f; params.scale[2] = 1.0f;
        params.material = "Default";
        params.mass = 1.0f;
        params.interactive = false;
        levelEditor->createObject(params);

        params.type = "Sphere";
        params.name = "TestSphere1";
        params.position[0] = 3.0f; params.position[1] = 0.0f; params.position[2] = 0.0f;
        levelEditor->createObject(params);

        params.type = "Cylinder";
        params.name = "TestCylinder1";
        params.position[0] = -3.0f; params.position[1] = 0.0f; params.position[2] = 0.0f;
        levelEditor->createObject(params);

        // Create an interactive element
        InteractiveCreationParams interactiveParams;
        interactiveParams.type = "Door";
        interactiveParams.name = "TestDoor1";
        interactiveParams.position[0] = 0.0f; interactiveParams.position[1] = 0.0f; interactiveParams.position[2] = 3.0f;
        interactiveParams.rotation[0] = 0.0f; interactiveParams.rotation[1] = 0.0f; interactiveParams.rotation[2] = 0.0f;
        interactiveParams.targetObject = "TestCube1";
        interactiveParams.state = "closed";
        levelEditor->createInteractiveElement(interactiveParams);

        // Create a trigger volume
        TriggerCreationParams triggerParams;
        triggerParams.name = "TestTrigger1";
        triggerParams.position[0] = 0.0f; triggerParams.position[1] = 1.0f; triggerParams.position[2] = 0.0f;
        triggerParams.size[0] = 2.0f; triggerParams.size[1] = 2.0f; triggerParams.size[2] = 2.0f;
        triggerParams.triggerType = "enter";
        triggerParams.target = "TestDoor1";
        levelEditor->createTriggerVolume(triggerParams);

        // Simulate for 5 seconds
        float totalTime = 0.0f;
        float deltaTime = 1.0f / 60.0f; // 60 FPS
        
        SPARKY_LOG_INFO("Starting level editor simulation...");
        
        while (totalTime < 5.0f) {
            update(deltaTime);
            totalTime += deltaTime;
            
            // Every second, perform some editor actions
            if (static_cast<int>(totalTime) > static_cast<int>(totalTime - deltaTime)) {
                static int actionCounter = 0;
                actionCounter++;
                
                switch (actionCounter % 4) {
                    case 0:
                        // Select an object
                        levelEditor->selectObject(0);
                        SPARKY_LOG_INFO("Selected object 0");
                        break;
                    case 1:
                        // Move selected object
                        if (levelEditor->isObjectSelected()) {
                            levelEditor->moveSelectedObject(1.0f, 0.0f, 1.0f);
                            SPARKY_LOG_INFO("Moved selected object");
                        }
                        break;
                    case 2:
                        // Duplicate selected object
                        if (levelEditor->isObjectSelected()) {
                            levelEditor->duplicateSelectedObject();
                            SPARKY_LOG_INFO("Duplicated selected object");
                        }
                        break;
                    case 3:
                        // Create a new object
                        ObjectCreationParams newParams;
                        newParams.type = "Cube";
                        newParams.name = "NewCube" + std::to_string(actionCounter);
                        newParams.position[0] = static_cast<float>(actionCounter);
                        newParams.position[1] = 1.0f;
                        newParams.position[2] = static_cast<float>(actionCounter);
                        newParams.scale[0] = 0.5f; newParams.scale[1] = 0.5f; newParams.scale[2] = 0.5f;
                        levelEditor->createObject(newParams);
                        SPARKY_LOG_INFO("Created new object: " + newParams.name);
                        break;
                }
            }
            
            // Sleep to simulate frame time (in a real application, this would be handled by the engine)
            // std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        // Save the level
        levelEditor->saveLevel("test_level.level");
        SPARKY_LOG_INFO("Level saved to test_level.level");
        
        SPARKY_LOG_INFO("Level editor simulation completed!");
    }
};

int main() {
    try {
        LevelEditorDemo demo;
        demo.run();
        return 0;
    } catch (const std::exception& e) {
        SPARKY_LOG_ERROR("Exception occurred: " + std::string(e.what()));
        return -1;
    }
}