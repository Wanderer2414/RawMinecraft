#ifndef CHAINING_HASHTABLE_H
#define CHAINING_HASHTABLE_H
#include "Global.h"
#include "Id.h"

class HashTable {
public:
    HashTable(const size_t& sz = 50);
    ~HashTable();
    size_t size() const;
    bool contains(const Id& index) const;
    unsigned int operator[](const Id& index) const;
    void insert(const Id& index, const unsigned int& data);
    class iterator {
        Id* id; unsigned int* data;
        size_t sz;
    public:
        iterator(Id*, unsigned int*, const size_t& size);
        pair<Id, unsigned int> operator*() const;
        iterator& operator++() ;
        bool operator!=(const iterator& other) const;
    };
    iterator begin();
    iterator end();
private:
    Id* storage;
    unsigned int* data;
    size_t current_sz;
    size_t max_size;
};
#endif