#include "NonuseItem.h"
#include "Item.h"
namespace MyCraft {
    bool isNonuseItem(const ItemType& type) {
        if (type == ItemType::Stick) return true;
        return false;
    }
    NonuseItem::NonuseItem(const ItemPacked& package, const ItemType& type, const unsigned char& count): Item(type, count), __package(package) {
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
    NonuseItem::~NonuseItem() {}

    glm::vec2 NonuseItem::getPosition() const {
        return getTextureExportPosition();
    }
    void NonuseItem::setPosition(const glm::vec2& position) {
        setTextureExportPosition(position);
        Text::setPosition(position + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void NonuseItem::update() {
        Text::setText(std::to_string(getCount()));
        Text::setPosition(getTextureExportPosition() + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void NonuseItem::draw() const {
        MyBase::TextureContainer::draw();
        Text::draw();
    }

    Item* NonuseItem::merge(Item*& item) {
        if (ItemType(*this) != *item) return item;
        if (getCount()+item->getCount()<=64) {
            setCount(getCount()+item->getCount());
            delete item;
            item = 0;
            update();
        }
        else {
            item->setCount(getCount() + item->getCount() - 64);
            setCount(64);
            update();
            item->update();
        }
        return item;
    }

    Item* NonuseItem::split(const int& count) {
        if (count<=0 || count >= getCount()) return 0;
        Item* item = new NonuseItem(__package, *this, count);
        setCount(getCount()-count);
        return item;
    }
}