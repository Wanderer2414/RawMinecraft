#include "Block.h"
#include "Global.h"
#include <limits>
namespace MyCraft {
    bool isSpecialBlock(const BlockCatogary& type) {
        switch (type) {
            case Torch: return true;
            default: return false;
        }
        return false;
    }
    bool isTransparent(const BlockCatogary& type) {
        switch (type) {
            case Air: return true;
            case OakLeaf: return true;
            case Torch: return true;
            default: return false;
        }
        return false;
    }

    glm::mat4 getSpecialBlockState(const BlockCatogary& type) {
        switch (type) {
            case Torch: return glm::mat4({0.2,0,0,0}, {0,0.2, 0,0},{0,0,0.75,0},{0.4,0.4,0,1});
            default: return glm::mat4(1);
        }
    }

    float getHardness(const BlockCatogary& type) {
        switch (type) {
            case Torch: return 10;
            case Dirt: return 100;
            case Grass: return 100;
            case Sand: return 100;
            case Podzol: return 100;
            case FarmLand: return 100;
            case FarmLandHydrad: return 100;
            case Ice: return 100;

            case DirtPath: return 130;
            case SandStone: return 160;

            case OakLeaf: return 40;

            case Stone: return 300;
            case StoneBrick: return 300;

            case CobbleStone: return 400;
            case SmoothStone: return 400;
            case SmoothStoneSlab: return 400;

            case OakLog: return 400;
            case StrippedOakLog: return 400;
            case OakPlank: return 400;

            case SpruceLog: return 400;
            case StrippedSpruceLog: return 400;
            case SprucePlank: return 400;

            case BirchLog: return 400;
            case StrippedBirchLog: return 400;
            case BirchPlank: return 400;

            case AcaciaLog: return 400;
            case StrippedAcaciaLog: return 400;
            case AcaciaPlank: return 400;

            case CraftingTable: return 500;
            case Chest: return 500;

            case CoalOre: return 600;
            case IronOre: return 600;
            case DeepSlate: return 600;

            case Furnace: return 700;
            case Smoker: return 700;
            case BlastFurnace: return 700;

            case Obsidian: return 10000;

            case BedRock: return std::numeric_limits<float>::max();

            default: return std::numeric_limits<float>::max();
        }
    }
}