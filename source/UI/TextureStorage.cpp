#include "TextureStorage.h"
#include "General.h"
#include "Global.h"

namespace MyBase {
    TextureStorage::TrieNode::TrieNode(): size(0), texture(0) {
        memset(node, 0, sizeof(void*)*128);
    }
    TextureStorage* TextureStorage::Default = 0;
    TextureStorage& TextureStorage::getInstance() {
        if (!Default) Default = new TextureStorage();
        return *Default;
    }
    void TextureStorage::close() {
        if (Default) delete Default;
        Default = 0;
    }
    TextureStorage::TextureStorage() {
        __root = new TrieNode();
    }
    TextureStorage::~TextureStorage() {
        std::cout << "Texture remains: ";
        if (__root) {
            std::cout << __root->size << std::endl;
            __free(__root);
        }
        else std::cout << 0 << std::endl;
    }
    GLuint TextureStorage::getTexture(const std::string& name) {
        return __create(__root, 0, name);
    }
    void TextureStorage::removeTexture(const std::string& name) {
        __remove(__root, 0, name);
    }
    void TextureStorage::__free(TrieNode*& root) {
        if (!root) return ;
        for (int i = 0; i<root->size; i++) {
            if (root->node[i]) {
                __free(root->node[i]);
                root->size -= root->node[i]->size;
            }
        }
        if (root->texture) glDeleteTextures(1, &root->texture);
        delete root;
        root = 0;
    }
    bool TextureStorage::__remove(TrieNode*& root, const int& index, const std::string& name) {
        if (!root) return false;
        if (index == name.size()) {
            if (!root->texture) return false;
            root->size--;
            if (!root->size) glDeleteTextures(1, &root->texture);
            return true;
        }
        if (__remove(root->node[name[index]], index+1, name)) {
            root->size--;
            if (!root->size) {
                delete root;
                root = 0;
            }
            return true;
        }
        return false;
    }
    GLuint TextureStorage::__create(TrieNode*& root, const int& index, const std::string& name) {
        if (!root) root = new TrieNode();
        if (index == name.size()) {
            if (!root->texture) root->texture = LoadTexture(name);
            root->size++;
            return root->texture;
        }
        root->size++;
        return __create(root->node[name[index]], index+1, name);
    }
}