#ifndef ITEM_H
#define ITEM_H
#include "Block.h"
#include "Font.h"
#include "Texture.h"
namespace MyCraft {
    enum class ItemType: unsigned char {
        Air, Dirt, Grass, 
        CobbleStone, Stone, OakLog, 
        OakLeaf,BedRock, Sand, 
        SandStone, Obsidian, DeepSlate, 
        Ice, Podzol, DirtPath, 
        FarmLand, FarmLandHydrad, SmoothStone,
        SmoothStoneSlab, StoneBrick, CoalOre, 
        IronOre, StrippedOakLog, OakPlank,
        SpruceLog, StrippedSpruceLog, SprucePlank,
        BirchLog, StrippedBirchLog, BirchPlank,
        AcaciaLog, StrippedAcaciaLog, AcaciaPlank,
        CraftTable, Furnace, Smoker,
        BlastFurnace, Chest,  Torch, 
        Water, WoodenAxe, StoneAxe,
        IronAxe, GoldenAxe,  DiamondAxe,
        NetheriteAxe, WoodenHoe, StoneHoe,
        IronHoe, GoldenHoe, DiamondHoe,
        NetheriteHoe, WoodenPickaxe, StonePickaxe,
        IronPickaxe, GoldenPickaxe, DiamondPickaxe,
        NetheritePickaxe, WoodenShovel, StoneShovel,
        IronShovel, GoldenShovel, DiamondShovel,
        NetheriteShovel, FlintAndSteel, Shears,
        LeatherCap, ChainmailCap,  IronCap,
        GoldenCap, DiamonCap, NetheriteCap,
        LeatherChestplate, ChainmailChestplate,  IronChestplate,
        GoldenChestplate, DiamonChestplate, NetheriteChestplate,
        LeatherLeggings, ChainmailLeggings,  IronLeggings,
        GoldenLeggings, DiamonLeggings, NetheriteLeggings,
        LeatherBoots, ChainmailBoots,  IronBoots,
        GoldenBoots, DiamonBoots, NetheriteBoots,
        WoodedSword, StoneSword,  IronSword,
        GoldenSword, DiamonSword, NetheriteSword,
        Bow, Bow1,  Bow2,
        Bow3, Arrow,  Stick,
        Coal, IronIngot, GoldenIngot,
        NetheriteIngot
    };
    struct ItemPacked {
        glm::vec2 size;
        MyBase::Font font;
        MyBase::Texture texture;
    };
    
    bool isAdaptive(const ItemType&, const BlockCatogary&);
    bool isShovelAdaptive(const BlockCatogary& block);
    bool isPickaxeAdaptive(const BlockCatogary& block);
    bool isAxeAdaptive(const BlockCatogary& block);
    bool isHandAdaptive(const BlockCatogary& block);
    
    float getPowerness(const ItemType& type);
    float getSharpness(const ItemType& type);
    class Item {
    public:
        Item(const ItemType& type, const unsigned char& count);
        virtual ~Item();
        Item(Item&&)        = delete;
        Item(const Item&)   = delete;
        Item& operator=(const Item&) const = delete; 
        unsigned char   getCount() const;
        void            setCount(const int& count);
        virtual glm::vec2 getPosition() const = 0;
        virtual void    setPosition(const glm::vec2& position) = 0;
        virtual void    draw() const = 0;
        virtual void    update() = 0;
        virtual Item*   merge(Item*& item) = 0;
        virtual Item*   split(const int& count) = 0;
        static Item*    create(const ItemPacked& package, const unsigned char& count, const ItemType& catogary);
        operator const ItemType&() const;
    protected:
    private:
        unsigned char   __count;
        ItemType        __type;
    };
    
}
#endif