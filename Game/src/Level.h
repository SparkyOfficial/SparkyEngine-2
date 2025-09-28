#pragma once

#include "../../Engine/include/GameObject.h"
#include <vector>
#include <string>

namespace Sparky {
    class Platform;
    class Gun;

    class Level : public GameObject {
    public:
        Level(const std::string& name);
        ~Level();

        void update(float deltaTime) override;
        void render() override;

        // Level management
        void loadLevel();
        void loadLevelFromFile(const std::string& filename);
        void unloadLevel();

        // Add entities to the level
        void addPlatform(Platform* platform);
        void addGun(Gun* gun);

        // Getters
        const std::vector<Platform*>& getPlatforms() const { return platforms; }
        const std::vector<Gun*>& getGuns() const { return guns; }

    private:
        std::vector<Platform*> platforms;
        std::vector<Gun*> guns;
        
        // Level geometry
        void createTestLevel();
    };
}