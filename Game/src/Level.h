#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Sparky {
    class GameObject;
    class Player;
    class InteractiveObject;
    class Pickup;
    
    struct LevelObject {
        std::string type;
        float position[3]; // x, y, z
        float rotation[3]; // x, y, z
        float scale[3];    // x, y, z
        std::string name;
        std::string material; // Material name for rendering
        float mass; // Physics mass (0.0 for static objects)
        bool interactive; // Whether the object is interactive
        std::string interactionType; // Type of interaction (door, button, etc.)
        std::string target; // Target object for interaction
    };
    
    // Struct for interactive elements
    struct InteractiveElement {
        std::string type; // door, button, switch, etc.
        std::string name;
        float position[3]; // x, y, z
        float rotation[3]; // x, y, z
        std::string targetObject; // Object that this element affects
        std::string state; // open/closed, pressed/released, etc.
        std::vector<std::string> connectedElements; // Other elements this connects to
    };
    
    // Struct for triggers/volumes
    struct TriggerVolume {
        std::string name;
        float position[3]; // x, y, z
        float size[3];     // width, height, depth
        std::string triggerType; // enter, exit, stay
        std::string target; // Object or script to trigger
        std::vector<std::string> conditions; // Conditions for trigger activation
    };
    
    class Level {
    public:
        Level();
        ~Level();
        
        bool loadFromFile(const std::string& filepath);
        bool saveToFile(const std::string& filepath) const;
        
        void addLevelObject(const LevelObject& obj);
        void removeLevelObject(size_t index);
        const std::vector<LevelObject>& getLevelObjects() const { return levelObjects; }
        
        void addInteractiveElement(const InteractiveElement& element);
        void removeInteractiveElement(size_t index);
        const std::vector<InteractiveElement>& getInteractiveElements() const { return interactiveElements; }
        
        void addTriggerVolume(const TriggerVolume& volume);
        void removeTriggerVolume(size_t index);
        const std::vector<TriggerVolume>& getTriggerVolumes() const { return triggerVolumes; }
        
        void spawnObjects();
        
        const std::string& getName() const { return name; }
        void setName(const std::string& levelName) { name = levelName; }
        
        const std::string& getDescription() const { return description; }
        void setDescription(const std::string& levelDescription) { description = levelDescription; }
        
        // Get spawned game objects
        const std::vector<std::unique_ptr<GameObject>>& getSpawnedObjects() const { return spawnedObjects; }
        
        // Additional methods for level management
        GameObject* findObjectByName(const std::string& name);
        std::vector<GameObject*> findObjectsByType(const std::string& type);
        void addSpawnedObject(std::unique_ptr<GameObject> object);
        void removeSpawnedObject(GameObject* object);

    private:
        std::string name;
        std::string description;
        std::vector<LevelObject> levelObjects;
        std::vector<InteractiveElement> interactiveElements;
        std::vector<TriggerVolume> triggerVolumes;
        std::vector<std::unique_ptr<GameObject>> spawnedObjects;
        
        // Private helper methods
        std::unique_ptr<GameObject> createObject(const LevelObject& obj) const;
        std::unique_ptr<InteractiveObject> createInteractiveElement(const InteractiveElement& element) const;
        std::unique_ptr<Pickup> createPickup(const LevelObject& obj) const;
        void parseLevelFile(const std::string& content);
        std::string serializeLevel() const;
    };
}