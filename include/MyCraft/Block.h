#ifndef BLOCK_H
#define BLOCK_H
#include "Global.h"
namespace MyCraft {
    enum BlockCatogary: unsigned char {
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
        CraftingTable, Furnace, Smoker,
        BlastFurnace, Chest, Torch
    };

    bool isSpecialBlock(const BlockCatogary& type);
    bool isTransparent(const BlockCatogary& type);
    float getHardness(const BlockCatogary& type);
    glm::mat4 getSpecialBlockState(const BlockCatogary& type);
}
#endif