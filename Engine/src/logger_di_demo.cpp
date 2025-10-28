#include "../include/Logger.h"
#include <iostream>
#include <memory>

// Example class that uses Logger through dependency injection
class GameSystem {
private:
    std::unique_ptr<Sparky::Logger> logger;
    
public:
    // Constructor that accepts a Logger instance
    GameSystem(std::unique_ptr<Sparky::Logger> logger) : logger(std::move(logger)) {
        if (this->logger) {
            this->logger->info("GameSystem created with injected logger");
        }
    }
    
    // Alternative constructor that creates its own logger
    GameSystem() : logger(Sparky::Logger::create(Sparky::LogLevel::DEBUG)) {
        if (this->logger) {
            this->logger->debug("GameSystem created with own logger");
        }
    }
    
    void doSomething() {
        if (logger) {
            logger->debug("GameSystem is doing something");
        }
    }
    
    void handleError() {
        if (logger) {
            logger->error("GameSystem encountered an error");
        }
    }
    
    // Method to change log level
    void setLogLevel(Sparky::LogLevel level) {
        if (logger) {
            logger->setLogLevel(level);
        }
    }
};

// Another example class showing different usage patterns
class NetworkManager {
private:
    Sparky::Logger* logger; // Raw pointer for shared logger
    
public:
    // Constructor that accepts a shared Logger instance
    NetworkManager(Sparky::Logger* logger) : logger(logger) {
        if (this->logger) {
            this->logger->info("NetworkManager created with shared logger");
        }
    }
    
    void connect() {
        if (logger) {
            logger->info("Connecting to server...");
        }
    }
    
    void disconnect() {
        if (logger) {
            logger->info("Disconnecting from server...");
        }
    }
};

int main() {
    std::cout << "=== Logger Dependency Injection Demo ===" << std::endl;
    
    // Example 1: Using unique_ptr for owned logger
    std::cout << "\n--- Example 1: Owned Logger ---" << std::endl;
    auto gameSystem = std::make_unique<GameSystem>(Sparky::Logger::create(Sparky::LogLevel::DEBUG));
    gameSystem->doSomething();
    gameSystem->handleError();
    
    // Example 2: Using default constructor that creates its own logger
    std::cout << "\n--- Example 2: Self-Created Logger ---" << std::endl;
    auto gameSystem2 = std::make_unique<GameSystem>();
    gameSystem2->setLogLevel(Sparky::LogLevel::DEBUG);
    gameSystem2->doSomething();
    
    // Example 3: Sharing a logger instance
    std::cout << "\n--- Example 3: Shared Logger ---" << std::endl;
    auto sharedLogger = Sparky::Logger::create(Sparky::LogLevel::INFO);
    auto networkManager = std::make_unique<NetworkManager>(sharedLogger.get());
    networkManager->connect();
    gameSystem->setLogLevel(Sparky::LogLevel::INFO); // This won't affect the shared logger
    sharedLogger->info("This message comes from shared logger directly");
    networkManager->disconnect();
    
    // Example 4: Backward compatibility with singleton
    std::cout << "\n--- Example 4: Singleton (Backward Compatibility) ---" << std::endl;
    SPARKY_LOG_INFO("This is using the old singleton pattern");
    Sparky::Logger::getInstance().warning("This is also using the singleton but with direct method call");
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}