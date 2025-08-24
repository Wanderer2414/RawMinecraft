#ifndef RECIPE_H
#define RECIPE_H
#include "Item.h"
namespace MyCraft {
    struct RecipeSlot {
        unsigned char   count;
        ItemType        item;
    };
    class Recipe {
    public:
        Recipe();
        ~Recipe();
        Recipe(const Recipe&) = delete;
        Recipe& operator=(const Recipe&) const = delete; 
        void get(Item** items, const int& count = 1);
        Item* getMaxResult(const ItemPacked& package, Item** items, const int& count = 1);
        friend class RecipeCreator;
    protected:
    private:
        std::vector<RecipeSlot> __recipe;
        RecipeSlot              __output;
    };


    class RecipeCreator {
    public:
        static Recipe* getItem(Item** item);
    protected:
    private:
        RecipeCreator();
        ~RecipeCreator();
        static RecipeCreator& Instance();
        struct TrieNode {
            TrieNode();
            unsigned char count;
            RecipeSlot result;
            std::map<ItemType, TrieNode*> maps;
        };
        TrieNode* __root;
        void __add(std::vector<RecipeSlot>& type, TrieNode*& root, const int& index, const RecipeSlot& result);
        void __free(TrieNode*& root);
        bool __get(Recipe&, Item** item, const int& size);
    };
}
#endif