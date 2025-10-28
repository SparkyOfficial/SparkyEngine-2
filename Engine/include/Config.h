#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <memory>

namespace Sparky {
    class Config {
    public:
        Config();
        ~Config();

        // Constructor for dependency injection
        Config(const std::string& configFile);

        static Config& getInstance();

        // Method to create a new Config instance for dependency injection
        static std::unique_ptr<Config> create(const std::string& configFile = "");

        void setInt(const std::string& key, int value);
        void setFloat(const std::string& key, float value);
        void setString(const std::string& key, const std::string& value);
        void setBool(const std::string& key, bool value);

        int getInt(const std::string& key, int defaultValue = 0) const;
        float getFloat(const std::string& key, float defaultValue = 0.0f) const;
        std::string getString(const std::string& key, const std::string& defaultValue = "") const;
        bool getBool(const std::string& key, bool defaultValue = false) const;

        bool hasKey(const std::string& key) const;
        void removeKey(const std::string& key);

        void loadFromFile(const std::string& filepath);
        void saveToFile(const std::string& filepath);

        // Getter for config file path
        const std::string& getConfigFile() const { return configFile; }

    private:
        std::unordered_map<std::string, std::any> configMap;
        std::string configFile;
    };
}