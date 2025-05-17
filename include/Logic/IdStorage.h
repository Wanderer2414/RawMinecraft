#ifndef IDSTORAGE_H
#define IDSTORAGE_H
#include "Id.h"

class IdStorage {
public:
    static const unsigned int file_max_id = 100;
    static const unsigned int block_size = 128;
    IdStorage(const string& source_dir);
    ~IdStorage();
    Id              create();
    string          getFile(const size_t& index) const;
    bool            contains(const Id& id) const;

    vector<pair<Id, Id>>    getSide(const Id& id)      const;
    vector<pair<Id, Id>>    getElement(const Id& id)   const;
    vector<Id>              operator[](const Id& id);

    void            load(),
                    save();

    void            match(const Id& sid, const Id& did, const Id& mid);
private:
    struct Node {
        Node* next[10];
    };
    struct vertex {
        Id id;
        vector<vertex*> next;
        map<Id, vertex*> element;
        vertex(const Id& d): id(d) {};
    };
    map<Id, vertex*> vertices;
    Node *root;

    unordered_set<size_t>   unsaved_id;
    map<Id, size_t>         indexes;
    string                  current_file;
    string                  source_dir;
    fstream                 index_file, current_data_file, fragile_file;
    map<unsigned int, stack<unsigned int>> fragile;

    void            search(Node* root, vector<Id>& ans),
                    search(Node* root, vector<Id>& ans, size_t& id),
                    loadIndexes(),
                    loadFragile(),
                    save(const Id& id),
                    insert(Id& id),
                    free(Node*& root);
    
};
#endif