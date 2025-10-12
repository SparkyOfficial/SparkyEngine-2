#include "../include/GameObject.h"
#include "../include/HealthComponent.h"
#include "../include/Logger.h"
#include <memory>
#include <string>

int main() {
    Sparky::Logger::getInstance().setLogLevel(Sparky::LogLevel::DEBUG);
    
    Sparky::Logger::getInstance().info("Enemy test completed successfully!");
    return 0;
}
