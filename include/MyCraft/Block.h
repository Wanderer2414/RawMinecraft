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

    bool isLightSource(const BlockCatogary& type);
    bool isValid(const BlockCatogary& type, const unsigned char& plane);
    bool isCollistion(const BlockCatogary& type);
    bool isSpecial(const BlockCatogary& type);
    bool isTransparent(const BlockCatogary& type);
    bool isMultiState(const BlockCatogary& type);
    float getHardness(const BlockCatogary& type);
    float getTransparentConst(const BlockCatogary& type);
    unsigned char getLightIndensity(const BlockCatogary& type);

    BlockCatogary getBrokenResult(const BlockCatogary& type);

    glm::mat4 getState(const BlockCatogary& type, const unsigned char& plane);
    glm::mat4 getSpecialState(const BlockCatogary& type);
}
#endif