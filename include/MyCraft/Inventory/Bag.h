#ifndef BAG_H
#define BAG_H
#include "Container2D.h"
#include "Item.h"
#include "Recipe.h"

namespace MyCraft {
    class ItemTable;    
    class Bag: public MyBase::Container2D {
    public:
        Bag(ItemTable& table);
        ~Bag();
        bool setHover(const glm::vec2& position)    override;
        void glDraw() const                         override;
        glm::vec2 getPosition() const               override;
        glm::vec2 getSize() const                   override;
        void update()                               override;
    private:
        bool __mouseClicked(GLFWwindow*)                override;
        bool __mouseDown(GLFWwindow*)                   override;
        bool contains(const glm::vec2& position) const  override;
        bool catchEvent(GLFWwindow* window)             override;
        Item* getBags(const glm::ivec2& position)       const;
        Item* placeBags(const glm::ivec2& position, Item* item);
        glm::vec2 getHelmetPosition() const;
        glm::vec2 getChestplatePosition() const;
        glm::vec2 getLeggingsPosition() const;
        glm::vec2 getBootsPosition() const;
        glm::vec2 getLeftHandPosition() const;
        glm::vec2 getRecipiPosition(const glm::ivec2& offfset) const;
        glm::vec2 getStoragePosition(const glm::ivec2& offset) const;
        glm::vec2 getToolbarPosition(const int& n) const;
        glm::vec2 getOutputItemPosition() const;
        glm::vec2 getItemPosition(const glm::ivec2& offest) const;

        glm::vec2   __itemOffset;
        glm::ivec2  __hoverPosition;
        ItemTable&  __items;
        Item*       __recipe[9];
        Item*       __output, *__hoverItem;
        Recipe      *__currentRecipe;
        MyBase::TextureViewer __chosenTexture, __inventoryTexture;
        void __reCheckRecipe();
    };
}
#endif