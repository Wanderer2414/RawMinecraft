#include "Inventory.h"
#include "Block.h"
#include "Container2D.h"
#include "ControlCenter.h"
#include "Controller2D.h"
#include "Font.h"
#include "Item.h"
#include "Message.h"
#include "PlayerModelController.h"
#include "Texture.h"
#include "World.h"
#include "Wrapper.h"
namespace MyCraft {
    ItemTable::ItemTable() {
        for (int i = 0; i<4; i++)
            for (int j = 0; j<10; j++) indices[i][j] = -1;
    }
    Bags::Bags(ItemTable& table): __items(table) {
        MyBase::Texture texture =  MyBase::Texture("assets/images/Inventory.png");
        __inventoryTexture.setTexture(texture);
        __inventoryTexture.setTextureImportPosition({0, 120.f/1060});
        __inventoryTexture.setTextureImportSize({1, 940.f/1060});
        glm::vec2 size(1610.f/940.f*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 1.2);
        __inventoryTexture.setTextureExportPosition(-size/2.f);
        __inventoryTexture.setTextureExportSize(size);

        __chosenTexture.setTexture(texture);
        __chosenTexture.setTextureImportPosition({1119.f/1610,9.f/1060});
        __chosenTexture.setTextureImportSize({102.f/1610, 102.f/1060});
        __chosenTexture.setTextureExportSize(glm::vec2(102.f/1610, 102.f/940)*size);
        __chosenTexture.setVisible(false);

        insert(&__chosenTexture);
        insert(&__inventoryTexture);

    }
    Bags::~Bags() {}
    bool Bags::setHover(const glm::vec2& position) {
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
                    else if (n==4 && offset.y>=489 && offset.y<=601) {
                        __chosenTexture.setVisible(true);
                        __chosenTexture.setTextureExportPosition(getLeftHandPosition());
                        return true;
                    }
                    else if (offset.y>=619 && offset.y <= 831) {
                        if (n==5 || n==6) {
                            offset.y -= 619;
                            int y = floor(offset.y/110);
                            offset.y -= y*110;
                            if (offset.y<=102) {
                                __chosenTexture.setVisible(true);
                                __chosenTexture.setTextureExportPosition(getRecipiPosition(glm::ivec2(n-5, y)));
                                return true;
                            }
                        }
                        else if (n==8 && offset.y>=729.f && offset.y<=849.f) {
                            __chosenTexture.setVisible(true);
                            __chosenTexture.setTextureExportPosition(getOutputItemPosition());
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
    bool Bags::contains(const glm::vec2& position) const {
        return false;
    }
    glm::vec2 Bags::getToolbarPosition(const int& n) const {
        return glm::vec2((509.f + n*110.f)/1610, 9.f/940)*getSize() + getPosition();
    }

    glm::vec2 Bags::getStoragePosition(const glm::ivec2& offset) const {
        return glm::vec2((509.f + offset.x*110.f)/1610, (139.f+110.f*offset.y)/940)*getSize() + getPosition();
    }
    glm::vec2 Bags::getHelmetPosition() const {
        return glm::vec2(509.f/1610, 819.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bags::getChestplatePosition() const {
        return glm::vec2(509.f/1610, 709.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bags::getLeggingsPosition() const {
        return glm::vec2(509.f/1610, 599.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bags::getBootsPosition() const {
        return glm::vec2(509.f/1610, 489.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bags::getRecipiPosition(const glm::ivec2& offset) const {
        return glm::vec2((1059.f + offset.x*110.f)/1610, (619.f+110.f*offset.y)/940)*getSize() + getPosition();
    }
    glm::vec2 Bags::getLeftHandPosition() const {
        return glm::vec2((509.f+110.f*4)/1610,489.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bags::getOutputItemPosition() const {
        return glm::vec2(1379.f/1610, (619.f+110.f)/940)*getSize() + getPosition();
    }

    void Bags::glDraw() const {
        MyBase::Container2D::glDraw();
        for (int i = 0; i<__items.items.size(); i++) __items.items[i]->draw();
    }
    glm::vec2 Bags::getPosition() const {
        return __inventoryTexture.getPosition();
    }
    glm::vec2 Bags::getSize() const {
        return __inventoryTexture.getSize();
    }

    void Bags::update() {
        for (int i = 0; i<10; i++) if (__items.indices[3][i]>-1) 
            __items.items[__items.indices[3][i]]->setPosition(getToolbarPosition(i) + __items.package.size*0.1f);
    }


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
            if (__items.indices[3][i]>-1) __items.items[__items.indices[3][i]]->draw();
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
        for (int i = 0; i<10; i++) if (__items.indices[3][i]>-1) 
            __items.items[__items.indices[3][i]]->setPosition(getToolbarPosition(i) + __items.package.size*0.1f);
    }


    Inventory::Inventory(): MyBase::Wrapper(0) {
        __items.package.texture = MyBase::Texture("assets/images/blockItem.png");
        __items.package.size = glm::vec2(102.f/940*1.2f/MyBase::ControlCenter::getInstance().GetWindowRatio(), 102.f/940*1.2)*0.8f;
        __items.package.font = MyBase::Font("assets/fonts/SyneMono-Regular.ttf");
        for (int i = 0; i<4; i++) for (int j = 0; j<10; j++) __indices[i][j] = -1;

        __items.indices[3][0] = 0;
        __items.items.push_back(new Item(__items.package, ItemType::Grass, 64));

        __items.indices[3][1] = 1;
        __items.items.push_back(new Item(__items.package, ItemType::Ice, 64));

        __bags = new Bags(__items);
        __toolBar = new ToolBar(__items);
        add(new LeftAttackCommand(__toolBar));
        add(new AcceptPlaceCommand(__toolBar));
        changeState(__toolBar);
    }
    Inventory::~Inventory() {
        delete __bags;
        delete __toolBar;
        for (int i = 0; i<__items.items.size(); i++) delete __items.items[i];
    }

    void Inventory::open() {
        changeState(__bags);
    }
    void Inventory::close() {
        changeState(__toolBar);
    }

    LeftAttackCommand::LeftAttackCommand(ToolBar* toolBar): __toolBar(toolBar) {}
    LeftAttackCommand::~LeftAttackCommand() {}

    MyBase::MessageType LeftAttackCommand::getType() const {
        return MyBase::LeftAttack;
    }
    void LeftAttackCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        LeftAttackMessage* package = (LeftAttackMessage*)message;
        int index = __toolBar->__items.indices[3][__toolBar->__chosenIndex];
        if (index!=-1) {
            if (isBlock(*__toolBar->__items.items[index])) {
                mine.send(new PlaceBlockMessage(package->shape, *__toolBar->__items.items[index]));
            }
        }
    }

    AcceptPlaceMessage::AcceptPlaceMessage(const BlockCatogary& t): type(t) {}
    AcceptPlaceMessage::~AcceptPlaceMessage() {}

    MyBase::MessageType AcceptPlaceMessage::getType() const {
        return MyBase::AcceptPlace;
    }

    AcceptPlaceCommand::AcceptPlaceCommand(ToolBar* toolbar): __toolbar(toolbar) {}
    AcceptPlaceCommand::~AcceptPlaceCommand() {}

    MyBase::MessageType AcceptPlaceCommand::getType() const {
        return MyBase::AcceptPlace;
    }
    void AcceptPlaceCommand::execute(MyBase::Port& mine, MyBase::Port& source, MyBase::Message* message) {
        AcceptPlaceMessage* package = (AcceptPlaceMessage*)message;
        int count = __toolbar->__items.items[__toolbar->__chosenIndex]->getCount();
        if (count) {
            __toolbar->__items.items[__toolbar->__chosenIndex]->setCount(count-1);
        }
    }

}