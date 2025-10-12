/*
 * LevelEditor.h
 * 
 * Author: Андрій Будильников
 * 
 * Simple level editor for creating and editing game levels
 */

#pragma once

#include "Level.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace Sparky {
    
    // Editor modes
    enum class EditorMode {
        SELECT,
        MOVE,
        ROTATE,
        SCALE,
        CREATE_OBJECT,
        CREATE_INTERACTIVE,
        CREATE_TRIGGER,
        DELETE
    };
    
    // Editor tool
    enum class EditorTool {
        TRANSLATE,
        ROTATE,
        SCALE,
        SELECT
    };
    
    // Grid settings
    struct GridSettings {
        bool enabled;
        float size;
        float snapThreshold;
        bool showGrid;
        
        GridSettings() : enabled(true), size(1.0f), snapThreshold(0.1f), showGrid(true) {}
    };
    
    // Editor camera settings
    struct EditorCameraSettings {
        float moveSpeed;
        float rotationSpeed;
        float zoomSpeed;
        bool invertY;
        
        EditorCameraSettings() : moveSpeed(5.0f), rotationSpeed(0.1f), zoomSpeed(1.0f), invertY(false) {}
    };
    
    // Object creation parameters
    struct ObjectCreationParams {
        std::string type;
        std::string name;
        float position[3];
        float rotation[3];
        float scale[3];
        std::string material;
        float mass;
        bool interactive;
        std::string interactionType;
        std::string target;
        
        ObjectCreationParams() : position{0.0f, 0.0f, 0.0f}, rotation{0.0f, 0.0f, 0.0f}, scale{1.0f, 1.0f, 1.0f},
                                mass(0.0f), interactive(false) {}
    };
    
    // Interactive element creation parameters
    struct InteractiveCreationParams {
        std::string type;
        std::string name;
        float position[3];
        float rotation[3];
        std::string targetObject;
        std::string state;
        
        InteractiveCreationParams() : position{0.0f, 0.0f, 0.0f}, rotation{0.0f, 0.0f, 0.0f} {}
    };
    
    // Trigger volume creation parameters
    struct TriggerCreationParams {
        std::string name;
        float position[3];
        float size[3];
        std::string triggerType;
        std::string target;
        
        TriggerCreationParams() : position{0.0f, 0.0f, 0.0f}, size{1.0f, 1.0f, 1.0f} {}
    };
    
    class LevelEditor {
    public:
        LevelEditor();
        ~LevelEditor();
        
        // Initialization and cleanup
        bool initialize();
        void cleanup();
        
        // Update and render
        void update(float deltaTime);
        void render();
        
        // Level management
        bool loadLevel(const std::string& filepath);
        bool saveLevel(const std::string& filepath);
        void newLevel();
        void closeLevel();
        
        // Editor mode and tool management
        void setMode(EditorMode mode);
        EditorMode getMode() const { return currentMode; }
        
        void setTool(EditorTool tool);
        EditorTool getTool() const { return currentTool; }
        
        // Selection management
        void selectObject(size_t index);
        void deselectObject();
        bool isObjectSelected() const { return selectedObjectIndex != -1; }
        size_t getSelectedObjectIndex() const { return selectedObjectIndex; }
        
        // Object manipulation
        void moveSelectedObject(float x, float y, float z);
        void rotateSelectedObject(float x, float y, float z);
        void scaleSelectedObject(float x, float y, float z);
        
        // Object creation
        void createObject(const ObjectCreationParams& params);
        void createInteractiveElement(const InteractiveCreationParams& params);
        void createTriggerVolume(const TriggerCreationParams& params);
        
        // Object deletion
        void deleteSelectedObject();
        void deleteObject(size_t index);
        
        // Grid settings
        void setGridSettings(const GridSettings& settings);
        const GridSettings& getGridSettings() const { return gridSettings; }
        
        // Camera settings
        void setCameraSettings(const EditorCameraSettings& settings);
        const EditorCameraSettings& getCameraSettings() const { return cameraSettings; }
        
        // Get current level
        Level* getCurrentLevel() { return currentLevel.get(); }
        const Level* getCurrentLevel() const { return currentLevel.get(); }
        
        // Editor state
        bool isEditing() const { return editing; }
        void setEditing(bool edit) { editing = edit; }
        
        // Undo/Redo
        void undo();
        void redo();
        bool canUndo() const;
        bool canRedo() const;
        
        // Callbacks
        void setOnLevelChangeCallback(std::function<void()> callback) { onLevelChangeCallback = callback; }
        void setOnSelectionChangeCallback(std::function<void()> callback) { onSelectionChangeCallback = callback; }
        void setOnObjectModifyCallback(std::function<void()> callback) { onObjectModifyCallback = callback; }
        
        // Utility functions
        void snapToGrid(float& x, float& y, float& z);
        void duplicateSelectedObject();
        void copySelectedObject();
        void pasteObject();
        
        // Gizmo manipulation (for visual editing)
        void enableGizmo(bool enable);
        bool isGizmoEnabled() const { return gizmoEnabled; }
        
        // Scene hierarchy
        void rebuildSceneHierarchy();
        
        // Asset browser
        std::vector<std::string> getAvailableObjectTypes() const;
        std::vector<std::string> getAvailableMaterials() const;
        std::vector<std::string> getAvailableInteractionTypes() const;
        
        // Viewport management
        void setViewportSize(int width, int height);
        void getViewportSize(int& width, int& height) const;
        
        // Editor preferences
        void setAutoSave(bool autoSave);
        bool getAutoSave() const { return autoSaveEnabled; }
        
        // Scene statistics
        size_t getObjectCount() const;
        size_t getInteractiveElementCount() const;
        size_t getTriggerVolumeCount() const;

    private:
        // Current level
        std::unique_ptr<Level> currentLevel;
        
        // Editor state
        EditorMode currentMode;
        EditorTool currentTool;
        bool editing;
        int selectedObjectIndex;
        bool gizmoEnabled;
        
        // Settings
        GridSettings gridSettings;
        EditorCameraSettings cameraSettings;
        
        // Viewport
        int viewportWidth;
        int viewportHeight;
        
        // Undo/Redo system
        struct EditorAction {
            enum Type {
                CREATE_OBJECT,
                DELETE_OBJECT,
                MODIFY_OBJECT,
                MOVE_OBJECT,
                ROTATE_OBJECT,
                SCALE_OBJECT
            };
            
            Type type;
            size_t objectIndex;
            LevelObject objectData;
            LevelObject previousObjectData;
        };
        
        std::vector<EditorAction> undoStack;
        std::vector<EditorAction> redoStack;
        size_t maxUndoSteps;
        
        // Clipboard for copy/paste
        LevelObject clipboardObject;
        bool clipboardHasData;
        
        // Preferences
        bool autoSaveEnabled;
        float autoSaveInterval;
        float lastAutoSaveTime;
        
        // Callbacks
        std::function<void()> onLevelChangeCallback;
        std::function<void()> onSelectionChangeCallback;
        std::function<void()> onObjectModifyCallback;
        
        // Private helper methods
        void handleInput(float deltaTime);
        void updateCamera(float deltaTime);
        void renderGrid();
        void renderGizmo();
        void renderSceneHierarchy();
        void renderPropertiesPanel();
        void renderAssetBrowser();
        void renderMenuBar();
        void renderToolbar();
        void renderStatusBar();
        
        // Action recording for undo/redo
        void recordAction(const EditorAction& action);
        void clearUndoStack();
        
        // Auto-save
        void checkAutoSave(float deltaTime);
        
        // Scene validation
        bool validateLevel() const;
    };
}