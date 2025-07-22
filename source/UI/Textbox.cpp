#include "Textbox.h"
#include "Controller2D.h"
#include "Shape.h"
#include "ShapeManager.h"
namespace MyBase {
    Textbox::Textbox(): __size(0,0), __roundness(0) {
        ShapeManager::getInstance().createShape(__bound, __size, __roundness);
        setMarginColor(BLACK);
    }
    Textbox::~Textbox() {
        ShapeManager::getInstance().removeShape(__bound, __size, __roundness);
    }

    glm::vec2 Textbox::getPosition() const {
        return ShapeContainer::getPosition();
    };
    glm::vec2 Textbox::getSize() const {
        return __size;
    };
    void Textbox::setSize(const glm::vec2& size, const float& roundness) {
        if (size!=__size || __roundness != roundness) {
            ShapeManager::getInstance().removeShape(__bound, __size, __roundness);
            __size = size; __roundness = roundness;
            ShapeManager::getInstance().createShape(__bound, __size, __roundness);
        }
    };
    void Textbox::setPosition(const glm::vec2& position) {
        ShapeContainer::setPosition(position);
    };
    bool Textbox::__getFocus() {
        setMarginWidth(2);
        return true;
    }
    bool Textbox::__lostFocus() {
        setMarginWidth(0);
        return true;
    }
    bool Textbox::contains(const glm::vec2& position) const {
        return __bound.contains(position-getPosition());
    };
    void Textbox::glDraw() const {
        ShapeContainer::draw(__bound);
    }
}