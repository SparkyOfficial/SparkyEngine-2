#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"
#include "../include/Texture.h"
#include "../include/Logger.h"
#include <iostream>

namespace Sparky {

    Texture::Texture() : width(0), height(0), channels(0) {
    }

    Texture::~Texture() {
        unload();
    }

    bool Texture::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Loading texture from file: " + filepath);
        
        // Use stb_image to load various image formats
        stbi_set_flip_vertically_on_load(true);
        unsigned char* imageData = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        
        if (imageData == nullptr) {
            SPARKY_LOG_ERROR("Failed to load texture: " + filepath + " - " + stbi_failure_reason());
            
            // Generate a simple placeholder texture (checkerboard pattern) as fallback
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
        
        // Copy image data to our container
        int imageSize = width * height * channels;
        data.resize(imageSize);
        std::memcpy(data.data(), imageData, imageSize);
        
        // Free STB image data
        stbi_image_free(imageData);
        
        SPARKY_LOG_DEBUG("Loaded texture: " + std::to_string(width) + "x" + std::to_string(height) + 
                        " with " + std::to_string(channels) + " channels");
        return true;
    }

    void Texture::unload() {
        data.clear();
        width = 0;
        height = 0;
        channels = 0;
    }
}