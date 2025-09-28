#pragma once

#include <string>
#include <iostream>

namespace Sparky {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    class Logger {
    public:
        static Logger& getInstance();
        
        void setLogLevel(LogLevel level);
        void log(LogLevel level, const std::string& message);
        
        void debug(const std::string& message);
        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);

    private:
        Logger();
        ~Logger();
        
        LogLevel currentLevel;
        
        std::string levelToString(LogLevel level);
    };
}

// Convenience macros
#define SPARKY_LOG_DEBUG(message) Sparky::Logger::getInstance().debug(message)
#define SPARKY_LOG_INFO(message) Sparky::Logger::getInstance().info(message)
#define SPARKY_LOG_WARNING(message) Sparky::Logger::getInstance().warning(message)
#define SPARKY_LOG_ERROR(message) Sparky::Logger::getInstance().error(message)