#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <stack>

namespace Sparky {
    class Profiler {
    public:
        static Profiler& getInstance();

        void beginProfile(const std::string& name);
        void endProfile(const std::string& name);
        void reset();

        void printReport();

    private:
        Profiler();
        ~Profiler();

        struct ProfileData {
            std::chrono::high_resolution_clock::time_point startTime;
            long long totalTime;
            int callCount;
        };

        std::unordered_map<std::string, ProfileData> profiles;
        std::stack<std::string> profileStack;
    };

    // Helper class for scoped profiling
    class ProfileScope {
    public:
        ProfileScope(const std::string& name);
        ~ProfileScope();

    private:
        std::string name;
    };
}

// Convenience macro for profiling
#define SPARKY_PROFILE(name) Sparky::ProfileScope profileScope(name);