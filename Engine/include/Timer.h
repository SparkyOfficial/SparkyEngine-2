#pragma once

namespace Sparky {
    class Timer {
    public:
        Timer();
        
        void start();
        void stop();
        void reset();
        
        float getElapsedTime() const;
        bool isRunning() const;
        
    private:
        float startTime;
        float elapsedTime;
        bool running;
    };
}