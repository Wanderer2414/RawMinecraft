#include "NameStorage.h"
#include "Id.h"
#include <cstdlib>

NameStorage::NameStorage() {
    root = new Node();
}
NameStorage::~NameStorage() {
    free(root);
}
void NameStorage::free(Node*& root) {
    if (!root) return ;
    for (int i = 0; i<256; i++)
        if (root->next[i]) free(root->next[i]);
    delete root;
    root = 0;
}
vector<Id> NameStorage::getList(const string& name) const {
    if (name.empty() || name.size() > 64) return {};
    Node* tmp = root;
    for (char i:name) {
        if (tmp->next[i]) tmp = tmp->next[i];
        else break;
    }
    vector<Id> ans;
    getList(tmp, ans);
    return ans;
}
void NameStorage::getList(Node* root, vector<Id>& ans) const {
    if (root->id) ans.push_back(root->id);
    for (int i = 0; i<256; i++) {
        if (root->next[i]) getList(root->next[i], ans);
    }
}
Id NameStorage::operator[](const string& name) const {
    if (name.empty()) return 0;
    Node* tmp = root;
    for (int i = 0; i<name.size(); i++) {
        if (tmp->next[name[i]]) tmp = tmp->next[name[i]];
        else break;
    }
    return tmp->id;
}
void NameStorage::insert(const string& name, const Id& id) {
    string str = name;
    Node* tmp = root;
    for (char i:name) {
        if (!tmp->next[i]) tmp->next[i] = new Node();
        tmp = tmp->next[i];
    }
    tmp->id = id;
}