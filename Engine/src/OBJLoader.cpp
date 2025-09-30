#include "../include/OBJLoader.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace Sparky {
    
    std::unique_ptr<Mesh> OBJLoader::loadFromFile(const std::string& filepath) {
        auto mesh = std::make_unique<Mesh>();
        
        if (!parseOBJ(filepath, mesh->vertices, mesh->indices)) {
            SPARKY_LOG_ERROR("Failed to parse OBJ file: " + filepath);
            return nullptr;
        }
        
        SPARKY_LOG_INFO("Loaded OBJ file: " + filepath + 
                       " with " + std::to_string(mesh->vertices.size()) + " vertices and " + 
                       std::to_string(mesh->indices.size()) + " indices");
        
        return mesh;
    }
    
    bool OBJLoader::parseOBJ(const std::string& filepath, 
                           std::vector<Vertex>& vertices, 
                           std::vector<uint32_t>& indices) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            SPARKY_LOG_ERROR("Could not open OBJ file: " + filepath);
            return false;
        }
        
        // Temporary storage for raw data
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        
        // Face data (indices into the above arrays)
        struct FaceIndex {
            int posIndex;
            int texIndex;
            int normIndex;
        };
        
        std::vector<FaceIndex> faceIndices;
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                continue; // Skip empty lines and comments
            }
            
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            
            if (prefix == "v") {
                // Vertex position
                glm::vec3 pos;
                iss >> pos.x >> pos.y >> pos.z;
                positions.push_back(pos);
            }
            else if (prefix == "vn") {
                // Vertex normal
                glm::vec3 norm;
                iss >> norm.x >> norm.y >> norm.z;
                normals.push_back(norm);
            }
            else if (prefix == "vt") {
                // Texture coordinate
                glm::vec2 tex;
                iss >> tex.x >> tex.y;
                texCoords.push_back(tex);
            }
            else if (prefix == "f") {
                // Face definition
                std::string faceData;
                std::getline(iss, faceData);
                
                // Split by spaces
                auto parts = splitString(faceData, ' ');
                
                // Process each vertex in the face
                for (const auto& part : parts) {
                    if (part.empty()) continue;
                    
                    FaceIndex faceIndex = {0, 0, 0};
                    
                    // Parse vertex data (format: v/vt/vn or v//vn or v)
                    auto indicesStr = splitString(part, '/');
                    
                    if (indicesStr.size() >= 1 && !indicesStr[0].empty()) {
                        faceIndex.posIndex = std::stoi(indicesStr[0]) - 1; // OBJ indices are 1-based
                    }
                    
                    if (indicesStr.size() >= 2 && !indicesStr[1].empty()) {
                        faceIndex.texIndex = std::stoi(indicesStr[1]) - 1;
                    }
                    
                    if (indicesStr.size() >= 3 && !indicesStr[2].empty()) {
                        faceIndex.normIndex = std::stoi(indicesStr[2]) - 1;
                    }
                    
                    faceIndices.push_back(faceIndex);
                }
            }
        }
        
        file.close();
        
        // Convert face indices to actual vertices
        // For simplicity, we'll create a vertex for each face vertex
        // A more advanced implementation would reuse vertices
        vertices.reserve(faceIndices.size());
        indices.reserve(faceIndices.size());
        
        for (size_t i = 0; i < faceIndices.size(); ++i) {
            const auto& faceIndex = faceIndices[i];
            
            Vertex vertex;
            
            // Get position
            if (faceIndex.posIndex >= 0 && faceIndex.posIndex < static_cast<int>(positions.size())) {
                vertex.position = positions[faceIndex.posIndex];
            }
            
            // Get normal
            if (faceIndex.normIndex >= 0 && faceIndex.normIndex < static_cast<int>(normals.size())) {
                vertex.normal = normals[faceIndex.normIndex];
            } else {
                vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default normal
            }
            
            // Get texture coordinate
            if (faceIndex.texIndex >= 0 && faceIndex.texIndex < static_cast<int>(texCoords.size())) {
                vertex.texCoord = texCoords[faceIndex.texIndex];
            } else {
                vertex.texCoord = glm::vec2(0.0f, 0.0f); // Default texture coordinate
            }
            
            vertices.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(i));
        }
        
        return true;
    }
    
    std::vector<std::string> OBJLoader::splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
}