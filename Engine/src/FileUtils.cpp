#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Sparky {

    bool FileUtils::fileExists(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Checking if file exists: " + filepath);
        
#ifdef _WIN32
        // Convert UTF-8 string to wide string for Windows
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, filepath.c_str(), (int)filepath.size(), NULL, 0);
        if (size_needed == 0) {
            SPARKY_LOG_ERROR("Failed to convert file path to wide string: " + filepath);
            return false;
        }
        std::wstring wpath(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, filepath.c_str(), (int)filepath.size(), &wpath[0], size_needed);
        std::ifstream file(wpath);
#else
        std::ifstream file(filepath);
#endif
        bool exists = file.good();
        SPARKY_LOG_DEBUG("File exists: " + std::to_string(exists) + " for path: " + filepath);
        return exists;
    }

    std::vector<char> FileUtils::readFile(const std::string& filepath) {
        SPARKY_LOG_DEBUG("Reading file: " + filepath);
        
#ifdef _WIN32
        // Convert UTF-8 string to wide string for Windows
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, filepath.c_str(), (int)filepath.size(), NULL, 0);
        if (size_needed == 0) {
            SPARKY_LOG_ERROR("Failed to convert file path to wide string: " + filepath);
            throw std::runtime_error("failed to convert file path: " + filepath);
        }
        std::wstring wpath(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, filepath.c_str(), (int)filepath.size(), &wpath[0], size_needed);
        std::ifstream file(wpath, std::ios::ate | std::ios::binary);
#else
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);
#endif

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
        
#ifdef _WIN32
        // Convert UTF-8 string to wide string for Windows
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, filepath.c_str(), (int)filepath.size(), NULL, 0);
        if (size_needed == 0) {
            SPARKY_LOG_ERROR("Failed to convert file path to wide string: " + filepath);
            return false;
        }
        std::wstring wpath(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, filepath.c_str(), (int)filepath.size(), &wpath[0], size_needed);
        std::ofstream file(wpath, std::ios::binary);
#else
        std::ofstream file(filepath, std::ios::binary);
#endif

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
#ifdef _WIN32
        std::vector<wchar_t> pathBuf;
        DWORD copied = 0;
        do {
            pathBuf.resize(pathBuf.size() + MAX_PATH);
            copied = GetModuleFileNameW(NULL, &pathBuf[0], (DWORD)pathBuf.size());
        } while (copied >= pathBuf.size());

        pathBuf.resize(copied);

        std::wstring widePath(pathBuf.begin(), pathBuf.end());
        
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
        
        SPARKY_LOG_DEBUG("Executable path: " + strTo);
        return strTo;
#else
        // For non-Windows platforms, we would need different implementation
        // This is a placeholder implementation
        SPARKY_LOG_WARNING("getExecutablePath not implemented for this platform");
        return "";
#endif
    }
}