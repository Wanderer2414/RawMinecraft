#include "List.h"
#include "Button.h"
#include "ControlCenter.h"
#include "RoundedRectangle.h"
#include "Shape.h"
#include "ShapeManager.h"
#include "VerticalBar.h"
namespace MyBase {
    List::List(): __size(0,0), __normalColor(WHITE), __hoverColor(WHITE), __clickColor(WHITE), __subScale(1, 32), __subPadding(10.0/683, 5.0/384), __childrenHeight(0),__deltaY(0) {
        insertPermanent(&__verticalBar);
    }
    List::~List() {
        ShapeManager::Default->removeShape(__rectangle, __size, 0.01);
        clear();
    }
    bool List::contains(const glm::vec2& position) const {
        return __rectangle.contains(position-getPosition());
    }

    bool List::setHover(const glm::vec2& position) {
        bool is_changed = Controller2D::setHover(position);
        if (isHovered()) {
            is_changed = __verticalBar.setHover(position) || is_changed;
            if (__getCurrentHover()!=-1) {
                is_changed = operator[](__getCurrentHover())->setHover(position) || is_changed;
                if (!operator[](__getCurrentHover())->isHovered()) 
                    __unsetCurrentHover();
            }
            if (__getCurrentHover()==-1) {
                if (__getCurrentFocus()!=-1) {
                    Controller2D* currentFocus = getCurrentFocus();
                    is_changed = currentFocus->setHover(position) || is_changed;
                    if (currentFocus->isHovered()) __setCurrentHover(__getCurrentFocus());
                }
                float y = getPosition().y + getSize().y - position.y + __deltaY;
                if ((position.x >= getPosition().x + __subPadding.x*2 && position.x <= getPosition().x + __subSize.x + __subPadding.x*2) && 
                    (y >= __subPadding.y && y <= (__subSize.y+__subPadding.y)*__buttons.size())) {
                    int index = y/(__subSize.y + __subPadding.y);
                    if (y - index*(__subSize.y+__subPadding.y) >= __subPadding.y) {
                        is_changed = operator[](index)->setHover(position) || is_changed;
                        __setCurrentHover(index);
                    }
                }
            }
        }
        else {
            setHover(false);
            __verticalBar.setHover(false);
        }
        return is_changed;
    }
    void List::setPosition(const glm::vec2& position) {
        glm::vec2 delta = position - getPosition();
        __moveChild(delta);
        ShapeContainer::setPosition(position);
        Text::move(delta);
        __verticalBar.setPosition(position+glm::vec2(0, 0.05));
    }
    void List::setList(const std::vector<std::string>& list) {
        clear();
        __buttons.resize(list.size());
        __childrenHeight = list.size()*(__subSize.y+__subPadding.y) + __subPadding.y;
        __verticalBar.setMaxValue(__childrenHeight-getSize().y);
        float subHeight = __subPadding.y + __subSize.y;
        for (int i = 0; i<list.size(); i++) {
            __buttons[i] = new RoundedRectangleButton();
            __buttons[i]->setFont(getFont());
            __buttons[i]->setTextColor(BLACK);
            __buttons[i]->setPosition(getPosition()+glm::vec2(__subPadding.x*2, getSize().y - __subSize.y - __subPadding.y - i*subHeight));
            __buttons[i]->setSize(__subSize, 0.01);
            __buttons[i]->setNormalColor(WHITE);
            __buttons[i]->setText(list[i]);
            __buttons[i]->setNormalColor(__normalColor);
            __buttons[i]->setHoverColor(__hoverColor);
            __buttons[i]->setClickColor(__clickColor);
            __buttons[i]->setScale({__subScale});
            insert(__buttons[i]);
        }
    }
    void List::setSubScale(const glm::vec2& scale) {
        __subScale = scale;
        for (auto& button: __buttons) 
            button->setScale(scale);
    }
    void List::setNormalColor(const Color& color) {
        if (color != __normalColor) {
            __normalColor = color;
            for (auto& button: __buttons) 
                button->setNormalColor(color);
        }
    }
    void List::setHoverColor(const Color& color) {
        if (color != __hoverColor) {
            __hoverColor = color;
            for (auto& button: __buttons) 
                button->setHoverColor(color);
        }
    }
    void List::setClickColor(const Color& color) {
        if (color != __clickColor) {
            __clickColor = color;
            for (auto& button: __buttons) 
                button->setClickColor(color);
        }
    }
    void List::setFont(const Font& font) {
        Text::setFont(font);
        for (int i = 0; i<__buttons.size(); i++)
            __buttons[i]->setFont(font);
    }
    void List::setSize(const glm::vec2& size) {
        if (__size!=size) {
            ShapeManager::Default->removeShape(__rectangle, __size, 0.01);
            __size = size;
            ShapeManager::Default->createShape(__rectangle, __size, 0.01);
            __subSize = glm::vec2(size.x - __subPadding.x*3, 40.0/384);
            __verticalBar.setHeight(size.y - 0.1);
            __verticalBar.setMaxValue(__childrenHeight-size.y);
        }
    }
    void List::clear() {
        for (int i = 0; i<__buttons.size(); i++) {
            delete __buttons[i];
        }
        Container2D::clear();
        __childrenHeight = 0;
    }
    void List::__moveChild(const glm::vec2& offset) {
        for (int i = 0; i<__buttons.size(); i++) {
            __buttons[i]->move(offset);
        }
    }
    bool List::catchEvent(GLFWwindow* window) {
        bool is_changed = Container2D::catchEvent(window);
        if (__verticalBar.getValue()!=__deltaY) {
            __moveChild({0, __verticalBar.getValue()-__deltaY});
            __deltaY = __verticalBar.getValue();
        }
        return is_changed;
    }
    void List::glDraw() const {
        ControlCenter::Default->enableScissorMode(getPosition(), getSize());
        ShapeContainer::draw(__rectangle);
        Container2D::glDraw();
        ControlCenter::Default->disableScissorMode();
    }
    glm::vec2 List::getSize() const {
        return __size;
    }
    glm::vec2 List::getPosition() const {
        return ShapeContainer::getPosition();
    }
}