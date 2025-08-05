#include "Block.h"
#include "Global.h"
#include <limits>
namespace MyCraft {
    bool isTransparent(const BlockCatogary& type) {
        switch (type) {
            case OakLeaf: return true;
            default: return false;
        }
        return false;
    }
    float getHardness(const BlockCatogary& type) {
        switch (type) {
            case Dirt: return 100;
            case Grass: return 500;
            case Ice: return 100;
        default: return std::numeric_limits<float>::max();
        }
    }
}