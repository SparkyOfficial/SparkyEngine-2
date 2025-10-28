#include "../include/Profiler.h"
#include "../include/Logger.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// Demo to showcase Profiler dependency injection
int main() {
    std::cout << "=== Profiler Dependency Injection Demo ===" << std::endl;
    
    // Example 1: Using singleton (backward compatibility)
    std::cout << "\n--- Example 1: Singleton (Backward Compatibility) ---" << std::endl;
    Sparky::Profiler& singletonProfiler = Sparky::Profiler::getInstance();
    std::cout << "Singleton Profiler name: " << singletonProfiler.getProfilerName() << std::endl;
    
    // Example 2: Creating Profiler with dependency injection
    std::cout << "\n--- Example 2: Dependency Injection ---" << std::endl;
    auto customProfiler = std::make_unique<Sparky::Profiler>("GameProfiler");
    std::cout << "Custom Profiler name: " << customProfiler->getProfilerName() << std::endl;
    
    // Example 3: Using factory method
    std::cout << "\n--- Example 3: Factory Method ---" << std::endl;
    auto factoryProfiler = Sparky::Profiler::create("RenderProfiler");
    std::cout << "Factory Profiler name: " << factoryProfiler->getProfilerName() << std::endl;
    
    // Example 4: Multiple Profiler instances for different systems
    std::cout << "\n--- Example 4: Multiple Instances ---" << std::endl;
    auto gameProfiler = Sparky::Profiler::create("GameLogic");
    auto renderProfiler = Sparky::Profiler::create("Rendering");
    auto physicsProfiler = Sparky::Profiler::create("Physics");
    auto audioProfiler = Sparky::Profiler::create("Audio");
    
    std::cout << "Game profiler: " << gameProfiler->getProfilerName() << std::endl;
    std::cout << "Render profiler: " << renderProfiler->getProfilerName() << std::endl;
    std::cout << "Physics profiler: " << physicsProfiler->getProfilerName() << std::endl;
    std::cout << "Audio profiler: " << audioProfiler->getProfilerName() << std::endl;
    
    // Example 5: Profiling with different profilers
    std::cout << "\n--- Example 5: Profiling with Different Profilers ---" << std::endl;
    
    // Profile some game logic with the game profiler
    {
        SPARKY_PROFILE_DI("GameUpdate", *gameProfiler);
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate work
    }
    
    // Profile some rendering with the render profiler
    {
        SPARKY_PROFILE_DI("RenderFrame", *renderProfiler);
        std::this_thread::sleep_for(std::chrono::milliseconds(15)); // Simulate work
    }
    
    // Profile some physics with the physics profiler
    {
        SPARKY_PROFILE_DI("PhysicsStep", *physicsProfiler);
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Simulate work
    }
    
    // Also use the singleton profiler for comparison
    {
        SPARKY_PROFILE("SingletonTest");
        std::this_thread::sleep_for(std::chrono::milliseconds(2)); // Simulate work
    }
    
    // Print reports from all profilers
    std::cout << "\n--- Profiling Reports ---" << std::endl;
    gameProfiler->printReport();
    renderProfiler->printReport();
    physicsProfiler->printReport();
    singletonProfiler.printReport();
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    
    return 0;
}