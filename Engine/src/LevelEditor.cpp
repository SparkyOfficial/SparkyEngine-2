/*
 * LevelEditor.cpp
 * 
 * Author: Андрій Будильников
 * 
 * Simple level editor implementation for creating and editing game levels
 */

#include "../include/LevelEditor.h"
#include "../include/Logger.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

namespace Sparky {
    
    LevelEditor::LevelEditor() : 
        currentMode(EditorMode::SELECT), currentTool(EditorTool::SELECT),
        editing(false), selectedObjectIndex(-1), gizmoEnabled(true),
        viewportWidth(800), viewportHeight(600),
        maxUndoSteps(50), clipboardHasData(false),
        autoSaveEnabled(false), autoSaveInterval(300.0f), lastAutoSaveTime(0.0f) {
        SPARKY_LOG_DEBUG("LevelEditor created");
    }
    
    LevelEditor::~LevelEditor() {
        cleanup();
        SPARKY_LOG_DEBUG("LevelEditor destroyed");
    }
    
    bool LevelEditor::initialize() {
        SPARKY_LOG_INFO("Initializing LevelEditor");
        
        try {
            // Create a new empty level
            newLevel();
            
            // Initialize default settings
            gridSettings = GridSettings();
            cameraSettings = EditorCameraSettings();
            
            // Clear undo/redo stacks
            clearUndoStack();
            
            editing = true;
            
            SPARKY_LOG_INFO("LevelEditor initialized successfully");
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Exception during LevelEditor initialization: " + std::string(e.what()));
            return false;
        }
    }
    
    void LevelEditor::cleanup() {
        SPARKY_LOG_INFO("Cleaning up LevelEditor");
        
        // Close current level
        closeLevel();
        
        // Clear undo/redo stacks
        clearUndoStack();
        
        // Clear clipboard
        clipboardHasData = false;
        
        editing = false;
        
        SPARKY_LOG_INFO("LevelEditor cleaned up");
    }
    
    void LevelEditor::update(float deltaTime) {
        if (!editing) return;
        
        // Handle user input
        handleInput(deltaTime);
        
        // Update camera
        updateCamera(deltaTime);
        
        // Check for auto-save
        checkAutoSave(deltaTime);
        
        // Update current level
        if (currentLevel) {
            // In a full implementation, we would update the level
            // For now, we'll just log that we're updating
        }
    }
    
    void LevelEditor::render() {
        if (!editing) return;
        
        // Render the scene
        if (currentLevel) {
            // In a full implementation, we would render the level
            // For now, we'll just log that we're rendering
            SPARKY_LOG_DEBUG("Rendering level in editor");
        }
        
        // Render editor UI elements
        renderGrid();
        if (gizmoEnabled && selectedObjectIndex != -1) {
            renderGizmo();
        }
        
        // Render UI panels
        renderMenuBar();
        renderToolbar();
        renderSceneHierarchy();
        renderPropertiesPanel();
        renderAssetBrowser();
        renderStatusBar();
    }
    
    bool LevelEditor::loadLevel(const std::string& filepath) {
        SPARKY_LOG_INFO("Loading level from: " + filepath);
        
        try {
            // Close current level if any
            closeLevel();
            
            // Create new level
            currentLevel = std::make_unique<Level>();
            
            // Load level from file
            if (!currentLevel->loadFromFile(filepath)) {
                SPARKY_LOG_ERROR("Failed to load level from: " + filepath);
                currentLevel.reset();
                return false;
            }
            
            // Clear undo/redo stacks
            clearUndoStack();
            
            // Notify callback
            if (onLevelChangeCallback) {
                onLevelChangeCallback();
            }
            
            SPARKY_LOG_INFO("Level loaded successfully from: " + filepath);
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Exception during level loading: " + std::string(e.what()));
            currentLevel.reset();
            return false;
        }
    }
    
    bool LevelEditor::saveLevel(const std::string& filepath) {
        SPARKY_LOG_INFO("Saving level to: " + filepath);
        
        if (!currentLevel) {
            SPARKY_LOG_WARNING("No level to save");
            return false;
        }
        
        try {
            // Save level to file
            if (!currentLevel->saveToFile(filepath)) {
                SPARKY_LOG_ERROR("Failed to save level to: " + filepath);
                return false;
            }
            
            SPARKY_LOG_INFO("Level saved successfully to: " + filepath);
            return true;
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Exception during level saving: " + std::string(e.what()));
            return false;
        }
    }
    
    void LevelEditor::newLevel() {
        SPARKY_LOG_INFO("Creating new level");
        
        // Close current level if any
        closeLevel();
        
        // Create new empty level
        currentLevel = std::make_unique<Level>();
        currentLevel->setName("New Level");
        currentLevel->setDescription("A new level created in the editor");
        
        // Clear undo/redo stacks
        clearUndoStack();
        
        // Notify callback
        if (onLevelChangeCallback) {
            onLevelChangeCallback();
        }
        
        SPARKY_LOG_INFO("New level created");
    }
    
    void LevelEditor::closeLevel() {
        SPARKY_LOG_INFO("Closing current level");
        
        if (currentLevel) {
            // In a full implementation, we might want to ask for save confirmation
            currentLevel.reset();
            selectedObjectIndex = -1;
            
            // Notify callback
            if (onLevelChangeCallback) {
                onLevelChangeCallback();
            }
        }
    }
    
    void LevelEditor::setMode(EditorMode mode) {
        currentMode = mode;
        SPARKY_LOG_DEBUG("Editor mode changed to: " + std::to_string(static_cast<int>(mode)));
    }
    
    void LevelEditor::setTool(EditorTool tool) {
        currentTool = tool;
        SPARKY_LOG_DEBUG("Editor tool changed to: " + std::to_string(static_cast<int>(tool)));
    }
    
    void LevelEditor::selectObject(size_t index) {
        if (currentLevel && index < currentLevel->getLevelObjects().size()) {
            selectedObjectIndex = static_cast<int>(index);
            
            // Notify callback
            if (onSelectionChangeCallback) {
                onSelectionChangeCallback();
            }
            
            SPARKY_LOG_DEBUG("Object selected: " + std::to_string(index));
        }
    }
    
    void LevelEditor::deselectObject() {
        selectedObjectIndex = -1;
        
        // Notify callback
        if (onSelectionChangeCallback) {
            onSelectionChangeCallback();
        }
        
        SPARKY_LOG_DEBUG("Object deselected");
    }
    
    void LevelEditor::moveSelectedObject(float x, float y, float z) {
        if (!currentLevel || selectedObjectIndex == -1) return;
        
        // Snap to grid if enabled
        if (gridSettings.enabled) {
            snapToGrid(x, y, z);
        }
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::MOVE_OBJECT;
        action.objectIndex = selectedObjectIndex;
        if (selectedObjectIndex < static_cast<int>(currentLevel->getLevelObjects().size())) {
            action.previousObjectData = currentLevel->getLevelObjects()[selectedObjectIndex];
        }
        
        // Move the object
        auto& objects = const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects());
        if (selectedObjectIndex < static_cast<int>(objects.size())) {
            objects[selectedObjectIndex].position[0] = x;
            objects[selectedObjectIndex].position[1] = y;
            objects[selectedObjectIndex].position[2] = z;
            
            action.objectData = objects[selectedObjectIndex];
            recordAction(action);
            
            // Notify callback
            if (onObjectModifyCallback) {
                onObjectModifyCallback();
            }
            
            SPARKY_LOG_DEBUG("Object moved to: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
        }
    }
    
    void LevelEditor::rotateSelectedObject(float x, float y, float z) {
        if (!currentLevel || selectedObjectIndex == -1) return;
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::ROTATE_OBJECT;
        action.objectIndex = selectedObjectIndex;
        if (selectedObjectIndex < static_cast<int>(currentLevel->getLevelObjects().size())) {
            action.previousObjectData = currentLevel->getLevelObjects()[selectedObjectIndex];
        }
        
        // Rotate the object
        auto& objects = const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects());
        if (selectedObjectIndex < static_cast<int>(objects.size())) {
            objects[selectedObjectIndex].rotation[0] = x;
            objects[selectedObjectIndex].rotation[1] = y;
            objects[selectedObjectIndex].rotation[2] = z;
            
            action.objectData = objects[selectedObjectIndex];
            recordAction(action);
            
            // Notify callback
            if (onObjectModifyCallback) {
                onObjectModifyCallback();
            }
            
            SPARKY_LOG_DEBUG("Object rotated to: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
        }
    }
    
    void LevelEditor::scaleSelectedObject(float x, float y, float z) {
        if (!currentLevel || selectedObjectIndex == -1) return;
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::SCALE_OBJECT;
        action.objectIndex = selectedObjectIndex;
        if (selectedObjectIndex < static_cast<int>(currentLevel->getLevelObjects().size())) {
            action.previousObjectData = currentLevel->getLevelObjects()[selectedObjectIndex];
        }
        
        // Scale the object
        auto& objects = const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects());
        if (selectedObjectIndex < static_cast<int>(objects.size())) {
            objects[selectedObjectIndex].scale[0] = x;
            objects[selectedObjectIndex].scale[1] = y;
            objects[selectedObjectIndex].scale[2] = z;
            
            action.objectData = objects[selectedObjectIndex];
            recordAction(action);
            
            // Notify callback
            if (onObjectModifyCallback) {
                onObjectModifyCallback();
            }
            
            SPARKY_LOG_DEBUG("Object scaled to: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
        }
    }
    
    void LevelEditor::createObject(const ObjectCreationParams& params) {
        if (!currentLevel) return;
        
        // Create level object from parameters
        LevelObject obj;
        obj.type = params.type;
        obj.name = params.name;
        obj.position[0] = params.position[0];
        obj.position[1] = params.position[1];
        obj.position[2] = params.position[2];
        obj.rotation[0] = params.rotation[0];
        obj.rotation[1] = params.rotation[1];
        obj.rotation[2] = params.rotation[2];
        obj.scale[0] = params.scale[0];
        obj.scale[1] = params.scale[1];
        obj.scale[2] = params.scale[2];
        obj.material = params.material;
        obj.mass = params.mass;
        obj.interactive = params.interactive;
        obj.interactionType = params.interactionType;
        obj.target = params.target;
        
        // Snap to grid if enabled
        if (gridSettings.enabled) {
            snapToGrid(obj.position[0], obj.position[1], obj.position[2]);
        }
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::CREATE_OBJECT;
        action.objectIndex = currentLevel->getLevelObjects().size();
        action.objectData = obj;
        recordAction(action);
        
        // Add object to level
        const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).push_back(obj);
        
        // Notify callback
        if (onObjectModifyCallback) {
            onObjectModifyCallback();
        }
        
        SPARKY_LOG_DEBUG("Object created: " + params.type + " (" + params.name + ")");
    }
    
    void LevelEditor::createInteractiveElement(const InteractiveCreationParams& params) {
        if (!currentLevel) return;
        
        // Create interactive element from parameters
        InteractiveElement element;
        element.type = params.type;
        element.name = params.name;
        element.position[0] = params.position[0];
        element.position[1] = params.position[1];
        element.position[2] = params.position[2];
        element.rotation[0] = params.rotation[0];
        element.rotation[1] = params.rotation[1];
        element.rotation[2] = params.rotation[2];
        element.targetObject = params.targetObject;
        element.state = params.state;
        
        // Snap to grid if enabled
        if (gridSettings.enabled) {
            snapToGrid(element.position[0], element.position[1], element.position[2]);
        }
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::CREATE_OBJECT; // Treat as object creation
        action.objectIndex = currentLevel->getInteractiveElements().size();
        // We would need to store the element data in the action
        recordAction(action);
        
        // Add element to level
        const_cast<std::vector<InteractiveElement>&>(currentLevel->getInteractiveElements()).push_back(element);
        
        // Notify callback
        if (onObjectModifyCallback) {
            onObjectModifyCallback();
        }
        
        SPARKY_LOG_DEBUG("Interactive element created: " + params.type + " (" + params.name + ")");
    }
    
    void LevelEditor::createTriggerVolume(const TriggerCreationParams& params) {
        if (!currentLevel) return;
        
        // Create trigger volume from parameters
        TriggerVolume volume;
        volume.name = params.name;
        volume.position[0] = params.position[0];
        volume.position[1] = params.position[1];
        volume.position[2] = params.position[2];
        volume.size[0] = params.size[0];
        volume.size[1] = params.size[1];
        volume.size[2] = params.size[2];
        volume.triggerType = params.triggerType;
        volume.target = params.target;
        
        // Snap to grid if enabled
        if (gridSettings.enabled) {
            snapToGrid(volume.position[0], volume.position[1], volume.position[2]);
        }
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::CREATE_OBJECT; // Treat as object creation
        action.objectIndex = currentLevel->getTriggerVolumes().size();
        // We would need to store the volume data in the action
        recordAction(action);
        
        // Add volume to level
        const_cast<std::vector<TriggerVolume>&>(currentLevel->getTriggerVolumes()).push_back(volume);
        
        // Notify callback
        if (onObjectModifyCallback) {
            onObjectModifyCallback();
        }
        
        SPARKY_LOG_DEBUG("Trigger volume created: " + params.name);
    }
    
    void LevelEditor::deleteSelectedObject() {
        if (!currentLevel || selectedObjectIndex == -1) return;
        
        deleteObject(selectedObjectIndex);
        selectedObjectIndex = -1;
    }
    
    void LevelEditor::deleteObject(size_t index) {
        if (!currentLevel || index >= currentLevel->getLevelObjects().size()) return;
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::DELETE_OBJECT;
        action.objectIndex = index;
        if (index < currentLevel->getLevelObjects().size()) {
            action.objectData = currentLevel->getLevelObjects()[index];
        }
        recordAction(action);
        
        // Remove object from level
        auto& objects = const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects());
        objects.erase(objects.begin() + index);
        
        // Adjust selection
        if (selectedObjectIndex >= static_cast<int>(index)) {
            selectedObjectIndex = -1;
        }
        
        // Notify callback
        if (onObjectModifyCallback) {
            onObjectModifyCallback();
        }
        
        SPARKY_LOG_DEBUG("Object deleted at index: " + std::to_string(index));
    }
    
    void LevelEditor::setGridSettings(const GridSettings& settings) {
        gridSettings = settings;
        SPARKY_LOG_DEBUG("Grid settings updated");
    }
    
    void LevelEditor::setCameraSettings(const EditorCameraSettings& settings) {
        cameraSettings = settings;
        SPARKY_LOG_DEBUG("Camera settings updated");
    }
    
    void LevelEditor::undo() {
        if (undoStack.empty() || !currentLevel) return;
        
        // Get the last action
        EditorAction action = undoStack.back();
        undoStack.pop_back();
        
        // Perform inverse action
        switch (action.type) {
            case EditorAction::CREATE_OBJECT:
                // Remove the created object
                if (action.objectIndex < currentLevel->getLevelObjects().size()) {
                    const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).erase(
                        const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).begin() + action.objectIndex);
                }
                break;
                
            case EditorAction::DELETE_OBJECT:
                // Restore the deleted object
                const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).insert(
                    const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).begin() + action.objectIndex,
                    action.objectData);
                break;
                
            case EditorAction::MODIFY_OBJECT:
            case EditorAction::MOVE_OBJECT:
            case EditorAction::ROTATE_OBJECT:
            case EditorAction::SCALE_OBJECT:
                // Restore previous object state
                if (action.objectIndex < currentLevel->getLevelObjects().size()) {
                    const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects())[action.objectIndex] = action.previousObjectData;
                }
                break;
        }
        
        // Add to redo stack
        redoStack.push_back(action);
        
        // Limit redo stack size
        if (redoStack.size() > maxUndoSteps) {
            redoStack.erase(redoStack.begin());
        }
        
        // Notify callback
        if (onObjectModifyCallback) {
            onObjectModifyCallback();
        }
        
        SPARKY_LOG_DEBUG("Undo performed");
    }
    
    void LevelEditor::redo() {
        if (redoStack.empty() || !currentLevel) return;
        
        // Get the last redo action
        EditorAction action = redoStack.back();
        redoStack.pop_back();
        
        // Perform action
        switch (action.type) {
            case EditorAction::CREATE_OBJECT:
                // Recreate the object
                const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).insert(
                    const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).begin() + action.objectIndex,
                    action.objectData);
                break;
                
            case EditorAction::DELETE_OBJECT:
                // Delete the object again
                if (action.objectIndex < currentLevel->getLevelObjects().size()) {
                    const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).erase(
                        const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).begin() + action.objectIndex);
                }
                break;
                
            case EditorAction::MODIFY_OBJECT:
            case EditorAction::MOVE_OBJECT:
            case EditorAction::ROTATE_OBJECT:
            case EditorAction::SCALE_OBJECT:
                // Apply object state
                if (action.objectIndex < currentLevel->getLevelObjects().size()) {
                    const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects())[action.objectIndex] = action.objectData;
                }
                break;
        }
        
        // Add to undo stack
        undoStack.push_back(action);
        
        // Limit undo stack size
        if (undoStack.size() > maxUndoSteps) {
            undoStack.erase(undoStack.begin());
        }
        
        // Notify callback
        if (onObjectModifyCallback) {
            onObjectModifyCallback();
        }
        
        SPARKY_LOG_DEBUG("Redo performed");
    }
    
    bool LevelEditor::canUndo() const {
        return !undoStack.empty();
    }
    
    bool LevelEditor::canRedo() const {
        return !redoStack.empty();
    }
    
    void LevelEditor::snapToGrid(float& x, float& y, float& z) {
        if (!gridSettings.enabled) return;
        
        float gridSize = gridSettings.size;
        x = round(x / gridSize) * gridSize;
        y = round(y / gridSize) * gridSize;
        z = round(z / gridSize) * gridSize;
    }
    
    void LevelEditor::duplicateSelectedObject() {
        if (!currentLevel || selectedObjectIndex == -1) return;
        
        if (selectedObjectIndex < static_cast<int>(currentLevel->getLevelObjects().size())) {
            // Get the selected object
            LevelObject original = currentLevel->getLevelObjects()[selectedObjectIndex];
            
            // Modify position to avoid exact overlap
            original.position[0] += gridSettings.size;
            original.position[1] += gridSettings.size;
            original.position[2] += gridSettings.size;
            
            // Record action for undo
            EditorAction action;
            action.type = EditorAction::CREATE_OBJECT;
            action.objectIndex = currentLevel->getLevelObjects().size();
            action.objectData = original;
            recordAction(action);
            
            // Add duplicated object to level
            const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).push_back(original);
            
            // Select the new object
            selectedObjectIndex = static_cast<int>(currentLevel->getLevelObjects().size()) - 1;
            
            // Notify callback
            if (onObjectModifyCallback) {
                onObjectModifyCallback();
            }
            
            SPARKY_LOG_DEBUG("Object duplicated");
        }
    }
    
    void LevelEditor::copySelectedObject() {
        if (!currentLevel || selectedObjectIndex == -1) return;
        
        if (selectedObjectIndex < static_cast<int>(currentLevel->getLevelObjects().size())) {
            clipboardObject = currentLevel->getLevelObjects()[selectedObjectIndex];
            clipboardHasData = true;
            SPARKY_LOG_DEBUG("Object copied to clipboard");
        }
    }
    
    void LevelEditor::pasteObject() {
        if (!currentLevel || !clipboardHasData) return;
        
        // Modify position to avoid exact overlap
        clipboardObject.position[0] += gridSettings.size;
        clipboardObject.position[1] += gridSettings.size;
        clipboardObject.position[2] += gridSettings.size;
        
        // Record action for undo
        EditorAction action;
        action.type = EditorAction::CREATE_OBJECT;
        action.objectIndex = currentLevel->getLevelObjects().size();
        action.objectData = clipboardObject;
        recordAction(action);
        
        // Add pasted object to level
        const_cast<std::vector<LevelObject>&>(currentLevel->getLevelObjects()).push_back(clipboardObject);
        
        // Select the new object
        selectedObjectIndex = static_cast<int>(currentLevel->getLevelObjects().size()) - 1;
        
        // Notify callback
        if (onObjectModifyCallback) {
            onObjectModifyCallback();
        }
        
        SPARKY_LOG_DEBUG("Object pasted from clipboard");
    }
    
    void LevelEditor::enableGizmo(bool enable) {
        gizmoEnabled = enable;
        SPARKY_LOG_DEBUG("Gizmo " + std::string(enable ? "enabled" : "disabled"));
    }
    
    void LevelEditor::rebuildSceneHierarchy() {
        // In a full implementation, we would rebuild the scene hierarchy
        SPARKY_LOG_DEBUG("Scene hierarchy rebuilt");
    }
    
    std::vector<std::string> LevelEditor::getAvailableObjectTypes() const {
        // Return a list of available object types
        return {"Cube", "Sphere", "Cylinder", "Plane", "PlayerStart", "EnemySpawn", "Pickup", "Door", "Button"};
    }
    
    std::vector<std::string> LevelEditor::getAvailableMaterials() const {
        // Return a list of available materials
        return {"Default", "Metal", "Wood", "Concrete", "Glass", "Water"};
    }
    
    std::vector<std::string> LevelEditor::getAvailableInteractionTypes() const {
        // Return a list of available interaction types
        return {"Door", "Button", "Switch", "Lever", "PressurePlate"};
    }
    
    void LevelEditor::setViewportSize(int width, int height) {
        viewportWidth = width;
        viewportHeight = height;
        SPARKY_LOG_DEBUG("Viewport size set to: " + std::to_string(width) + "x" + std::to_string(height));
    }
    
    void LevelEditor::getViewportSize(int& width, int& height) const {
        width = viewportWidth;
        height = viewportHeight;
    }
    
    void LevelEditor::setAutoSave(bool autoSave) {
        autoSaveEnabled = autoSave;
        SPARKY_LOG_DEBUG("Auto-save " + std::string(autoSave ? "enabled" : "disabled"));
    }
    
    size_t LevelEditor::getObjectCount() const {
        return currentLevel ? currentLevel->getLevelObjects().size() : 0;
    }
    
    size_t LevelEditor::getInteractiveElementCount() const {
        return currentLevel ? currentLevel->getInteractiveElements().size() : 0;
    }
    
    size_t LevelEditor::getTriggerVolumeCount() const {
        return currentLevel ? currentLevel->getTriggerVolumes().size() : 0;
    }
    
    // Private helper methods
    void LevelEditor::handleInput(float deltaTime) {
        // In a full implementation, we would handle user input
        // For now, we'll just log that we're handling input
        SPARKY_LOG_DEBUG("Handling editor input");
    }
    
    void LevelEditor::updateCamera(float deltaTime) {
        // In a full implementation, we would update the editor camera
        // For now, we'll just log that we're updating the camera
        SPARKY_LOG_DEBUG("Updating editor camera");
    }
    
    void LevelEditor::renderGrid() {
        if (!gridSettings.showGrid) return;
        
        // In a full implementation, we would render the grid
        SPARKY_LOG_DEBUG("Rendering grid");
    }
    
    void LevelEditor::renderGizmo() {
        // In a full implementation, we would render the gizmo
        SPARKY_LOG_DEBUG("Rendering gizmo");
    }
    
    void LevelEditor::renderSceneHierarchy() {
        // In a full implementation, we would render the scene hierarchy panel
        SPARKY_LOG_DEBUG("Rendering scene hierarchy");
    }
    
    void LevelEditor::renderPropertiesPanel() {
        // In a full implementation, we would render the properties panel
        SPARKY_LOG_DEBUG("Rendering properties panel");
    }
    
    void LevelEditor::renderAssetBrowser() {
        // In a full implementation, we would render the asset browser
        SPARKY_LOG_DEBUG("Rendering asset browser");
    }
    
    void LevelEditor::renderMenuBar() {
        // In a full implementation, we would render the menu bar
        SPARKY_LOG_DEBUG("Rendering menu bar");
    }
    
    void LevelEditor::renderToolbar() {
        // In a full implementation, we would render the toolbar
        SPARKY_LOG_DEBUG("Rendering toolbar");
    }
    
    void LevelEditor::renderStatusBar() {
        // In a full implementation, we would render the status bar
        SPARKY_LOG_DEBUG("Rendering status bar");
    }
    
    void LevelEditor::recordAction(const EditorAction& action) {
        // Add action to undo stack
        undoStack.push_back(action);
        
        // Limit undo stack size
        if (undoStack.size() > maxUndoSteps) {
            undoStack.erase(undoStack.begin());
        }
        
        // Clear redo stack when new action is recorded
        redoStack.clear();
    }
    
    void LevelEditor::clearUndoStack() {
        undoStack.clear();
        redoStack.clear();
    }
    
    void LevelEditor::checkAutoSave(float deltaTime) {
        if (!autoSaveEnabled || !currentLevel) return;
        
        lastAutoSaveTime += deltaTime;
        if (lastAutoSaveTime >= autoSaveInterval) {
            // Perform auto-save
            saveLevel("autosave.level");
            lastAutoSaveTime = 0.0f;
            SPARKY_LOG_INFO("Auto-save performed");
        }
    }
    
    bool LevelEditor::validateLevel() const {
        if (!currentLevel) return false;
        
        // In a full implementation, we would validate the level
        // For now, we'll just return true
        return true;
    }
}