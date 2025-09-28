#pragma once

#include <string>
#include <unordered_map>
#include <any>

namespace Sparky {
    class Config {
    public:
        static Config& getInstance();

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

    private:
        Config();
        ~Config();

        std::unordered_map<std::string, std::any> configMap;
    };
}