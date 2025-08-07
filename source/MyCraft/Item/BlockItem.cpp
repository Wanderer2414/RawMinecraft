#include "BlockItem.h"
namespace MyCraft {

    bool isBlock(const ItemType& type) {
        if (type>=ItemType::Dirt && type<=ItemType::Chest) return true;
        return false;
    }
    
    BlockItem::BlockItem(const ItemPacked& package, const ItemType& type, const unsigned char& count): Item(type, count), __package(package) {
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

    glm::vec2 BlockItem::getPosition() const {
        return getTextureExportPosition();
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

    Item* BlockItem::merge(Item*& item) {
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

    Item* BlockItem::split(const int& count) {
        if (count<=0 || count >= getCount()) return 0;
        Item* item = new BlockItem(__package, *this, count);
        setCount(getCount()-count);
        return item;
    }
}