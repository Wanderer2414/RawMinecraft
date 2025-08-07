#include "Bag.h"
#include "Container2D.h"
#include "ControlCenter.h"
#include "GLFW/glfw3.h"
#include "Inventory.h"
#include "Recipe.h"
namespace MyCraft {

    Bag::Bag(ItemTable& table): __items(table), __hoverItem(0), __output(0), __currentRecipe(0) {
        memset(&__recipe[0], 0, sizeof(void*)*9);
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
    Bag::~Bag() {
        for (int i = 0; i<9; i++)
            if (__recipe[i]) delete __recipe[i];
        if (__output) delete __output;
    }
    bool Bag::setHover(const glm::vec2& position) {
        Controller2D::setHover(position);
        glm::vec2 offset = position - getPosition();
        offset /= getSize();
        offset *= glm::vec2(1610, 940);
        if (offset.x>=509 && offset.x<=1601) {
            offset.x -= 509;
            int n = floor(offset.x/110);
            offset.x -= n*110;
            if (offset.x<=102) {
                if (offset.y>=9 && offset.y<=111) {
                    __hoverPosition = {3,n};
                    __chosenTexture.setVisible(true);
                    __chosenTexture.setTextureExportPosition(getToolbarPosition(n));
                    return true;
                }
                else if (offset.y>=139 && offset.y <= 461) {
                    offset.y -= 139;
                    int y = floor(offset.y/110);
                    offset.y -= y*110;
                    if (offset.y<=102) {
                        __hoverPosition = {y, n};
                        __chosenTexture.setVisible(true);
                        __chosenTexture.setTextureExportPosition(getStoragePosition(glm::ivec2(y, n)));
                        return true;
                    }
                }
                else if (offset.y >= 489 && offset.y <= 921) {
                    if (n==0) {
                        offset.y -= 489;
                        int y = floor(offset.y/110);
                        offset.y -= y*110;
                        if (offset.y<=102 && y<4) {
                            __hoverPosition = {-1, y};
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
                        __hoverPosition = {-2, 0};
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
                                __hoverPosition = {-3, (1-y)*3 + (n-5)};
                                __chosenTexture.setVisible(true);
                                __chosenTexture.setTextureExportPosition(getRecipiPosition(glm::ivec2(1-y, n-5)));
                                return true;
                            }
                        }
                        else if (n==8 && offset.y>=729.f && offset.y<=849.f) {
                            __hoverPosition = {-4, 0};
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
    bool Bag::contains(const glm::vec2& position) const {
        return position.x>=__inventoryTexture.getPosition().x && position.x<=__inventoryTexture.getPosition().x + __inventoryTexture.getSize().x
                && position.y>=__inventoryTexture.getPosition().y && position.y <= __inventoryTexture.getPosition().y + __inventoryTexture.getSize().y;
    }
    bool Bag::__mouseDown(GLFWwindow* window) {
        bool is_changed = MyBase::Container2D::__mouseDown(window);
        if (__chosenTexture.isVisible()) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) &&  __hoverItem) {
                if (!getBags(__hoverPosition)) {
                    if (__hoverItem->getCount()==1) {
                        placeBags(__hoverPosition, __hoverItem);
                        __hoverItem->setPosition(getItemPosition(__hoverPosition) + __items.package.size*0.125f);
                        __hoverItem = 0;
                    }
                    else {
                        Item* item = __hoverItem->split(1);
                        placeBags(__hoverPosition, item);
                        item->setPosition(getItemPosition(__hoverPosition) + __items.package.size*0.125f);
                    }
                }
            }
        }
        return is_changed;
    }
    bool Bag::__mouseClicked(GLFWwindow* window) {
        bool is_changed = MyBase::Container2D::__mouseClicked(window);
        if (__chosenTexture.isVisible()) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
                if (!__hoverItem && getBags(__hoverPosition)) {
                    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
                        Item* item = placeBags(__hoverPosition, 0);
                        char same = -1, empty = -1;
                        for (int i = 8; i>=0; i--) {
                            if (__recipe[i] &&*__recipe[i] == *item) same = i;
                            else if (!__recipe[i]) empty = i;
                        }
                        if (same != -1) {;
                            item = getBags({-3, same})->merge(item);
                            placeBags(__hoverPosition, item);
                            __reCheckRecipe();
                        }
                        else if (empty != -1) {
                            placeBags({-3, empty}, item);
                            item->setPosition(getItemPosition({-3, empty}) + __items.package.size*0.125f);
                        }
                    }
                    else {
                        __hoverItem = placeBags(__hoverPosition, 0);
                        __itemOffset = MyBase::ControlCenter::getInstance().getCursorPos(window) - __hoverItem->getPosition();
                    }
                }
                else if (__hoverItem && __hoverPosition.x>=-3) {
                    if (!getBags(__hoverPosition)) {
                        placeBags(__hoverPosition, __hoverItem);
                        __hoverItem->setPosition(getItemPosition(__hoverPosition) + __items.package.size*0.125f);
                        __hoverItem = 0;
                    }
                    else {
                        Item* hover = placeBags(__hoverPosition, 0);
                        hover->merge(__hoverItem);
                        placeBags(__hoverPosition, hover);
                    }
                }
                else if (__hoverPosition.x == -4 && __output) {
                    __hoverItem = __output;
                    __output = 0;
                    __currentRecipe->get(__recipe, __hoverItem->getCount());
                    __itemOffset = MyBase::ControlCenter::getInstance().getCursorPos(window) - getOutputItemPosition();
                    __reCheckRecipe();
                }
            }
            else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
                if (!__hoverItem) {
                    if (getBags(__hoverPosition)) {
                        Item* item = placeBags(__hoverPosition, 0);
                        __hoverItem = item->split(item->getCount()/2);
                        __itemOffset = MyBase::ControlCenter::getInstance().getCursorPos(window) - item->getPosition();
                        placeBags(__hoverPosition, item);
                    }
                }
                else {
                    if (getBags(__hoverPosition)) {
                        if (__hoverItem->getCount()==1) {
                            Item* item = placeBags(__hoverPosition, 0);
                            item->merge(__hoverItem);
                            placeBags(__hoverPosition, item);
                        }
                        else {
                            Item* item = __hoverItem->split(1);
                            Item* hover = placeBags(__hoverPosition, 0);
                            hover->merge(item);
                            placeBags(__hoverPosition, hover);
                        }
                    }
                }
            }
        }
        return is_changed;
    }
    bool Bag::catchEvent(GLFWwindow* window) {
        bool is_changed = MyBase::Container2D::catchEvent(window);
        if (__hoverItem) {
            glm::vec2 position = MyBase::ControlCenter::getInstance().getCursorPos(window) - __itemOffset;
            __hoverItem->setPosition(position);
            is_changed = true;
        }
        return is_changed;
    }

    glm::vec2 Bag::getToolbarPosition(const int& n) const {
        return glm::vec2((509.f + n*110.f)/1610, 9.f/940)*getSize() + getPosition();
    }

    glm::vec2 Bag::getStoragePosition(const glm::ivec2& offset) const {
        return glm::vec2((509.f + offset.y*110.f)/1610, (139.f+110.f*offset.x)/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getHelmetPosition() const {
        return glm::vec2(509.f/1610, 819.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getChestplatePosition() const {
        return glm::vec2(509.f/1610, 709.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getLeggingsPosition() const {
        return glm::vec2(509.f/1610, 599.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getBootsPosition() const {
        return glm::vec2(509.f/1610, 489.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getRecipiPosition(const glm::ivec2& offset) const {
        return glm::vec2((1059.f + offset.y*110.f)/1610, (729.f-110.f*offset.x)/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getLeftHandPosition() const {
        return glm::vec2((509.f+110.f*4)/1610,489.f/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getOutputItemPosition() const {
        return glm::vec2(1379.f/1610, (619.f+110.f)/940)*getSize() + getPosition();
    }
    glm::vec2 Bag::getItemPosition(const glm::ivec2& position) const {
        if (position.x == 3) return getToolbarPosition(position.y);
        if (position.x >= 0) return getStoragePosition(position);
        if (position.x == -3) return getRecipiPosition({position.y/3, position.y%3});
        if (position.x == -4) return getOutputItemPosition();
        return {0,0};
    }
    Item* Bag::getBags(const glm::ivec2& position) const {
        if (position.x>=0) return __items.getBags(position);
        else if (position.x == -3) return __recipe[position.y];
        return 0;
    }
    Item* Bag::placeBags(const glm::ivec2& position, Item* item) {
        if (position.x>=0) return __items.placeBags(position, item);
        else if (position.x==-3) {
            Item* out = __recipe[position.y];
            __recipe[position.y] = item;
            __reCheckRecipe();
            return out;
        }
        return item;
    }
    void Bag::glDraw() const {
        MyBase::Container2D::glDraw();
        for (glm::ivec2 ofs(0,0); ofs.x<=3; ofs.x++)
            for (ofs.y=0; ofs.y<10; ofs.y++) 
                if (__items.getBags(ofs)) 
                    __items.getBags(ofs)->draw();
        for (int i = 0; i<9; i++) 
            if (__recipe[i]) __recipe[i]->draw();
        if (__output) __output->draw();
        if (__hoverItem) __hoverItem->draw();
    }
    glm::vec2 Bag::getPosition() const {
        return __inventoryTexture.getPosition();
    }
    glm::vec2 Bag::getSize() const {
        return __inventoryTexture.getSize();
    }

    void Bag::update() {
        // if (__hoverItem) {
        //     __hoverItem = false;
        //     glm::vec2 position = getItemPosition(__chosenPosition) + __items.package.size*0.125f;
        //     __items.getBags(__chosenPosition)->setPosition(position);
        // }
        for (int i = 0; i<10; i++) 
            if (__items.getToolBar(i)) 
                __items.getToolBar(i)->setPosition(getToolbarPosition(i) + __items.package.size*0.125f);
    }
    void Bag::__reCheckRecipe() {
        if (__currentRecipe) {
            delete __currentRecipe;
            __currentRecipe = 0;
        }
        if (__output) {
            delete __output;
            __output = 0;
        }
        __currentRecipe = RecipeCreator::getItem(__recipe);
    
        if (__currentRecipe) {
            __output = __currentRecipe->getMaxResult(__items.package, __recipe, 64);
            __output->setPosition(getOutputItemPosition() + __items.package.size*0.125f);
        }
    }
}