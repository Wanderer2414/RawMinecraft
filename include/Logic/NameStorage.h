#ifndef NAMESTORAGE_H
#define NAMESTORAGE_H
#include "Global.h"
#include "Id.h"

class NameStorage {
public:
    NameStorage();
    ~NameStorage();
    vector<Id> getList(const string& name) const;
    Id operator[](const string& name) const;
    void insert(const string& name, const Id& id);
private:
    void insert(const int& index, const string& name, const Id& id);
    struct Node {
        Id id;
        Node* next[256] = {0};
    };
    void getList(Node* root, vector<Id>& ans) const;
    void free(Node*& root);
    Node* root;
};
#endif