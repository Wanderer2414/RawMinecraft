#ifndef INVENTORY_H
#define INVENTORY_H
#include "Container2D.h"
#include "Controller2D.h"
#include "Item.h"
#include "Texture.h"
namespace MyCraft {
    struct ItemTable {
        ItemTable();
        char indices[4][10];
        std::vector<Item*> items;
    };
    class InventoryComponent: public MyBase::Container2D {
    public:
        InventoryComponent() = default;
        virtual ~InventoryComponent();
        virtual bool setHover(const glm::vec2& position)    override = 0;
        virtual void glDraw() const                 override = 0;
        virtual glm::vec2 getPosition() const       override = 0;
        virtual glm::vec2 getSize() const           override = 0;
        bool contains(const glm::vec2& position) const override;
    };
    class Bags: public InventoryComponent {
    public:
        Bags(ItemTable& table, const ItemPacked& package);
        ~Bags();
        bool setHover(const glm::vec2& position)    override;
        void glDraw() const                         override;
        glm::vec2 getPosition() const               override;
        glm::vec2 getSize() const                   override;
    private:
        glm::vec2 getHelmetPosition() const;
        glm::vec2 getChestplatePosition() const;
        glm::vec2 getLeggingsPosition() const;
        glm::vec2 getBootsPosition() const;
        glm::vec2 getLeftHandPosition() const;
        glm::vec2 getRecipiPosition(const glm::ivec2& offfset) const;
        glm::vec2 getStoragePosition(const glm::ivec2& offset) const;
        glm::vec2 getToolbarPosition(const int& n) const;
        glm::vec2 getOutputItemPosition() const;

        ItemTable& __items;
        MyBase::TextureViewer __chosenTexture, __inventoryTexture;
    };
    class ToolBar: public InventoryComponent {
    public:
        ToolBar(ItemTable& table, const ItemPacked& package);
        ~ToolBar();
        bool setHover(const glm::vec2& position)    override;
        void glDraw() const                         override;
        glm::vec2 getPosition() const               override;
        glm::vec2 getSize() const                   override;
    private:
        ItemTable& __items;
        MyBase::TextureViewer __toolBarTexture;

        glm::vec2 getToolbarPosition(const int& n) const;
    };
    class Inventory: public MyBase::Container2D {
    public:
        Inventory();
        ~Inventory();
        Inventory(const Inventory&) = delete;
        Inventory& operator=(const Inventory&) const = delete; 

        bool setHover(const glm::vec2& position) override;
        glm::vec2 getPosition() const override;
        glm::vec2 getSize() const override;
        void open();
        void close();
    protected:
    private:
        bool contains(const glm::vec2& position) const override;
        void glDraw() const override;
        char __indices[4][10];
        ItemPacked __itemPacked;
        InventoryComponent* __inventoryComponent;
        ItemTable __items;
    };
}
#endif