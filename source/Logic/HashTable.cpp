#include "HashTable.h"

HashTable::HashTable(const size_t& sz):max_size(sz) {
    storage = new Id[sz+1] {0};
    data = new unsigned int[sz+1] {0};
    current_sz = 0;
}

HashTable::~HashTable() {
}
size_t HashTable::size() const {
    return current_sz;
}
void HashTable::insert(const Id& id, const unsigned int& d) {
    size_t index = id;
    index %= max_size;
    if (!storage[index]) {
        storage[index] = id;
        data[index] = d;
        current_sz++;
    }
    else if (storage[index] != id) {
        while (storage[index]) index = (index+1)%max_size;
        storage[index] = id;
        data[index] = d;
        current_sz++;
    }
}
bool HashTable::contains(const Id& id) const {
    size_t index = id;
    index %= max_size;
    if (storage[index] == id) return true;
    else {
        size_t pivot = index;
        index++;
        while (storage[index] != id && index != pivot) index = (index+1)%max_size;
        if (index == pivot) return false;
        else return true;
    }
}
unsigned int HashTable::operator[](const Id& id) const {
    size_t index = id;
    index %= max_size;
    if (storage[index] == id) return data[index];
    else {
        size_t pivot = index;
        index++;
        while (storage[index] != id && index != pivot) index = (index+1)%max_size;
        if (index == pivot) return 0;
        else return data[index];
    }
}

HashTable::iterator::iterator(Id* i, unsigned int* d, const size_t& size): id(i), data(d), sz(size) {

}
pair<Id, unsigned int> HashTable::iterator::operator*() const {
    return {*id, *data};
}
HashTable::iterator& HashTable::iterator::operator++() {
    while (id && sz) {
        id++;
        data++;
        sz--;
    }
    return *this;
}
bool HashTable::iterator::operator!=(const iterator& other) const {
    return other.sz != sz;
}
HashTable::iterator HashTable::begin() {
    size_t t = 0;
    while (!storage[t] && t<max_size) t++;
    return HashTable::iterator(&storage[t], &data[t], max_size-t);
}
HashTable::iterator HashTable::end() {
    return HashTable::iterator(&storage[max_size], &data[max_size], 0);
}