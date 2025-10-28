#pragma once

#include <string>
#include <iostream>
#include <memory>

namespace Sparky {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERR
    };

    class Logger {
    public:
        Logger();
        ~Logger();
        
        // Constructor for dependency injection
        Logger(LogLevel level);
        
        static Logger& getInstance();
        
        void setLogLevel(LogLevel level);
        void log(LogLevel level, const std::string& message);
        
        void debug(const std::string& message);
        void info(const std::string& message);
        void warning(const std::string& message);
        void error(const std::string& message);

        // Method to create a new logger instance for dependency injection
        static std::unique_ptr<Logger> create(LogLevel level = LogLevel::INFO);

    private:
        LogLevel currentLevel;
        
        std::string levelToString(LogLevel level);
    };
}

// Convenience macros (still using singleton for backward compatibility)
#define SPARKY_LOG_DEBUG(message) Sparky::Logger::getInstance().debug(message)
#define SPARKY_LOG_INFO(message) Sparky::Logger::getInstance().info(message)
#define SPARKY_LOG_WARNING(message) Sparky::Logger::getInstance().warning(message)
#define SPARKY_LOG_ERROR(message) Sparky::Logger::getInstance().error(message)