#include "MapMenu.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyCraft {
    MapMenu::MapMenu(): __size(0,0) {
        MyBase::ShapeManager::getInstance().createShape(__bound, __size);
        MyBase::ShapeContainer::setFillColor(WHITE);
    }
    MapMenu::~MapMenu() {
        MyBase::ShapeManager::getInstance().removeShape(__bound, __size);
    }
    glm::vec2 MapMenu::getPosition() const {
        return MyBase::ShapeContainer::getPosition();
    }
    glm::vec2 MapMenu::getSize() const {
        return __size;
    }
    void MapMenu::setSize(const glm::vec2& size) {
        if (size!=__size) {
            MyBase::ShapeManager::getInstance().removeShape(__bound, __size);
            __size = size;
            MyBase::ShapeManager::getInstance().createShape(__bound, __size);
        }
    }
    void MapMenu::glDraw() const {
        MyBase::ShapeContainer::draw(__bound);
    }
    bool MapMenu::contains(const glm::vec2& position) const {
        return false;
    }
}