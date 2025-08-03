#include "Inventory.h"
#include "ControlCenter.h"
#include "Texture.h"
namespace MyCraft {
    Inventory::Inventory() {
        __inventoryTexture.setTexture("assets/images/Inventory.png");
        __inventoryTexture.setTextureImportPosition({0, 120.f/1060});
        __inventoryTexture.setTextureImportSize({1, 940.f/1060});
        glm::vec2 size(1610.f/940.f*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 1.2);
        __inventoryTexture.setTextureExportPosition(-size/2.f);
        __inventoryTexture.setTextureExportSize(size);

        __chosenTexture.setTexture("assets/images/Inventory.png");
        __chosenTexture.setTextureImportPosition({1119.f/1610,9.f/1060});
        __chosenTexture.setTextureImportSize({102.f/1610, 102.f/1060});
        __chosenTexture.setTextureExportSize(glm::vec2(102.f/1610, 102.f/940)*size);
        __chosenTexture.setVisible(false);
        insert(&__inventoryTexture, 1);
        insert(&__chosenTexture, 0);
    }
    Inventory::~Inventory() {}
    bool Inventory::setHover(const glm::vec2& position) {
        glm::vec2 offset = position - getPosition();
        offset /= getSize();
        offset *= glm::vec2(1610, 940);
        if (offset.x>=509 && offset.x<=1601) {
            offset.x -= 509;
            int n = floor(offset.x/110);
            offset.x -= n*110;
            if (offset.x<=102) {
                if (offset.y>=9 && offset.y<=111) {
                    __chosenTexture.setVisible(true);
                    __chosenTexture.setTextureExportPosition(getToolbarPosition(n));
                    return true;
                }
                else if (offset.y>=139 && offset.y <= 461) {
                    offset.y -= 139;
                    int y = floor(offset.y/110);
                    offset.y -= y*110;
                    if (offset.y<=102) {
                        __chosenTexture.setVisible(true);
                        __chosenTexture.setTextureExportPosition(getStoragePosition(glm::ivec2(n, y)));
                        return true;
                    }
                }
                else if (offset.y >= 489 && offset.y <= 921) {
                    if (n==0) {
                        offset.y -= 489;
                        int y = floor(offset.y/110);
                        offset.y -= y*110;
                        if (offset.y<=102 && y<4) {
                            __chosenTexture.setVisible(true);
                            switch (y) {
                                case 3: __chosenTexture.setTextureExportPosition(getHelmetPosition()); break;
                                case 2: __chosenTexture.setTextureExportPosition(getChestplatePosition()); break;
                                case 1: __chosenTexture.setTextureExportPosition(getLeggingsPosition()); break;
                                case 0: __chosenTexture.setTextureExportPosition(getBootsPosition()); break;
                                default: break;
                            }
                            return true;
                        }
                    }
                }
            }
        }
        if (__chosenTexture.isVisible()) {
            __chosenTexture.setVisible(false);
            return true;
        }
        return false;
    }
    glm::vec2 Inventory::getToolbarPosition(const int& n) const {
        return glm::vec2((509.f + n*110.f)/1610, 9.f/940)*getSize() + getPosition();
    }

    glm::vec2 Inventory::getStoragePosition(const glm::ivec2& offset) const {
        return glm::vec2((509.f + offset.x*110.f)/1610, (139.f+110.f*offset.y)/940)*getSize() + getPosition();
    }
    glm::vec2 Inventory::getHelmetPosition() const {
        return glm::vec2(509.f/1610, 819.f/940)*getSize() + getPosition();
    }
    glm::vec2 Inventory::getChestplatePosition() const {
        return glm::vec2(509.f/1610, 709.f/940)*getSize() + getPosition();
    }
    glm::vec2 Inventory::getLeggingsPosition() const {
        return glm::vec2(509.f/1610, 599.f/940)*getSize() + getPosition();
    }
    glm::vec2 Inventory::getBootsPosition() const {
        return glm::vec2(509.f/1610, 489.f/940)*getSize() + getPosition();
    }
    glm::vec2 Inventory::getPosition() const {
        return __inventoryTexture.getPosition();
    }
    glm::vec2 Inventory::getSize() const {
        return __inventoryTexture.getSize();
    }
    bool Inventory::contains(const glm::vec2& position) const {
        return false;
    }
    void Inventory::glDraw() const {
        Container2D::glDraw();
    }
}