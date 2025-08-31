#include "FlyweightStorage.h"
#include <stdexcept>

namespace MyBase {
    void FlyWeightObject::FlyWeightObject::load(const std::string& src) {
        if (__core) FlyWeightCore::close(__core);
        FlyweightStorage::getObject(*this, src);
    }
    FlyWeightObject::FlyWeightObject(): __core(0) {}

    FlyWeightObject::FlyWeightObject(const FlyWeightObject& obj) {
        __core = obj.__core;
        (*__core->__count)++;
    }
    FlyWeightObject::FlyWeightObject(FlyWeightObject&& obj) {
        __core = obj.__core;
        obj.__core = 0;
    }
    bool FlyWeightObject::isEmpty() const {
        return !__core;
    }
    FlyWeightObject& FlyWeightObject::operator=(const FlyWeightObject& obj) {
        __core = obj.__core;
        (*__core->__count)++;
        return *this;
    }
    FlyWeightCore* FlyWeightObject::getCore() const {
        return __core;
    }
    const std::string& FlyWeightObject::getSource() const {
        if (__core) return __core->__source;
        throw std::runtime_error("No data found!");
    }
    FlyWeightObject::~FlyWeightObject() {
        FlyWeightCore::close(__core);
        __core = 0;
    }
    FlyWeightCore::FlyWeightCore() {}
    void FlyWeightCore::close(FlyWeightCore* core) {
        if (core) {
            (*core->__count)--;
            if (!(*core->__count)) FlyweightStorage::removeObject(core->__source);
        }
    }
    FlyWeightCore::~FlyWeightCore() {
    };
    FlyweightStorage::TrieNode::TrieNode(): size(0), obj(0) {
        memset(node, 0, sizeof(void*)*128);
    }
    FlyweightStorage* FlyweightStorage::Default = 0;
    void FlyweightStorage::create() {
        if (!Default) Default = new FlyweightStorage();
    }
    void FlyweightStorage::close() {
        if (Default) delete Default;
        else std::cout << "Flyweight already close!" << std::endl;
        Default = 0;
    }
    FlyweightStorage::FlyweightStorage() {
        __root = new TrieNode();
    }
    
    FlyweightStorage::~FlyweightStorage() {
        std::cout << "Flyweight remains: ";
        if (__root) {
            std::cout << __root->size << std::endl; 
            __free(__root);
        }
    }
    void FlyweightStorage::getObject(FlyWeightObject& obj, const std::string& name) {
        create();
        Default->__create(obj, Default->__root, 0, name);
    }
    void FlyweightStorage::removeObject(const std::string& name) {
        create();
        Default->__remove(Default->__root, 0, name);
        if (!Default->__root) {
            delete Default;
            Default = 0;
        }
    }
    void FlyweightStorage::__free(TrieNode*& root) {
        if (!root) return ;
        for (int i = 0; i<root->size; i++) {
            if (root->node[i]) {
                __free(root->node[i]);
                root->size -= root->node[i]->size;
            }
        }
        if (root->obj) delete root->obj;
        delete root;
        root = 0;
    }
    bool FlyweightStorage::__remove(TrieNode*& root, const int& index, const std::string& name) {
        if (!root) return false;
        if (index == name.size()) {
            if (!root->obj) return false;
            if (!root->size) {
                delete root->obj;
                root->obj = 0;
                return true;
            }
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
    bool FlyweightStorage::__create(FlyWeightObject& obj, TrieNode*& root, const int& index, const std::string& name) {
        if (!root) root = new TrieNode();
        if (index == name.size()) {
            if (!root->obj) {
                root->obj = obj.create(name);
                root->obj->__source = name;
                obj.__core = root->obj;
                obj.__core->__count = &root->size;
                root->size++;
                return true;
            }
            obj.__core = root->obj;
            root->size++;
        }
        else {
            if (__create(obj, root->node[name[index]], index+1, name)) {
                root->size++;
                return true;
            }
        }
        return false;
    }
}