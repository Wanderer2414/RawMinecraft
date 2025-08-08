#include "InteractiveForm.h"
#include "Block.h"
#include "ControlCenter.h"
#include "CraftingTableUI.h"
#include "MessageBox.h"

namespace MyCraft {

    bool isInteractive(const BlockCatogary& type) {
        if (type == CraftingTable) return true;
        return false;
    }
}