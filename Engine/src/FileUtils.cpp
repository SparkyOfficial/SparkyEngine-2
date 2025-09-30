#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>

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

        // Log first part of file content for debugging
        size_t previewSize = fileSize < 200 ? fileSize : 200;
        std::string content(buffer.data(), previewSize);
        SPARKY_LOG_DEBUG("File content preview: " + content);
        
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

    std::string FileUtils::getExecutablePath() {
        std::vector<wchar_t> pathBuf;
        DWORD copied = 0;
        do {
            pathBuf.resize(pathBuf.size() + MAX_PATH);
            copied = GetModuleFileNameW(NULL, &pathBuf[0], pathBuf.size());
        } while (copied >= pathBuf.size());

        pathBuf.resize(copied);

        std::wstring widePath(pathBuf.begin(), pathBuf.end());
        
        // Log the wide path for debugging
        SPARKY_LOG_DEBUG("Wide executable path: " + std::to_string(widePath.length()) + " chars");
        
        // Log each character of the wide path for debugging
        std::string widePathDebug = "Wide path characters: ";
        for (size_t i = 0; i < widePath.length() && i < 20; ++i) {
            widePathDebug += std::to_string((int)widePath[i]) + " ";
        }
        SPARKY_LOG_DEBUG(widePathDebug);
        
        // Convert wide string to UTF-8 with error handling
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, widePath.c_str(), (int)widePath.size(), NULL, 0, NULL, NULL);
        if (size_needed == 0) {
            SPARKY_LOG_ERROR("Failed to convert wide string to UTF-8");
            return "";
        }
        
        std::string strTo(size_needed, 0);
        int converted = WideCharToMultiByte(CP_UTF8, 0, widePath.c_str(), (int)widePath.size(), &strTo[0], size_needed, NULL, NULL);
        if (converted == 0) {
            SPARKY_LOG_ERROR("Failed to convert wide string to UTF-8");
            return "";
        }
        
        SPARKY_LOG_DEBUG("Final executable path: " + strTo);
        
        return strTo;
    }
}