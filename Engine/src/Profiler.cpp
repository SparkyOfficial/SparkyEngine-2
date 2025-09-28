#include "../include/Profiler.h"
#include "../include/Logger.h"
#include <iostream>
#include <iomanip>

namespace Sparky {

    Profiler::Profiler() {
    }

    Profiler::~Profiler() {
    }

    Profiler& Profiler::getInstance() {
        static Profiler instance;
        return instance;
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
            SPARKY_LOG_INFO("Profiler: No profiling data collected");
            return;
        }

        SPARKY_LOG_INFO("=== Profiling Report ===");
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
    ProfileScope::ProfileScope(const std::string& name) : name(name) {
        Profiler::getInstance().beginProfile(name);
    }

    ProfileScope::~ProfileScope() {
        Profiler::getInstance().endProfile(name);
    }
}