#include "Block.h"
#include "Global.h"
namespace MyCraft {
    bool isTransparent(const BlockCatogary& type) {
        switch (type) {
            case OakLeaf: return true;
            default: return false;
        }
        return false;
    }
}