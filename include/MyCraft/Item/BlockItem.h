#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H
#include "Item.h"
#include "Text.h"
#include "Texture.h"
namespace MyCraft {
    bool isBlock(const ItemType& type);
    class BlockItem: private MyBase::TextureContainer, private MyBase::Text, public Item {
    public:
        BlockItem(const ItemPacked& package, const ItemType& type, const unsigned char& count);
        ~BlockItem();
        BlockItem(const BlockItem&) = delete;
        BlockItem& operator=(const BlockItem&) const = delete;
        virtual Item*merge(Item*& item)             override;
        virtual glm::vec2 getPosition() const       override;
        void setPosition(const glm::vec2& position) override;
        void draw() const                           override;
        void update()                               override;
        Item*split(const int& count)                override;
    protected:
    private:
        const ItemPacked& __package;
    };
}
#endif