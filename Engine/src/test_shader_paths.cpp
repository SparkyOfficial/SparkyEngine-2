#include <iostream>
#include <vector>
#include <string>

// Forward declarations for the functions we need
namespace Sparky {
    namespace FileUtils {
        std::string getExecutablePath();
        std::string getDirectory(const std::string& filepath);
        bool fileExists(const std::string& filepath);
    }
}

int main() {
    // Test the file utilities
    std::string exePath = Sparky::FileUtils::getExecutablePath();
    std::cout << "Executable path: " << exePath << std::endl;
    
    std::string exeDir = Sparky::FileUtils::getDirectory(exePath);
    std::cout << "Executable directory: " << exeDir << std::endl;
    
    // Test shader paths
    std::vector<std::string> searchPaths = {
        exeDir + "/Engine/shaders/",
        exeDir + "/../Engine/shaders/",
        exeDir + "/../../Engine/shaders/"
    };
    
    for (const auto& path : searchPaths) {
        std::string vertPath = path + "advanced.vert.spv";
        std::string fragPath = path + "advanced.frag.spv";
        std::cout << "Checking path: " << path << std::endl;
        std::cout << "  Vertex shader exists: " << Sparky::FileUtils::fileExists(vertPath) << std::endl;
        std::cout << "  Fragment shader exists: " << Sparky::FileUtils::fileExists(fragPath) << std::endl;
    }
    
    return 0;
}