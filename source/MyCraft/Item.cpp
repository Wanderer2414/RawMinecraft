#include "Item.h"
#include "Texture.h"
namespace MyCraft {
    Item::Item(const ItemPacked& package, const ItemType& type, const int& count): __type(type), __count(count) {
        MyBase::TextureContainer::setTexture(package.texture);
        setTextureImportPosition(glm::vec2(300.f*(int)type/getTexture().getSize().x, 0));
        setTextureImportSize(glm::vec2(300.f, 300.f)/(glm::vec2)getTexture().getSize());
        setTextureExportSize(package.size);

        Text::setFont(package.font);
        Text::setScale({package.size.x/2, package.size.y/2});
        Text::setTextColor(WHITE);
        Text::setText(std::to_string(__count));
    }
    Item::~Item() {}
    void Item::setPosition(const glm::vec2& position) {
        setTextureExportPosition(position);
        Text::setPosition(position + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void Item::setCount(const int& count) {
        __count = count;
        Text::setPosition(getTextureExportPosition() + getTextureExportSize()/2.f - Text::getSize()/2.f);
    }
    void Item::draw() const {
        MyBase::TextureContainer::draw();
        Text::draw();
    }
}