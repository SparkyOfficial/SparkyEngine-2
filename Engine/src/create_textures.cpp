#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

// Simple function to write a PPM image file (PPM is a simple image format)
void writePPM(const std::string& filename, int width, int height, const std::vector<uint8_t>& pixels) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return;
    }
    
    // Write PPM header
    file << "P6\n" << width << " " << height << "\n255\n";
    
    // Write pixel data
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    
    file.close();
    std::cout << "Created texture: " << filename << std::endl;
}

int main() {
    // Create textures in the current directory
    std::cout << "Creating textures in current directory..." << std::endl;
    
    // Create a metal texture (gray with some variation)
    {
        const int width = 64, height = 64;
        std::vector<uint8_t> pixels(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                
                // Create a metallic pattern with some noise
                uint8_t base = 100;
                uint8_t variation = (x * y) % 50;
                
                pixels[index] = base + variation;     // R
                pixels[index + 1] = base + variation; // G
                pixels[index + 2] = base + variation; // B
            }
        }
        
        writePPM("metal.ppm", width, height, pixels);
    }
    
    // Create a concrete texture (gray with more variation)
    {
        const int width = 64, height = 64;
        std::vector<uint8_t> pixels(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                
                // Create a concrete pattern with more variation
                uint8_t base = 80;
                uint8_t variation = ((x * 13 + y * 7) % 80);
                
                pixels[index] = base + variation;     // R
                pixels[index + 1] = base + variation; // G
                pixels[index + 2] = base + variation; // B
            }
        }
        
        writePPM("concrete.ppm", width, height, pixels);
    }
    
    // Create a grass texture (green)
    {
        const int width = 64, height = 64;
        std::vector<uint8_t> pixels(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                
                // Create a grass pattern
                uint8_t baseGreen = 50;
                uint8_t variation = ((x * 3 + y * 5) % 100);
                
                pixels[index] = 30 + (variation / 3);          // R
                pixels[index + 1] = baseGreen + variation;     // G
                pixels[index + 2] = 30 + (variation / 4);      // B
            }
        }
        
        writePPM("grass.ppm", width, height, pixels);
    }
    
    // Create a wood texture (brown)
    {
        const int width = 64, height = 64;
        std::vector<uint8_t> pixels(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                
                // Create a wood pattern
                uint8_t base = 100;
                uint8_t variation = ((x * 2 + y) % 50);
                
                pixels[index] = base + variation;              // R
                pixels[index + 1] = base/2 + variation/2;      // G
                pixels[index + 2] = base/4 + variation/4;      // B
            }
        }
        
        writePPM("wood.ppm", width, height, pixels);
    }
    
    // Create a brick texture (red)
    {
        const int width = 64, height = 64;
        std::vector<uint8_t> pixels(width * height * 3);
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 3;
                
                // Create a brick pattern
                bool isBrick = ((x / 8) + (y / 16)) % 2 == 0;
                
                if (isBrick) {
                    pixels[index] = 180;     // R
                    pixels[index + 1] = 80;  // G
                    pixels[index + 2] = 60;  // B
                } else {
                    pixels[index] = 100;     // R
                    pixels[index + 1] = 100; // G
                    pixels[index + 2] = 100; // B (mortar)
                }
            }
        }
        
        writePPM("brick.ppm", width, height, pixels);
    }
    
    std::cout << "All textures created successfully!" << std::endl;
    return 0;
}