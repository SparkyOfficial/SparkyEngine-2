#pragma once

#include <string>
#include <vector>

namespace Sparky {
    class FileUtils {
    public:
        static bool fileExists(const std::string& filepath);
        static std::vector<char> readFile(const std::string& filepath);
        static bool writeFile(const std::string& filepath, const std::vector<char>& data);
        static std::string getFileName(const std::string& filepath);
        static std::string getFileExtension(const std::string& filepath);
        static std::string getDirectory(const std::string& filepath);
        static std::string getExecutablePath();
    };
}