#include <fstream>

enum class ItemType: unsigned char {
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
    CraftTable, Furnace, Smoker,
    BlastFurnace, Chest, WoodedAxe,
    StoneAxe, IronAxe, GoldenAxe, 
    DiamonAxe, NetheriteAxe, WoodenHoe,
    StoneHoe, IronHoe, GoldenHoe,
    DiamonHoe, NetheriteHoe, WoodPickaxe,
    StonePickaxe, IronPickaxe, GoldenPickaxe,
    DianmonPickaxe, NetheritePickaxe, WoodenShovel,
    StoneShovel, IronShovel, GoldenShovel,
    DiamonShovel, NetheriteShovel, FlintAndSteel,
    Shears, LeatherCap, ChainmailCap, 
    IronCap, GoldenCap, DiamonCap,
    NetheriteCap, LeatherChestplate, ChainmailChestplate, 
    IronChestplate, GoldenChestplate, DiamonChestplate,
    NetheriteChestplate,LeatherLeggings, ChainmailLeggings, 
    IronLeggings, GoldenLeggings, DiamonLeggings,
    NetheriteLeggings, LeatherBoots, ChainmailBoots, 
    IronBoots, GoldenBoots, DiamonBoots,
    NetheriteBoots, WoodedSword, StoneSword, 
    IronSword, GoldenSword, DiamonSword,
    NetheriteSword, Bow, Bow1, 
    Bow2, Bow3, Arrow, 
    Stick, Coal, IronIngot,
    GoldenIngot, NetheriteIngot
};
#include <vector>
void write(std::ofstream& file, std::vector<std::pair<unsigned char, ItemType>>& type, const std::pair<unsigned char,ItemType>& output) {
    unsigned char size = type.size();
    file.write((char*)&size, sizeof(char));
    file.write((char*)type.data(), size*(sizeof(ItemType)+sizeof(char)));
    file.write((char*)&output, sizeof(ItemType) + sizeof(char));
}
void Pickaxe(std::ofstream& file) {
    {
        // Wooden Pickaxe
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::OakPlank}, {1, ItemType::OakPlank}, {1, ItemType::OakPlank},
            {0, ItemType::Air},      {1, ItemType::Stick},    {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::WoodPickaxe});
    }

    {   
        // Stone Pickaxe
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone},
            {0, ItemType::Air},         {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::StonePickaxe});
    }
    {
        // Iron Pickaxe
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::IronIngot}, {1, ItemType::IronIngot}, {1, ItemType::IronIngot},
            {0, ItemType::Air},       {1, ItemType::Stick},     {0, ItemType::Air},
            {0, ItemType::Air},       {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::IronPickaxe});
    }
    {
        // Golden Pickaxe
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::GoldenIngot}, {1, ItemType::GoldenIngot}, {1, ItemType::GoldenIngot},
            {0, ItemType::Air},         {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::GoldenPickaxe});
    }
    {
        // Netherite Pickaxe
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::NetheriteIngot}, {1, ItemType::NetheriteIngot}, {1, ItemType::NetheriteIngot},
            {0, ItemType::Air},            {1, ItemType::Stick},          {0, ItemType::Air},
            {0, ItemType::Air},            {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::NetheritePickaxe});
    }
}
void Axe(std::ofstream& file) {// Wooden Axe
    // Wooden Axe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::OakPlank}, {1, ItemType::OakPlank}, {0, ItemType::Air},
            {1, ItemType::OakPlank}, {1, ItemType::Stick},    {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::WoodedAxe});
    }

    // Stone Axe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone}, {0, ItemType::Air},
            {1, ItemType::CobbleStone}, {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::StoneAxe});
    }

    // Iron Axe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::IronIngot}, {1, ItemType::IronIngot}, {0, ItemType::Air},
            {1, ItemType::IronIngot}, {1, ItemType::Stick},     {0, ItemType::Air},
            {0, ItemType::Air},       {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::IronAxe});
    }

    // Golden Axe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::GoldenIngot}, {1, ItemType::GoldenIngot}, {0, ItemType::Air},
            {1, ItemType::GoldenIngot}, {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::GoldenAxe});
    }

    // Netherite Axe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::NetheriteIngot}, {1, ItemType::NetheriteIngot}, {0, ItemType::Air},
            {1, ItemType::NetheriteIngot}, {1, ItemType::Stick},          {0, ItemType::Air},
            {0, ItemType::Air},            {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::NetheriteAxe});
    }

}
void Shovel(std::ofstream& file) {
    // Wooden Shovel
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::OakPlank}, {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick},    {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::WoodenShovel});
    }

    // Stone Shovel
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
                  {1, ItemType::CobbleStone}, {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::StoneShovel});
    }

    // Iron Shovel
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::IronIngot}, {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick},     {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::IronShovel});
    }

    // Golden Shovel
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::GoldenIngot}, {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::GoldenShovel});
    }

    // Netherite Shovel
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::NetheriteIngot}, {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick},          {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::NetheriteShovel});
    }

}
void Hoe(std::ofstream& file) {
    // Wooden Hoe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::OakPlank}, {1, ItemType::OakPlank}, {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick},    {0, ItemType::Air},
            {0, ItemType::Air},      {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::WoodenHoe});
    }

    // Stone Hoe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone}, {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::StoneHoe});
    }

    // Iron Hoe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::IronIngot}, {1, ItemType::IronIngot}, {0, ItemType::Air},
            {0, ItemType::Air},       {1, ItemType::Stick},     {0, ItemType::Air},
            {0, ItemType::Air},       {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::IronHoe});
    }

    // Golden Hoe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::GoldenIngot}, {1, ItemType::GoldenIngot}, {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick},       {0, ItemType::Air},
            {0, ItemType::Air},         {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::GoldenHoe});
    }

    // Netherite Hoe
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::NetheriteIngot}, {1, ItemType::NetheriteIngot}, {0, ItemType::Air},
            {0, ItemType::Air},            {1, ItemType::Stick},          {0, ItemType::Air},
            {0, ItemType::Air},            {1, ItemType::Stick}
        };
        write(file, items, {1, ItemType::NetheriteHoe});
    }
}
void CraftingTable(std::ofstream& file) {// Oak Crafting Table
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::OakPlank}, {1, ItemType::OakPlank}, {0, ItemType::Air},
            {1, ItemType::OakPlank}, {1, ItemType::OakPlank}
        };
        write(file, items, {1, ItemType::CraftTable});
    }

    // Spruce Crafting Table
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::SprucePlank}, {1, ItemType::SprucePlank}, {0, ItemType::Air},
            {1, ItemType::SprucePlank}, {1, ItemType::SprucePlank}
        };
        write(file, items, {1, ItemType::CraftTable});
    }

    // Birch Crafting Table
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::BirchPlank}, {1, ItemType::BirchPlank}, {0, ItemType::Air},
            {1, ItemType::BirchPlank}, {1, ItemType::BirchPlank}
        };
        write(file, items, {1, ItemType::CraftTable});
    }

    // Acacia Crafting Table
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::AcaciaPlank}, {1, ItemType::AcaciaPlank}, {0, ItemType::Air},
            {1, ItemType::AcaciaPlank}, {1, ItemType::AcaciaPlank}
        };
        write(file, items, {1, ItemType::CraftTable});
    }

}
void Furnace(std::ofstream& file) {
    // Furnace
    std::vector<std::pair<unsigned char, ItemType>> items = {
        {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone},
        {1, ItemType::CobbleStone}, {0, ItemType::Air},         {1, ItemType::CobbleStone},
        {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone}, {1, ItemType::CobbleStone}
    };
    write(file, items, {1, ItemType::Furnace});
}
void Chest(std::ofstream& file) {
    // Oak Chest
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::OakPlank}, {1, ItemType::OakPlank}, {1, ItemType::OakPlank},
            {1, ItemType::OakPlank}, {0, ItemType::Air},      {1, ItemType::OakPlank},
            {1, ItemType::OakPlank}, {1, ItemType::OakPlank}, {1, ItemType::OakPlank}
        };
        write(file, items, {1, ItemType::Chest});
    }

    // Spruce Chest
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::SprucePlank}, {1, ItemType::SprucePlank}, {1, ItemType::SprucePlank},
            {1, ItemType::SprucePlank}, {0, ItemType::Air},         {1, ItemType::SprucePlank},
            {1, ItemType::SprucePlank}, {1, ItemType::SprucePlank}, {1, ItemType::SprucePlank}
        };
        write(file, items, {1, ItemType::Chest});
    }

    // Birch Chest
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::BirchPlank}, {1, ItemType::BirchPlank}, {1, ItemType::BirchPlank},
            {1, ItemType::BirchPlank}, {0, ItemType::Air},        {1, ItemType::BirchPlank},
            {1, ItemType::BirchPlank}, {1, ItemType::BirchPlank}, {1, ItemType::BirchPlank}
        };
        write(file, items, {1, ItemType::Chest});
    }

    // Acacia Chest
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::AcaciaPlank}, {1, ItemType::AcaciaPlank}, {1, ItemType::AcaciaPlank},
            {1, ItemType::AcaciaPlank}, {0, ItemType::Air},         {1, ItemType::AcaciaPlank},
            {1, ItemType::AcaciaPlank}, {1, ItemType::AcaciaPlank}, {1, ItemType::AcaciaPlank}
        };
        write(file, items, {1, ItemType::Chest});
    }
}
void Stick(std::ofstream& file) {
    // Standard Stick (from any planks)
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::OakPlank},    {0, ItemType::Air},  {0, ItemType::Air},
          {1, ItemType::OakPlank}};
        write(file, items, {4, ItemType::Stick});
    }

    // Spruce Stick
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::SprucePlank}, {0, ItemType::Air}, {0, ItemType::Air},
            {1, ItemType::SprucePlank}};
        write(file, items, {4, ItemType::Stick});
    }

    // Birch Stick
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::BirchPlank},  {0, ItemType::Air}, {0, ItemType::Air},      
            {1, ItemType::BirchPlank}};
        write(file, items, {4, ItemType::Stick});
    }

    // Acacia Stick
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {
            {1, ItemType::AcaciaPlank}, {0, ItemType::Air}, {0, ItemType::Air},      
            {1, ItemType::AcaciaPlank}, };
        write(file, items, {4, ItemType::Stick});
    }
}
void Plank(std::ofstream& file) {
    // Oak Planks (from Oak Log)
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {{1, ItemType::OakLog}};
        write(file, items, {4, ItemType::OakPlank});
    }

    // Spruce Planks (from Spruce Log)
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {{1, ItemType::SpruceLog}};
        write(file, items, {4, ItemType::SprucePlank});
    }

    // Birch Planks (from Birch Log)
    {
        std::vector<std::pair<unsigned char, ItemType>> items = { {1, ItemType::BirchLog}};
        write(file, items, {4, ItemType::BirchPlank});
    }

    // Acacia Planks (from Acacia Log)
    {
        std::vector<std::pair<unsigned char, ItemType>> items = {{1, ItemType::AcaciaLog}};
        write(file, items, {4, ItemType::AcaciaPlank});
    }
}
int main() {
    std::ofstream file("bin/recipe.bin");
    unsigned int size = 37;
    file.write((char*)&size, sizeof(int));
    CraftingTable(file); // 4
    Furnace(file); // 1
    Chest(file); // 4
    Axe(file); //5
    Pickaxe(file);//5
    Hoe(file);//5
    Shovel(file);//5
    Stick(file); //4
    Plank(file); //4
    file.close();
    return 0;
}