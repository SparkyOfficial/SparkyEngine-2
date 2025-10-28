#include "../include/Profiler.h"
#include "../include/Logger.h"
#include <iostream>
#include <iomanip>
#include <memory>

namespace Sparky {

    // Default constructor
    Profiler::Profiler() : profilerName("Default") {
    }

    // Constructor for dependency injection
    Profiler::Profiler(const std::string& profilerName) : profilerName(profilerName) {
        SPARKY_LOG_DEBUG("Profiler created with name: " + profilerName);
    }

    Profiler::~Profiler() {
    }

    // Singleton instance accessor (backward compatibility)
    Profiler& Profiler::getInstance() {
        static Profiler instance;
        return instance;
    }

    // Factory method for dependency injection
    std::unique_ptr<Profiler> Profiler::create(const std::string& profilerName) {
        return std::make_unique<Profiler>(profilerName);
    }

    void Profiler::beginProfile(const std::string& name) {
        profiles[name].startTime = std::chrono::high_resolution_clock::now();
        profileStack.push(name);
    }

    void Profiler::endProfile(const std::string& name) {
        if (profileStack.empty() || profileStack.top() != name) {
            SPARKY_LOG_WARNING("Profiler: Mismatched begin/end profile calls for " + name);
            return;
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - profiles[name].startTime);

        profiles[name].totalTime += duration.count();
        profiles[name].callCount++;

        profileStack.pop();
    }

    void Profiler::reset() {
        profiles.clear();
        while (!profileStack.empty()) {
            profileStack.pop();
        }
    }

    void Profiler::printReport() {
        if (profiles.empty()) {
            SPARKY_LOG_INFO("Profiler '" + profilerName + "': No profiling data collected");
            return;
        }

        SPARKY_LOG_INFO("=== Profiling Report (" + profilerName + ") ===");
        for (const auto& pair : profiles) {
            const std::string& name = pair.first;
            const ProfileData& data = pair.second;

            if (data.callCount > 0) {
                double averageTime = static_cast<double>(data.totalTime) / data.callCount;
                SPARKY_LOG_INFO(name + ": " + std::to_string(data.callCount) + " calls, " + 
                               std::to_string(data.totalTime) + " μs total, " + 
                               std::to_string(averageTime) + " μs avg");
            }
        }
        SPARKY_LOG_INFO("========================");
    }

    // ProfileScope implementation
    ProfileScope::ProfileScope(const std::string& name) : name(name), profiler(nullptr), useDefaultProfiler(true) {
        Profiler::getInstance().beginProfile(name);
    }

    ProfileScope::ProfileScope(const std::string& name, Profiler& profiler) : name(name), profiler(&profiler), useDefaultProfiler(false) {
        profiler.beginProfile(name);
    }

    ProfileScope::~ProfileScope() {
        if (useDefaultProfiler) {
            Profiler::getInstance().endProfile(name);
        } else {
            profiler->endProfile(name);
        }
    }
}