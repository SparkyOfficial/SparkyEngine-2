#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Mesh.h"

namespace Sparky {
    
    class OBJLoader {
    public:
        static std::unique_ptr<Mesh> loadFromFile(const std::string& filepath);
        
    private:
        static bool parseOBJ(const std::string& filepath, 
                           std::vector<Vertex>& vertices, 
                           std::vector<uint32_t>& indices);
        
        static std::vector<std::string> splitString(const std::string& str, char delimiter);
        static Vertex parseVertex(const std::string& line);
        static glm::vec3 parseVec3(const std::string& line);
        static glm::vec2 parseVec2(const std::string& line);
    };
}