#pragma once

#include "StateMachine.h"

namespace Sparky {
    class ExampleState : public State {
    public:
        ExampleState(const std::string& name);
        ~ExampleState();

        void onEnter() override;
        void onUpdate(float deltaTime) override;
        void onExit() override;

    private:
        float timer;
    };
}