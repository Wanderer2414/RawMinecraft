#include "Item.h"
#include "Block.h"
#include "BlockItem.h"
#include "NonuseItem.h"
#include "ToolItem.h"
namespace MyCraft {
    bool isAdaptive(const ItemType& tool, const BlockCatogary& block) {
        if (tool >= ItemType::WoodenShovel && tool <= ItemType::NetheriteShovel)
            return isShovelAdaptive(block);

        if (tool >= ItemType::WoodenPickaxe && tool <= ItemType::NetheritePickaxe)
            return isPickaxeAdaptive(block);

        if (tool >= ItemType::WoodenAxe && tool <= ItemType::NetheriteAxe)
            return isAxeAdaptive(block);

        return isHandAdaptive(block);
    }

    bool isHandAdaptive(const BlockCatogary& block) {
        switch (block) {
            case Torch: return true;
            case Dirt: return true;
            case Sand: return true;
            case Grass: return true;
            case OakLog: return true;
            case AcaciaLog: return true;
            case SpruceLog: return true;
            case BirchLog: return true;
        default: return false;
        }
    }

    bool isShovelAdaptive(const BlockCatogary& block) {
        switch (block) {
            case Dirt:
            case Grass:
            case Sand:
            case Podzol:
            case FarmLand:
            case FarmLandHydrad:
            case Ice:
            case DirtPath:
            case SandStone:
                return true;
            default:
                return false;
        }
    }

    bool isPickaxeAdaptive(const BlockCatogary& block) {
        switch (block) {
            case Stone:
            case CobbleStone:
            case SmoothStone:
            case SmoothStoneSlab:
            case StoneBrick:
            case CoalOre:
            case IronOre:
            case Furnace:
            case Smoker:
            case BlastFurnace:
            case DeepSlate:
            case Obsidian:
                return true;
            default:
                return false;
        }
    }

    bool isAxeAdaptive(const BlockCatogary& block) {
        switch (block) {
            case OakLog:
            case StrippedOakLog:
            case OakPlank:
            case SpruceLog:
            case StrippedSpruceLog:
            case SprucePlank:
            case BirchLog:
            case StrippedBirchLog:
            case BirchPlank:
            case AcaciaLog:
            case StrippedAcaciaLog:
            case AcaciaPlank:
            case CraftingTable:
            case Chest:
                return true;
            default:
                return false;
        }
    }


    float getPowerness(const ItemType& tool) {
        switch (tool) {
            case (ItemType::WoodenAxe): return 30;
            case (ItemType::StoneAxe): return 40;
            case (ItemType::IronAxe): return 60;
            case (ItemType::GoldenAxe): return 80;
            case (ItemType::DiamondAxe): return 70;
            case (ItemType::NetheriteAxe): return 90;
            case (ItemType::WoodenPickaxe): return 30;
            case (ItemType::StonePickaxe): return 40;
            case (ItemType::IronPickaxe): return 60;
            case (ItemType::GoldenPickaxe): return 80;
            case (ItemType::DiamondPickaxe): return 70;
            case (ItemType::NetheritePickaxe): return 90;
            case (ItemType::WoodenShovel): return 30;
            case (ItemType::StoneShovel): return 40;
            case (ItemType::IronShovel): return 60;
            case (ItemType::GoldenShovel): return 80;
            case (ItemType::DiamondShovel): return 70;
            case (ItemType::NetheriteShovel): return 90;
            case (ItemType::WoodenHoe): return 30;
            case (ItemType::StoneHoe): return 40;
            case (ItemType::IronHoe): return 60;
            case (ItemType::GoldenHoe): return 80;
            case (ItemType::DiamondHoe): return 70;
            case (ItemType::NetheriteHoe): return 90;
            case (ItemType::WoodedSword): return 10;
            case (ItemType::StoneSword): return 20;
            case (ItemType::IronSword): return 40;
            case (ItemType::GoldenSword): return 80;
            case (ItemType::DiamonSword): return 72;
            case (ItemType::NetheriteSword): return 100;
            default: return 20;
        }
    }

float getSharpness(const ItemType& tool) {
    switch (tool) {
        case ItemType::WoodenAxe:      return 30;
        case ItemType::StoneAxe:       return 40;
        case ItemType::IronAxe:        return 60;
        case ItemType::GoldenAxe:      return 80;
        case ItemType::DiamondAxe:     return 70;
        case ItemType::NetheriteAxe:   return 90;

        case ItemType::WoodenPickaxe:     return 30;
        case ItemType::StonePickaxe:      return 40;
        case ItemType::IronPickaxe:       return 60;
        case ItemType::GoldenPickaxe:     return 80;
        case ItemType::DiamondPickaxe:    return 70;
        case ItemType::NetheritePickaxe:  return 90;

        case ItemType::WoodenShovel:     return 30;
        case ItemType::StoneShovel:      return 40;
        case ItemType::IronShovel:       return 60;
        case ItemType::GoldenShovel:     return 80;
        case ItemType::DiamondShovel:    return 70;
        case ItemType::NetheriteShovel:  return 90;

        case ItemType::WoodenHoe:     return 30;
        case ItemType::StoneHoe:      return 40;
        case ItemType::IronHoe:       return 60;
        case ItemType::GoldenHoe:     return 80;
        case ItemType::DiamondHoe:    return 70;
        case ItemType::NetheriteHoe:  return 90;

        case ItemType::WoodedSword:     return 10;
        case ItemType::StoneSword:      return 20;
        case ItemType::IronSword:       return 40;
        case ItemType::GoldenSword:     return 80;
        case ItemType::DiamonSword:    return 72;
        case ItemType::NetheriteSword:  return 100;

        default: return 20;
    }
}

    BlockCatogary to_block(const ItemType& type) {
        return (BlockCatogary)type;
    }
    Item* Item::create(const ItemPacked& package, const unsigned char& count, const ItemType& type) {
        Item* item = 0;
        if (MyCraft::isBlock(type)) item = new BlockItem(package, type, count);
        else if (MyCraft::isNonuseItem(type)) item = new NonuseItem(package, type, count);
        else if (MyCraft::isToolItem(type)) item = new ToolItem(package, type);
        return item;
    }
    Item::Item(const ItemType& type, const unsigned char& count):  __type(type), __count(count) {}
    Item::~Item() {}

    Item::operator const ItemType&() const {
        return __type;
    }

    unsigned char Item::getCount() const {
        return __count;
    }
    
    void Item::setCount(const int& count) {
        __count = count;
        update();
    }
}