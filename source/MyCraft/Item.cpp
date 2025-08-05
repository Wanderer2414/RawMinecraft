#include "Item.h"
#include "Block.h"
#include "Shape.h"
#include "ShapeManager.h"
#include "Texture.h"
namespace MyCraft {

    bool isBlock(const ItemType& type) {
        if (type>=ItemType::Dirt && type<=ItemType::Ice) return true;
        return false;
    }

    BlockCatogary to_block(const ItemType& type) {
        return (BlockCatogary)type;
    }

    Item::Item(const ItemType& type, const unsigned char& count):  __type(type), __count(count) {}
    Item::~Item() {}

    Item::operator const ItemType&() const {
        return __type;
    }
    bool Item::isTool() const {
        return false;
    }
    bool Item::isBlock() const {
        return false;
    }

    unsigned char Item::getCount() const {
        return __count;
    }
    void Item::setCount(const int& count) {
        __count = count;
        update();
    }

    BlockItem::BlockItem(const ItemPacked& package, const ItemType& type, const unsigned char& count): Item(type, count) {
        MyBase::TextureContainer::setTexture(package.texture);
        int index= int(type);
        setTextureImportPosition(glm::vec2(160.f*(index%3)/getTexture().getSize().x, 1-160.f*int(index/3+1)/getTexture().getSize().y));
        setTextureImportSize(glm::vec2(160.f, 160.f)/(glm::vec2)getTexture().getSize());
        setTextureExportSize(package.size);

        Text::setFont(package.font);
        Text::setScale({package.size.x/2, package.size.y/2});
        Text::setTextColor(WHITE);
        Text::setText(std::to_string(getCount()));
    }
    BlockItem::~BlockItem() {}

    bool BlockItem::isBlock() const {
        return true;
    }
    
    void BlockItem::setPosition(const glm::vec2& position) {
        setTextureExportPosition(position);
        Text::setPosition(position + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void BlockItem::update() {
        Text::setText(std::to_string(getCount()));
        Text::setPosition(getTextureExportPosition() + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void BlockItem::draw() const {
        MyBase::TextureContainer::draw();
        Text::draw();
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
    bool ToolItem::isTool() const {
        return true;
    }
    void ToolItem::setPosition(const glm::vec2& position) {
        MyBase::ShapeContainer::setPosition(position);
        setTextureExportPosition(position);
    }
    void ToolItem::update() {
        if (__currentCount != getCount()) {
            if (getCount()<32) setFillColor(RED);
            else if (getCount()<64) setFillColor(YELLOW);
            glm::vec2 size = getTextureExportSize();
            size.x = 1.0f*size.y/255*__currentCount;
            MyBase::ShapeManager::getInstance().removeShape(__durabilityBackground, size);
            __currentCount = getCount();
            size.x = 1.0f*size.y/255*getCount();
            MyBase::ShapeManager::getInstance().createShape(__durabilityBackground, size);
        }
    }
    void ToolItem::draw() const {
        MyBase::ShapeContainer::draw(__durabilityBackground);
        MyBase::TextureContainer::draw();
    }
}