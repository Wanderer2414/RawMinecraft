#include "Item.h"
#include "Block.h"
#include "Texture.h"
namespace MyCraft {
    Item::Item(const ItemPacked& package, const ItemType& type, const int& count): __type(type), __count(count) {
        MyBase::TextureContainer::setTexture(package.texture);
        int index= int(type);
        setTextureImportPosition(glm::vec2(160.f*(index%3)/getTexture().getSize().x, 1-160.f*int(index/3+1)/getTexture().getSize().y));
        setTextureImportSize(glm::vec2(160.f, 160.f)/(glm::vec2)getTexture().getSize());
        setTextureExportSize(package.size);

        Text::setFont(package.font);
        Text::setScale({package.size.x/2, package.size.y/2});
        Text::setTextColor(WHITE);
        Text::setText(std::to_string(__count));
    }
    Item::~Item() {}
    bool isBlock(const ItemType& type) {
        if (type>=ItemType::Dirt && type<=ItemType::Ice) return true;
        return false;
    }
    Item::operator const ItemType&() const {
        return __type;
    }
    Item::operator BlockCatogary() const {
        return (BlockCatogary)__type;
    }
    int Item::getCount() const {
        return __count;
    }
    void Item::setPosition(const glm::vec2& position) {
        setTextureExportPosition(position);
        Text::setPosition(position + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void Item::setCount(const int& count) {
        __count = count;
        Text::setText(std::to_string(__count));
        Text::setPosition(getTextureExportPosition() + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void Item::draw() const {
        MyBase::TextureContainer::draw();
        Text::draw();
    }
}