#include "../include/Logger.h"
#include <iostream>
#include <ctime>
#include <memory>

namespace Sparky {

    Logger::Logger() : currentLevel(LogLevel::INFO) {
    }

    // Constructor for dependency injection
    Logger::Logger(LogLevel level) : currentLevel(level) {
    }

    Logger::~Logger() {
    }

    Logger& Logger::getInstance() {
        static Logger instance;
        return instance;
    }

    std::unique_ptr<Logger> Logger::create(LogLevel level) {
        return std::make_unique<Logger>(level);
    }

    void Logger::setLogLevel(LogLevel level) {
        currentLevel = level;
    }

    void Logger::log(LogLevel level, const std::string& message) {
        if (static_cast<int>(level) >= static_cast<int>(currentLevel)) {
            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            
            char timeStr[9];
            std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", localTime);
            
            std::cout << "[" << timeStr << "] [" << levelToString(level) << "] " << message << std::endl;
        }
    }

    void Logger::debug(const std::string& message) {
        log(LogLevel::DEBUG, message);
    }

    void Logger::info(const std::string& message) {
        log(LogLevel::INFO, message);
    }

    void Logger::warning(const std::string& message) {
        log(LogLevel::WARNING, message);
    }

    void Logger::error(const std::string& message) {
        log(LogLevel::ERR, message);
    }

    std::string Logger::levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERR: return "ERROR";
            default: return "UNKNOWN";
        }
    }
}