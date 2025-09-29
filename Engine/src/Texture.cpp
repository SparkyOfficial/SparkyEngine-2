#include "../include/Texture.h"
#include "../include/Logger.h"
#include <fstream>
#include <iostream>
#include <vector>

namespace Sparky {

    Texture::Texture() : width(0), height(0), channels(0) {
    }

    Texture::~Texture() {
        unload();
    }

    bool Texture::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Loading texture from file: " + filepath);
        
        try {
            // Use a library like stb_image to load various image formats
            // For now, we'll create a simple implementation that generates a placeholder texture
            // or loads raw RGB data if the file is in a simple format
            
            // Check if this is a raw RGB file (for demonstration purposes)
            if (filepath.substr(filepath.find_last_of(".") + 1) == "rgb") {
                return loadImageData(filepath);
            } else {
                // Generate a simple placeholder texture (checkerboard pattern)
                width = 64;
                height = 64;
                channels = 3; // RGB
                
                data.resize(width * height * channels);
                
                // Generate checkerboard pattern
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        int index = (y * width + x) * channels;
                        bool isWhite = ((x / 8) + (y / 8)) % 2 == 0;
                        
                        data[index] = isWhite ? 255 : 128;     // R
                        data[index + 1] = isWhite ? 255 : 128; // G
                        data[index + 2] = isWhite ? 255 : 128; // B
                    }
                }
                
                SPARKY_LOG_DEBUG("Generated placeholder texture: " + std::to_string(width) + "x" + std::to_string(height));
                return true;
            }
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load texture: " + filepath + " - " + std::string(e.what()));
            return false;
        }
    }

    bool Texture::loadImageData(const std::string& filepath) {
        // Load raw RGB image data
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) {
            SPARKY_LOG_ERROR("Failed to open texture file: " + filepath);
            return false;
        }
        
        // For simplicity, assume the file contains width, height, and then raw RGB data
        file.read(reinterpret_cast<char*>(&width), sizeof(int));
        file.read(reinterpret_cast<char*>(&height), sizeof(int));
        channels = 3; // Assume RGB
        
        if (file.fail()) {
            SPARKY_LOG_ERROR("Failed to read texture dimensions from: " + filepath);
            return false;
        }
        
        data.resize(width * height * channels);
        file.read(reinterpret_cast<char*>(data.data()), data.size());
        
        if (file.fail()) {
            SPARKY_LOG_ERROR("Failed to read texture data from: " + filepath);
            return false;
        }
        
        file.close();
        SPARKY_LOG_DEBUG("Loaded raw RGB texture: " + std::to_string(width) + "x" + std::to_string(height));
        return true;
    }

    void Texture::unload() {
        data.clear();
        width = 0;
        height = 0;
        channels = 0;
    }
}