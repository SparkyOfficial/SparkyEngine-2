#pragma once

#include <string>
#include <vector>

namespace Sparky {
    class Texture {
    public:
        Texture();
        ~Texture();

        bool loadFromFile(const std::string& filepath);
        void unload();

        // Getters
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        int getChannels() const { return channels; }
        const std::vector<unsigned char>& getData() const { return data; }

    private:
        int width;
        int height;
        int channels;
        std::vector<unsigned char> data;
    };
}