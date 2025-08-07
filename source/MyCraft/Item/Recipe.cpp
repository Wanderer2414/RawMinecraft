#include "Recipe.h"
#include "Block.h"
#include "Item.h"
namespace MyCraft {
    Recipe::Recipe() {}
    Recipe::~Recipe() {}
    void Recipe::get(Item** items, const int& count) {
        int start = 0;
        while (!items[start]) start++;
        int maxCount = items[start]->getCount();
        for (int i = 0; i<__recipe.size(); i++) {
            if (items[start+i])
                maxCount = std::min(maxCount, items[start+i]->getCount()/__recipe[i].count);
        }

        maxCount = std::min(maxCount, count/__output.count);
        maxCount = std::min(maxCount, 64/__output.count);
        for (int i = 0; i<__recipe.size(); i++) {
            if (items[start+i]) {
                items[start+i]->setCount(items[start+i]->getCount() - __recipe[i].count*maxCount);
                if (!items[start+i]->getCount()) {
                    delete items[start+i];
                    items[start+i] = 0;
                }
            }
        }
    }

    Item* Recipe::getMaxResult(const ItemPacked& package, Item** items, const int& count){
        int start = 0;
        while (!items[start]) start++;
        int maxCount = items[start]->getCount();
        for (int i = 0; i<__recipe.size(); i++) 
            if (items[start+i]) maxCount = std::min(maxCount, items[start+i]->getCount()/__recipe[i].count);
        maxCount = std::min(maxCount, count/__output.count);
        maxCount = std::min(maxCount, 64/__output.count);
        Item* item = Item::create(package, maxCount*__output.count, __output.item);
        return item;
    }
    Recipe* RecipeCreator::getItem(Item** item) {
        int i = 0; int j = 8;
        while (!item[i] && i<9) i++;
        while (!item[j] && j<9) j--;
        if (i>j) return 0;
        Recipe* recipe = new Recipe();
        if (Instance().__get(*recipe, item+i, j-i+1)) return recipe;
        else {
            delete recipe;
            return 0;
        }
    }
    RecipeCreator::RecipeCreator(): __root(0) {
        std::ifstream file("bin/recipe.bin");
        if (file.is_open()) {
            unsigned int size = 0;
            file.read((char*)&size, sizeof(int));
            for (int i = 0; i<size; i++) {
                unsigned char s = 0;
                file.read((char*)&s, sizeof(char));
                std::vector<RecipeSlot> recipe(s);
                file.read((char*)&recipe[0], sizeof(RecipeSlot)*s);

                RecipeSlot result = {0,ItemType::Air};
                file.read((char*)&result, sizeof(RecipeSlot));
                __add(recipe, __root, 0, result);
            }
            file.close();
        }
    }
    RecipeCreator::~RecipeCreator() {
        __free(__root);
    }
    RecipeCreator& RecipeCreator::Instance() {
        static RecipeCreator recipe;
        return recipe;
    }
    RecipeCreator::TrieNode::TrieNode() {
        result = {0, ItemType::Air};
    }
    void RecipeCreator::__add(std::vector<RecipeSlot>& type, TrieNode*& root, const int& index, const RecipeSlot& result) {
        if (!root) root = new TrieNode();
        if (index==type.size()) {
            root->result = result;
        }
        else {
            __add(type, root->maps[type[index].item], index+1, result);
            root->maps[type[index].item]->count = type[index].count;
        }
    }
    void RecipeCreator::__free(TrieNode*& root) {
        if (!root) return;
        for (auto component: root->maps) {
            __free(component.second);
        }
        delete root;
        root = 0;
    }
    bool RecipeCreator::__get(Recipe& recipe, Item **item, const int& size) {
        TrieNode* root = __root;
        RecipeSlot out = {item[0]->getCount(), ItemType::Air};
        for (int i = 0; i<size; i++) {
            if (!root) return false;
            if (item[i]) {
                if (root->maps.find(*item[i]) == root->maps.end()) return false;
                root = root->maps[*item[i]];
                recipe.__recipe.push_back({root->count, *item[i]});
            }
            else if (root->maps.find(ItemType::Air) != root->maps.end()){
                root = root->maps[ItemType::Air];
                recipe.__recipe.push_back({0, ItemType::Air});
            }
            else return false;
        }
        if (root->result.item != ItemType::Air) {
            recipe.__output = root->result;
            return true;
        }
        else return false;
    }
}