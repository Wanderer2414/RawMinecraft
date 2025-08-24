#ifndef NON_USE_ITEM_H
#define NON_USE_ITEM_H
#include "Item.h"
#include "Text.h"
#include "Texture.h"
namespace MyCraft {
    bool isNonuseItem(const ItemType& type);
    class NonuseItem: private MyBase::TextureContainer, private MyBase::Text, public Item {
    public:
        NonuseItem(const ItemPacked& package, const ItemType& type, const unsigned char& count);
        ~NonuseItem();
        NonuseItem(const NonuseItem&) = delete;
        NonuseItem& operator=(const NonuseItem&) const = delete;
        virtual Item* merge(Item*& item)             override;
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