#ifndef BLOCK_H
#define BLOCK_H
#include "Global.h"
namespace MyCraft {
    enum BlockCatogary: unsigned char {
        Air, Dirt, Grass, CobbleStone, Stone, OakWood, OakLeaf,
        BedRock, Sand, SandStone, Obsidian, DeepStale, Ice
    };

    bool isTransparent(const BlockCatogary& type);
}
#endif