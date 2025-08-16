#ifndef CRAFTING_TABLE_H
#define CRAFTING_TABLE_H
#include "Controller2D.h"
#include "Inventory.h"
#include "InventoryElement.h"
#include "Message.h"
#include "Recipe.h"
#include "Texture.h"

namespace MyCraft {
    class CraftingTableUI: public InventoryUI, public MyBase::Port {
    public:
        CraftingTableUI(ItemTable* table);
        ~CraftingTableUI();
        CraftingTableUI(const CraftingTableUI&) = delete;
        CraftingTableUI& operator=(const CraftingTableUI&) const = delete; 

        bool setHover(const glm::vec2& position) override;
        glm::vec2 getSize() const override;
        glm::vec2 getPosition() const override;
        void update() override;
        void close() override;
        void open() override;
    protected:
    private:   
        glm::vec2 __itemOffset;
        glm::ivec2 __hoverPosition;
        Item* __hoverItem;
        ItemTable& __items;
        MyBase::TextureViewer __chosenTexture, __inventoryTexture;
        bool catchEvent(GLFWwindow*) override;
        bool __mouseClicked(GLFWwindow*) override;
        bool __mouseDown(GLFWwindow*) override;
        bool contains(const glm::vec2& position) const override;
        void glDraw() const override;
        Recipe *__currentRecipe;
        Item* __recipe[9], *__output;
        Item* getBags(const glm::ivec2& offset) const;
        Item* placeBags(const glm::ivec2& offset, Item*);
        void    __reCheckRecipe();
        glm::vec2 getItemPosition(const glm::ivec2& offset) const;
        glm::vec2 getStoragePosition(const glm::ivec2& offset) const;
        glm::vec2 getRecipiPosition(const glm::ivec2& offset) const;
        glm::vec2 getToolbarPosition(const int& n) const;
        glm::vec2 getOutputItemPosition() const;
    };
}
#endif