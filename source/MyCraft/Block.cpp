#include "Block.h"
#include "Global.h"
namespace MyCraft {
    bool isLightSource(const BlockCatogary &type) {
        switch (type) {
            case Torch: return true;
            default: return false;
        }
    }
    bool isSpecial(const BlockCatogary& type) {
        switch (type) {
            case Torch:
            case OakPlank: return true;
            default: return false;
        }
        return false;
    }
    bool isCollistion(const BlockCatogary& type) {
        switch (type) {
            case Torch: return false;
            case Air: return false;
            default: return true;
        }
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
    bool isMultiState(const BlockCatogary& type) {
        switch (type) {
            case Torch: return true;
            default: return false;
        }
    }
    bool isValid(const BlockCatogary& type, const unsigned char& plane) {
        switch (type) {
            case Torch: {
                if (plane == 5) return false;
                return true;
            }
            default: return true;
        }
    }
    float getTransparentConst(const BlockCatogary& type) {
        switch (type) {
            case OakLeaf: return 0.98;
            default: return 0;
        }
    }
    unsigned char getLightIndensity(const BlockCatogary &type) {
        switch (type) {
            case Torch: return 30;
            default: return 0;
        }
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

    glm::mat4 getState(const BlockCatogary& type, const unsigned char& plane) {
        switch (type) {
            case Torch: {
                switch (plane) {
                    case 0: return glm::mat4({1,0,0,0}, {0,1,0,0},{0,0,1,0},{0.4,0.4,0,1});
                    case 1: return glm::mat4({cos(M_PI/6), 0, -sin(M_PI/6),0}, {0,1,0,0},{sin(M_PI/6),0,cos(M_PI/6),0},{-0.1,0.4,0.3,1});
                    case 2: return glm::mat4({1, 0, 0 ,0}, {0, cos(M_PI/6), -sin(M_PI/6),0}, {0, sin(M_PI/6),cos(M_PI/6),0} ,{0.4,-0.1,0.3,1});
                    case 3: return glm::mat4({1, 0, 0 ,0}, {0, cos(M_PI/6), sin(M_PI/6),0}, {0, -sin(M_PI/6),cos(M_PI/6),0} ,{0.4,0.9,0.3,1});
                    case 4: return glm::mat4({cos(M_PI/6), 0, sin(M_PI/6),0}, {0,1,0,0},{-sin(M_PI/6),0,cos(M_PI/6),0},{0.9,0.4,0.3,1});
                    default: return glm::mat4(1);
                }
            }
            default: return glm::mat4(1);
        }
    }

    glm::mat4 getSpecialState(const BlockCatogary& type) {
        switch (type) {
            case Torch: return glm::mat4({0.2,0,0,0}, {0,0.2, 0,0},{0,0,0.75,0},{0,0,0,1});
            case OakPlank: return glm::mat4({1,0,0,0}, {0,1, 0,0},{0,0,0.5,0},{0,0,0,1});
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