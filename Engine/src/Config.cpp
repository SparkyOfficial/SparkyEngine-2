#include "../include/Config.h"
#include "../include/FileUtils.h"
#include "../include/Logger.h"
#include <fstream>
#include <sstream>
#include <memory>

namespace Sparky {

    // Default constructor
    Config::Config() : configFile("") {
    }

    // Constructor for dependency injection
    Config::Config(const std::string& configFile) : configFile(configFile) {
        if (!configFile.empty()) {
            loadFromFile(configFile);
        }
    }

    Config::~Config() {
    }

    // Singleton instance accessor (backward compatibility)
    Config& Config::getInstance() {
        static Config instance;
        return instance;
    }

    // Factory method for dependency injection
    std::unique_ptr<Config> Config::create(const std::string& configFile) {
        return std::make_unique<Config>(configFile);
    }

    void Config::setInt(const std::string& key, int value) {
        configMap[key] = value;
    }

    void Config::setFloat(const std::string& key, float value) {
        configMap[key] = value;
    }

    void Config::setString(const std::string& key, const std::string& value) {
        configMap[key] = value;
    }

    void Config::setBool(const std::string& key, bool value) {
        configMap[key] = value;
    }

    int Config::getInt(const std::string& key, int defaultValue) const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            try {
                return std::any_cast<int>(it->second);
            } catch (const std::bad_any_cast& e) {
                SPARKY_LOG_WARNING("Failed to cast config value for key: " + key);
            }
        }
        return defaultValue;
    }

    float Config::getFloat(const std::string& key, float defaultValue) const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            try {
                return std::any_cast<float>(it->second);
            } catch (const std::bad_any_cast& e) {
                SPARKY_LOG_WARNING("Failed to cast config value for key: " + key);
            }
        }
        return defaultValue;
    }

    std::string Config::getString(const std::string& key, const std::string& defaultValue) const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            try {
                return std::any_cast<std::string>(it->second);
            } catch (const std::bad_any_cast& e) {
                SPARKY_LOG_WARNING("Failed to cast config value for key: " + key);
            }
        }
        return defaultValue;
    }

    bool Config::getBool(const std::string& key, bool defaultValue) const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            try {
                return std::any_cast<bool>(it->second);
            } catch (const std::bad_any_cast& e) {
                SPARKY_LOG_WARNING("Failed to cast config value for key: " + key);
            }
        }
        return defaultValue;
    }

    bool Config::hasKey(const std::string& key) const {
        return configMap.find(key) != configMap.end();
    }

    void Config::removeKey(const std::string& key) {
        configMap.erase(key);
    }

    void Config::loadFromFile(const std::string& filepath) {
        SPARKY_LOG_INFO("Loading config from file: " + filepath);
        
        if (!FileUtils::fileExists(filepath)) {
            SPARKY_LOG_WARNING("Config file does not exist: " + filepath);
            return;
        }

        try {
            std::vector<char> data = FileUtils::readFile(filepath);
            std::string content(data.begin(), data.end());
            
            std::istringstream iss(content);
            std::string line;
            
            while (std::getline(iss, line)) {
                // Skip empty lines and comments
                if (line.empty() || line[0] == '#' || line[0] == ';') {
                    continue;
                }
                
                // Parse key=value pairs
                size_t equalsPos = line.find('=');
                if (equalsPos != std::string::npos) {
                    std::string key = line.substr(0, equalsPos);
                    std::string value = line.substr(equalsPos + 1);
                    
                    // Trim whitespace
                    key.erase(0, key.find_first_not_of(" \t"));
                    key.erase(key.find_last_not_of(" \t") + 1);
                    value.erase(0, value.find_first_not_of(" \t"));
                    value.erase(value.find_last_not_of(" \t") + 1);
                    
                    // Try to determine the type and store accordingly
                    if (value == "true" || value == "false") {
                        setBool(key, value == "true");
                    } else if (value.find('.') != std::string::npos) {
                        try {
                            setFloat(key, std::stof(value));
                        } catch (...) {
                            setString(key, value);
                        }
                    } else {
                        try {
                            setInt(key, std::stoi(value));
                        } catch (...) {
                            setString(key, value);
                        }
                    }
                }
            }
            
            SPARKY_LOG_INFO("Config loaded successfully");
        } catch (const std::exception& e) {
            SPARKY_LOG_ERROR("Failed to load config: " + std::string(e.what()));
        }
    }

    void Config::saveToFile(const std::string& filepath) {
        SPARKY_LOG_INFO("Saving config to file: " + filepath);
        
        std::ofstream file(filepath);
        if (!file.is_open()) {
            SPARKY_LOG_ERROR("Failed to open config file for writing: " + filepath);
            return;
        }
        
        for (const auto& pair : configMap) {
            const std::string& key = pair.first;
            
            try {
                if (pair.second.type() == typeid(int)) {
                    file << key << "=" << std::any_cast<int>(pair.second) << "\n";
                } else if (pair.second.type() == typeid(float)) {
                    file << key << "=" << std::any_cast<float>(pair.second) << "\n";
                } else if (pair.second.type() == typeid(bool)) {
                    file << key << "=" << (std::any_cast<bool>(pair.second) ? "true" : "false") << "\n";
                } else if (pair.second.type() == typeid(std::string)) {
                    file << key << "=" << std::any_cast<std::string>(pair.second) << "\n";
                }
            } catch (const std::bad_any_cast& e) {
                SPARKY_LOG_WARNING("Failed to save config value for key: " + key);
            }
        }
        
        file.close();
        SPARKY_LOG_INFO("Config saved successfully");
    }
}