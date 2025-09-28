#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

namespace Sparky {
    class State {
    public:
        State(const std::string& name);
        virtual ~State();

        virtual void onEnter() = 0;
        virtual void onUpdate(float deltaTime) = 0;
        virtual void onExit() = 0;

        const std::string& getName() const { return name; }

    private:
        std::string name;
    };

    class StateMachine {
    public:
        StateMachine();
        ~StateMachine();

        void addState(std::unique_ptr<State> state);
        void setState(const std::string& stateName);
        void update(float deltaTime);

        State* getCurrentState() const { return currentState; }
        bool hasState(const std::string& stateName) const;

    private:
        std::unordered_map<std::string, std::unique_ptr<State>> states;
        State* currentState;
        State* previousState;
    };
}