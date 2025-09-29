#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace Sparky {
    class GameObject;
    class Player;
    
    struct LevelObject {
        std::string type;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        std::string name;
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
        std::vector<std::unique_ptr<GameObject>> spawnedObjects;
        
        // Private helper methods
        std::unique_ptr<GameObject> createObject(const LevelObject& obj) const;
        void parseLevelFile(const std::string& content);
        std::string serializeLevel() const;
    };
}