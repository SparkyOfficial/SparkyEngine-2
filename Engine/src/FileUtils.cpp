#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Sparky {

    bool FileUtils::fileExists(const std::string& filepath) {
        std::ifstream file(filepath);
        return file.good();
    }

    std::vector<char> FileUtils::readFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Reading file: " + filepath);
        
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            SPARKY_LOG_ERROR("Failed to open file: " + filepath);
            throw std::runtime_error("failed to open file: " + filepath);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        SPARKY_LOG_DEBUG("File read successfully, size: " + std::to_string(fileSize) + " bytes");
        return buffer;
    }

    bool FileUtils::writeFile(const std::string& filepath, const std::vector<char>& data) {
        SPARKY_LOG_DEBUG("Writing file: " + filepath);
        
        std::ofstream file(filepath, std::ios::binary);

        if (!file.is_open()) {
            SPARKY_LOG_ERROR("Failed to open file for writing: " + filepath);
            return false;
        }

        file.write(data.data(), data.size());
        file.close();

        SPARKY_LOG_DEBUG("File written successfully, size: " + std::to_string(data.size()) + " bytes");
        return true;
    }

    std::string FileUtils::getFileName(const std::string& filepath) {
        size_t lastSlash = filepath.find_last_of("/\\");
        if (lastSlash == std::string::npos) {
            return filepath;
        }
        return filepath.substr(lastSlash + 1);
    }

    std::string FileUtils::getFileExtension(const std::string& filepath) {
        size_t lastDot = filepath.find_last_of(".");
        if (lastDot == std::string::npos) {
            return "";
        }
        return filepath.substr(lastDot + 1);
    }

    std::string FileUtils::getDirectory(const std::string& filepath) {
        size_t lastSlash = filepath.find_last_of("/\\");
        if (lastSlash == std::string::npos) {
            return "";
        }
        return filepath.substr(0, lastSlash);
    }
}