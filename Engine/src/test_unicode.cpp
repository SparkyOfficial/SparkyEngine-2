#include <iostream>
#include <fstream>
#include <string>

bool fileExists(const std::string& filepath) {
    std::ifstream file(filepath);
    bool result = file.good();
    std::cout << "File check for '" << filepath << "': " << result << std::endl;
    return result;
}

int main() {
    // Test with a known file
    std::string testPath = "../Game/Release/Engine/shaders/advanced.vert.spv";
    std::cout << "Testing relative file: " << testPath << std::endl;
    bool exists = fileExists(testPath);
    std::cout << "Relative file exists: " << exists << std::endl;
    
    // Also test with absolute path
    std::string absolutePath = "C:\\Users\\Богдан\\Desktop\\SparkyEngine2\\build\\Game\\Release\\Engine\\shaders\\advanced.vert.spv";
    std::cout << "Testing absolute file: " << absolutePath << std::endl;
    bool absoluteExists = fileExists(absolutePath);
    std::cout << "Absolute file exists: " << absoluteExists << std::endl;
    
    return 0;
}