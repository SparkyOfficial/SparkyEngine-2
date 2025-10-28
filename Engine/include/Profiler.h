#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <stack>
#include <memory>

namespace Sparky {
    class Profiler {
    public:
        Profiler();
        ~Profiler();

        // Constructor for dependency injection
        Profiler(const std::string& profilerName);

        static Profiler& getInstance();

        // Method to create a new Profiler instance for dependency injection
        static std::unique_ptr<Profiler> create(const std::string& profilerName = "Default");

        void beginProfile(const std::string& name);
        void endProfile(const std::string& name);
        void reset();

        void printReport();

        // Getters
        const std::string& getProfilerName() const { return profilerName; }

    private:
        struct ProfileData {
            std::chrono::high_resolution_clock::time_point startTime;
            long long totalTime;
            int callCount;
        };

        std::unordered_map<std::string, ProfileData> profiles;
        std::stack<std::string> profileStack;
        std::string profilerName;
    };

    // Helper class for scoped profiling
    class ProfileScope {
    public:
        ProfileScope(const std::string& name);
        ProfileScope(const std::string& name, Profiler& profiler); // New constructor for DI
        ~ProfileScope();

    private:
        std::string name;
        Profiler* profiler; // Pointer to specific profiler instance
        bool useDefaultProfiler;
    };
}

// Convenience macros for profiling
#define SPARKY_PROFILE(name) Sparky::ProfileScope profileScope(name);
#define SPARKY_PROFILE_DI(name, profiler) Sparky::ProfileScope profileScope(name, profiler);