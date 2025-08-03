#include "Toolbal.h"
#include "ControlCenter.h"
#include "Texture.h"
#include "TextureStorage.h"
namespace MyCraft {
    ToolBar::ToolBar() {
        __texture = MyBase::TextureStorage::getInstance().getTexture("assets/images/Inventory.png");
        MyBase::TextureContainer::setTexture(__texture);
        MyBase::TextureContainer::setTextureImportPosition({0, 0});
        MyBase::TextureContainer::setTextureImportSize({1110.f/1610, 120.f/1060});
        __size.y = 0.15;
        __size.x = __size.y/MyBase::ControlCenter::getInstance().GetWindowRatio()*1110.f/120.f;;
        __position = {-__size.x/2, -1};
        MyBase::TextureContainer::setTextureExportPosition(__position);
        MyBase::TextureContainer::setTextureExportSize(__size);

    }
    ToolBar::~ToolBar() {
        MyBase::TextureStorage::getInstance().removeTexture("assets/images/Inventory.png");
    }
    glm::vec2 ToolBar::getPosition() const {
        return __position;
    }
    glm::vec2 ToolBar::getSize() const {
        return __size;
    }

    bool ToolBar::contains(const glm::vec2& position) const {
        return false;
    }
    void ToolBar::glDraw() const {
        MyBase::TextureContainer::draw();
    }
}