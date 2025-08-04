#ifndef FLYWEIGHT_STORAGE_H
#define FLYWEIGHT_STORAGE_H
#include "Global.h"
namespace MyBase {
    class FlyWeightCore;
    class FlyWeightObject {
    public:
        FlyWeightObject();
        FlyWeightObject(const FlyWeightObject& obj);
        FlyWeightObject(FlyWeightObject&& obj);
        FlyWeightObject& operator=(const FlyWeightObject& obj);
        virtual ~FlyWeightObject();
        bool isEmpty() const;
        void load(const std::string& src);
        const std::string& getSource() const;

        friend class FlyweightStorage;
    protected:
        FlyWeightCore* getCore() const;
    private:
        FlyWeightCore* __core;
        virtual FlyWeightCore* create(const std::string& src) const = 0;
    };

    class FlyWeightCore {
    public:
        FlyWeightCore(const FlyWeightCore&) = delete;
        FlyWeightCore& operator=(const FlyWeightCore&) const = delete; 
        friend class FlyWeightObject;
        friend class FlyweightStorage;
        virtual ~FlyWeightCore();
    protected:
        FlyWeightCore();
    private:
        unsigned int    *__count;
        std::string     __source;

        static void close(FlyWeightCore* core);
    };
    
    class FlyweightStorage {
    public:
        friend class FlyWeightObject;
        friend class FlyWeightCore;
        static void close();
    protected:
    private:
        static void getObject(FlyWeightObject& object, const std::string& name);
        static void removeObject(const std::string& name);
        static void create();
        struct TrieNode {
            TrieNode();
            unsigned int size;
            FlyWeightCore* obj;
            TrieNode* node[128];
        };
        TrieNode* __root;
        FlyweightStorage();
        ~FlyweightStorage();
        static FlyweightStorage* Default;

        void __free(TrieNode*& root);
        bool __remove(TrieNode*& root, const int& index, const std::string& name);
        bool __create(FlyWeightObject& object, TrieNode*& root, const int& index, const std::string& name);
    };
}
#endif