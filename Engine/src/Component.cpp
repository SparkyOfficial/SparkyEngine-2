#include "../include/Component.h"

namespace Sparky {

    Component::Component() : owner(nullptr) {
    }

    Component::~Component() {
    }

    void Component::setOwner(GameObject* owner) {
        this->owner = owner;
    }

    GameObject* Component::getOwner() const {
        return owner;
    }
}