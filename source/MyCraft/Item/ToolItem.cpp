#include "Item.h"
#include "ShapeManager.h"
#include "ToolItem.h"

namespace MyCraft {

    bool isToolItem(const ItemType& type) {
        if (type>=ItemType::WoodedAxe && type<=ItemType::NetheriteShovel)
            return true;
        return false;
    }
    
    ToolItem::ToolItem(const ItemPacked& package, const ItemType& type): Item(type, 255), __currentCount(255) {
        MyBase::TextureContainer::setTexture(package.texture);
        int index= int(type);
        setTextureImportPosition(glm::vec2(160.f*(index%3)/getTexture().getSize().x, 1-160.f*int(index/3+1)/getTexture().getSize().y));
        setTextureImportSize(glm::vec2(160.f, 160.f)/(glm::vec2)getTexture().getSize());
        setTextureExportSize(package.size);

        MyBase::ShapeContainer::setPosition(getTextureExportPosition());
        MyBase::ShapeContainer::setFillColor(GREEN);
        
        glm::vec2 size = getTextureExportSize();
        size.y = 1.0f*size.y/255*getCount();
        MyBase::ShapeManager::getInstance().createShape(__durabilityBackground, size);
    }
    ToolItem::~ToolItem() {
        glm::vec2 size = getTextureExportSize();
        size.y = 1.0f*size.y/255*__currentCount;
        MyBase::ShapeManager::getInstance().removeShape(__durabilityBackground, size);
    }
    glm::vec2 ToolItem::getPosition() const {
        return getTextureExportPosition();
    }
    void ToolItem::setPosition(const glm::vec2& position) {
        MyBase::ShapeContainer::setPosition(position);
        setTextureExportPosition(position);
    }
    void ToolItem::update() {
        if (__currentCount != getCount()) {
            if (getCount()<32) setFillColor(RED);
            else if (getCount()<128) setFillColor(YELLOW);
            glm::vec2 size = getTextureExportSize();
            size.y = 1.0f*getTextureExportSize().y/255*__currentCount;
            MyBase::ShapeManager::getInstance().removeShape(__durabilityBackground, size);
            __currentCount = getCount();
            size.y = 1.0f*getTextureExportSize().y/255*getCount();
            MyBase::ShapeManager::getInstance().createShape(__durabilityBackground, size);
        }
    }
    void ToolItem::draw() const {
        MyBase::ShapeContainer::draw(__durabilityBackground);
        MyBase::TextureContainer::draw();
    }
    Item* ToolItem::merge(Item*& item) {
        return item;
    }

    Item* ToolItem::split(const int& count) {
        return 0;
    }
}