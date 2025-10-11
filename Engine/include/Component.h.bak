#pragma once

namespace Sparky {
    class GameObject;

    class Component {
    public:
        Component();
        virtual ~Component();

        virtual void update(float deltaTime) = 0;
        virtual void render() = 0;

        void setOwner(GameObject* owner);
        GameObject* getOwner() const;

    protected:
        GameObject* owner;
    };
}