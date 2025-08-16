#include "MapMenu.h"
#include "Color.h"
#include "Container2D.h"
#include "ControlCenter.h"
#include "Date.h"
#include "Font.h"
#include "Shape.h"
#include "ShapeManager.h"

namespace MyCraft {
    MapMenuItem::MapMenuItem(const MyBase::Font& font, const std::string& name, const MyBase::Date& date): __name(name), __dateCreated(date), __roundness(0), __size(0,0) {
        __worldNameLabel.setFont(font);
        __worldNameLabel.setText("Name: " + name);
        __worldNameLabel.setTextColor(WHITE);
        __worldNameLabel.setPosition(getPosition() + glm::vec2(0.05, getSize().y - 0.1));
        __worldNameLabel.setScale({0.05, 0.1});
        insert(&__worldNameLabel);

        __dateCreatedLabel.setFont(font);
        __dateCreatedLabel.setTextColor(WHITE);
        __dateCreatedLabel.setText(std::format("Date created: {}:{}:{}", date.getDay(), date.getMonth(), date.getYear()));
        __dateCreatedLabel.setPosition(getPosition() + glm::vec2(0.05, getSize().y - 0.17));
        __dateCreatedLabel.setScale({0.04, 0.06});
        insert(&__dateCreatedLabel);

        MyBase::ShapeManager::getInstance().createShape(__background, __size, __roundness);
    }

    MapMenuItem::~MapMenuItem() {
        MyBase::ShapeManager::getInstance().removeShape(__background, __size, __roundness);
    };


    glm::vec2 MapMenuItem::getPosition() const {
        return MyBase::ShapeContainer::getPosition();
    }
    glm::vec2 MapMenuItem::getSize() const {
        return __size;
    }
    const std::string& MapMenuItem::getName() const {
        return __name;
    }
    const MyBase::Date& MapMenuItem::getDateCreated() const {
        return __dateCreated;
    }
    bool MapMenuItem::contains(const glm::vec2& position) const {
        return __background.contains(position-getPosition());
    }
    void MapMenuItem::setPosition(const glm::vec2& position) {
        __dateCreatedLabel.move(position - getPosition());
        __worldNameLabel.move(position - getPosition());
        MyBase::ShapeContainer::setPosition(position);
    }

    void MapMenuItem::setSize(const glm::vec2& size, const float& roundness) {
        if (size!= __size || roundness != __roundness) {
            __worldNameLabel.move(glm::vec2(0, size.y - __size.y));
            __dateCreatedLabel.move(glm::vec2(0, size.y - __size.y));
            MyBase::ShapeManager::getInstance().removeShape(__background, __size, __roundness);
            __size = size; __roundness = roundness;
            MyBase::ShapeManager::getInstance().createShape(__background, __size, __roundness);
        }
    }
    void MapMenuItem::glDraw() const {
        MyBase::ShapeContainer::draw(__background);
        MyBase::Container2D::glDraw();
    }

    void MapMenuItem::__choose() {
        setFillColor({200,200,200,255});
        __dateCreatedLabel.setTextColor(BLACK);
        __worldNameLabel.setTextColor(BLACK);
    }
    void MapMenuItem::__unchoose() {
        setFillColor(TRANSPARENCY);
        __dateCreatedLabel.setTextColor(WHITE);
        __worldNameLabel.setTextColor(WHITE);
    }
    MapMenu::MapMenu(): __size(0,0), __index(-1) {
        MyBase::ShapeManager::getInstance().createShape(__bound, __size);
        MyBase::ShapeContainer::setFillColor(WHITE);
    }
    MapMenu::~MapMenu() {
        MyBase::ShapeManager::getInstance().removeShape(__bound, __size);
        for (int i = 0; i<__MapMenuItems.size(); i++) delete __MapMenuItems[i];
        __MapMenuItems.clear();
    }
    bool MapMenu::__mouseClicked(GLFWwindow*) {
        if (__getCurrentFocus() != -1) {
            if (__index!=-1) __MapMenuItems[__index]->__unchoose();
            __index = __getCurrentFocus();
            __MapMenuItems[__index]->__choose();
            return true;
        }
        return false;
    }
    bool MapMenu::contains(const glm::vec2& position) const {
        return __bound.contains(position-getPosition());
    }
    glm::vec2 MapMenu::getPosition() const {
        return MyBase::ShapeContainer::getPosition();
    }
    glm::vec2 MapMenu::getSize() const {
        return __size;
    }
    const char& MapMenu::getChoice() const {
        return __index;
    }
    void MapMenu::setFont(const MyBase::Font& font) {
        __font = (MyBase::Font*)&font;
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
        MyBase::ControlCenter::getInstance().EnableScissorMode(getPosition()+glm::vec2(0, 0.02), getSize() - glm::vec2(0, 0.04));
        MyBase::Container2D::glDraw();
        MyBase::ControlCenter::getInstance().DisableScissorMode();
    }
    void MapMenu::add(const std::string& name, const MyBase::Date& date) {
        __MapMenuItems.push_back(new MapMenuItem(*__font, name, date));
        __MapMenuItems.back()->setFillColor(TRANSPARENCY);
        __MapMenuItems.back()->setMarginWidth(2);
        __MapMenuItems.back()->setMarginColor(WHITE);
        __MapMenuItems.back()->setSize({getSize().x - 0.02, 0.2}, 0.01);
        __MapMenuItems.back()->setPosition({getPosition().x + 0.01, getPosition().y + getSize().y - __MapMenuItems.size()*0.23});
        insert(__MapMenuItems.back());
    }
}