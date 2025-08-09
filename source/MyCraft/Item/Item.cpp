#include "Item.h"
#include "Block.h"
#include "BlockItem.h"
#include "NonuseItem.h"
#include "ToolItem.h"
namespace MyCraft {
    bool isAdaptive(const ItemType& tool, const BlockCatogary& block) {
        if (tool == ItemType::Air || tool <= ItemType::Chest)
            return isHandAdaptive(block);

        if (tool >= ItemType::WoodenShovel && tool <= ItemType::NetheriteShovel)
            return isShovelAdaptive(block);

        if (tool >= ItemType::WoodenPickaxe && tool <= ItemType::NetheritePickaxe)
            return isPickaxeAdaptive(block);

        if (tool >= ItemType::WoodenAxe && tool <= ItemType::NetheriteAxe)
            return isAxeAdaptive(block);

        return false;
    }

    bool isHandAdaptive(const BlockCatogary& block) {
        switch (block) {
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
        if (tool == ItemType::WoodenAxe) return 30;
        if (tool == ItemType::StoneAxe) return 40;
        if (tool == ItemType::IronAxe) return 60;
        if (tool == ItemType::GoldenAxe) return 80;
        if (tool == ItemType::DiamonAxe) return 70;
        if (tool == ItemType::NetheriteAxe) return 90;

        if (tool == ItemType::WoodenPickaxe) return 30;
        if (tool == ItemType::StonePickaxe) return 40;
        if (tool == ItemType::IronPickaxe) return 60;
        if (tool == ItemType::GoldenPickaxe) return 80;
        if (tool == ItemType::DianmonPickaxe) return 70;
        if (tool == ItemType::NetheritePickaxe) return 90;

        if (tool == ItemType::WoodenShovel) return 30;
        if (tool == ItemType::StoneShovel) return 40;
        if (tool == ItemType::IronShovel) return 60;
        if (tool == ItemType::GoldenShovel) return 80;
        if (tool == ItemType::DiamonShovel) return 70;
        if (tool == ItemType::NetheriteShovel) return 90;

        if (tool == ItemType::WoodenHoe) return 30;
        if (tool == ItemType::StoneHoe) return 40;
        if (tool == ItemType::IronHoe) return 60;
        if (tool == ItemType::GlodenHoe) return 80;
        if (tool == ItemType::DiamonHoe) return 70;
        if (tool == ItemType::NetheriteHoe) return 90;

        if (tool == ItemType::WoodedSword) return 10;
        if (tool == ItemType::StoneSword) return 20;
        if (tool == ItemType::IronSword) return 40;
        if (tool == ItemType::GoldenSword) return 80;
        if (tool == ItemType::DiamonSword) return 72;
        if (tool == ItemType::NetheriteSword) return 100;

        return 20;
    }
    BlockCatogary getBrokenResult(const BlockCatogary& block) {
        // Blocks that break into Dirt
        if (block == Grass) return Dirt;
        if (block == Podzol) return Dirt;
        if (block == DirtPath) return Dirt;
        if (block == FarmLand) return Dirt;
        if (block == FarmLandHydrad) return Dirt;

        // Ice breaks into nothing (non-recoverable without Silk Touch)
        if (block == Ice) return Air;

        // Leaves break into nothing (chance to drop saplings, but no block)
        if (block == OakLeaf) return Air;

        // Slabs drop as full block item (assuming no stacking)
        if (block == SmoothStoneSlab) return SmoothStoneSlab;

        // Logs, planks, stripped logs drop themselves
        if (
            block == OakLog || block == StrippedOakLog || block == OakPlank ||
            block == SpruceLog || block == StrippedSpruceLog || block == SprucePlank ||
            block == BirchLog || block == StrippedBirchLog || block == BirchPlank ||
            block == AcaciaLog || block == StrippedAcaciaLog || block == AcaciaPlank
        ) return block;

        // Stone turns into cobblestone
        if (block == Stone) return CobbleStone;

        // SmoothStone breaks into cobblestone (if not Silk Touch)
        if (block == SmoothStone) return CobbleStone;

        // Stone variants return themselves
        if (block == CobbleStone || block == StoneBrick || block == SandStone) return block;

        // Ores drop nothing without correct tool (simulate as breaking to Air)
        if (block == CoalOre || block == IronOre) return Air;

        // Obsidian drops nothing unless mined correctly
        if (block == Obsidian) return Air;

        // DeepSlate simulates dropping cobbled variant
        if (block == DeepSlate) return CobbleStone;

        // Sand breaks to itself
        if (block == Sand) return Sand;

        // Functional blocks drop themselves
        if (
            block == CraftingTable || block == Furnace ||
            block == Smoker || block == BlastFurnace ||
            block == Chest
        ) return block;

        // Air returns Air (no item)
        if (block == Air) return Air;

        // Everything else: assume it drops itself
        return block;
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