#include "Toolbar.h"
#include "ControlCenter.h"
#include "Inventory.h"
namespace MyCraft {
    ToolBar::ToolBar(ItemTable& table): __items(table), __chosenIndex(0) {
        MyBase::Texture texture = MyBase::Texture("assets/images/Inventory.png");
        __toolBarTexture.setTexture(texture);
        
        __toolBarTexture.setTextureImportPosition({0, 0});
        __toolBarTexture.setTextureImportSize({1110.f/1610, 120.f/1060});
        glm::vec2 size(0.15f/MyBase::ControlCenter::getInstance().GetWindowRatio()*1110.f/120.f, 0.15);
        __toolBarTexture.setTextureExportPosition({-size.x/2, -1});
        __toolBarTexture.setTextureExportSize(size);

        __chosenTexture.setTexture(texture);
        __chosenTexture.setTextureImportPosition({1233.f/1610, 3.f/1060});
        __chosenTexture.setTextureImportSize({114.f/1610, 114.f/1060});
        __chosenTexture.setTextureExportSize(glm::vec2(114.f/940*1.2/MyBase::ControlCenter::getInstance().GetWindowRatio(), 114.f/940*1.2));
        __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(0));

        insert(&__chosenTexture);
        insert(&__toolBarTexture);
        __mouseEllapse.setDuration(100);
    }

    ToolBar::~ToolBar() {}
    bool ToolBar::setHover(const glm::vec2& position) {
        return false;
    }
    bool ToolBar::catchEvent(GLFWwindow* window) {
        bool is_changed = Controller2D::catchEvent(window);
        if (__mouseEllapse.get()) {
            float x = MyBase::ControlCenter::getInstance().getScroll().y;
            if (x<0) {
                __mouseEllapse.restart();
                __chosenIndex = (__chosenIndex+1)%10;
                __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(__chosenIndex));
                is_changed = true;
            }
            else if (x>0) {
                __mouseEllapse.restart();
                __chosenIndex = (__chosenIndex+9)%10;
                __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(__chosenIndex));
                is_changed = true;
            }
        }
        if (MyBase::ControlCenter::getInstance().IsKeyPressed()) {
            char c = MyBase::ControlCenter::getInstance().GetCharInput() - '0';
            if (c>=0 && c<=9) {
                __chosenIndex = (c+9)%10;
                __chosenTexture.setTextureExportPosition(getToolbarChosenPosition(__chosenIndex));
                is_changed = true;
            }
        }
        return is_changed;
    }
    void ToolBar::glDraw() const {
        MyBase::Container2D::glDraw();
        for (int i = 0; i<10; i++)
            if (__items.getToolBar(i)) __items.getToolBar(i)->draw();
    }
    glm::vec2 ToolBar::getPosition() const {
        return __toolBarTexture.getPosition();
    };
    glm::vec2 ToolBar::getSize() const {
        return __toolBarTexture.getSize();
    };
    glm::vec2 ToolBar::getToolbarPosition(const int& n) const {
        return getPosition() + glm::vec2((9.f + 110.f*n)/1110, 9.f/120)*getSize();
    }
    glm::vec2 ToolBar::getToolbarChosenPosition(const int& n) const {
        return getPosition() + glm::vec2((3.f + 110.f*n)/1110, 3.f/120)*getSize();
    }

    bool ToolBar::contains(const glm::vec2& position) const {
        return false;
    }
    void ToolBar::update() {
        for (int i = 0; i<10; i++) if (__items.getToolBar(i)) 
            __items.getToolBar(i)->setPosition(getToolbarPosition(i) + __items.package.size*0.125f);
    }
}