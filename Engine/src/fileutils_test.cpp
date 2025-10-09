#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <iostream>
#include <string>

int main() {
    // Set up logger
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    std::cout << "Testing FileUtils implementation" << std::endl;
    
    // Test with a known file
    std::string testPath = "../Game/Release/Engine/shaders/advanced.vert.spv";
    std::cout << "Testing relative file: " << testPath << std::endl;
    
    bool exists = Sparky::FileUtils::fileExists(testPath);
    std::cout << "Relative file exists: " << exists << std::endl;
    
    if (exists) {
        try {
            auto data = Sparky::FileUtils::readFile(testPath);
            std::cout << "File read successfully, size: " << data.size() << " bytes" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Failed to read file: " << e.what() << std::endl;
        }
    }
    
    // Also test with absolute path
    std::string absolutePath = "C:\\Users\\Богдан\\Desktop\\SparkyEngine2\\build\\Game\\Release\\Engine\\shaders\\advanced.vert.spv";
    std::cout << "Testing absolute file: " << absolutePath << std::endl;
    
    bool absoluteExists = Sparky::FileUtils::fileExists(absolutePath);
    std::cout << "Absolute file exists: " << absoluteExists << std::endl;
    
    if (absoluteExists) {
        try {
            auto data = Sparky::FileUtils::readFile(absolutePath);
            std::cout << "Absolute file read successfully, size: " << data.size() << " bytes" << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Failed to read absolute file: " << e.what() << std::endl;
        }
    }
    
    return 0;
}