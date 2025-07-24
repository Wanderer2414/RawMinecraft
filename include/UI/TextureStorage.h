#ifndef TEXTURE_STORAGE_H
#define TEXTURE_STORAGE_H
#include "Global.h"
namespace MyBase {
    class TextureStorage {
    public:
        GLuint getTexture(const std::string& name);
        void removeTexture(const std::string& name);
        static TextureStorage& getInstance();
        static void close();
    protected:
    private:
        struct TrieNode {
            TrieNode();
            unsigned int size;
            GLuint texture;
            TrieNode* node[128];
        };
        TrieNode* __root;
        TextureStorage();
        ~TextureStorage();
        static TextureStorage* Default;

        void __free(TrieNode*& root);
        bool __remove(TrieNode*& root, const int& index, const std::string& name);
        GLuint __create(TrieNode*& root, const int& index, const std::string& name);
    };
}
#endif