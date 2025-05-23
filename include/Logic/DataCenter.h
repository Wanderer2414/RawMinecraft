#ifndef DATACENTER_H
#define DATACENTER_H
#include "Data.h"
#include "DataStorage.h"
#include "Id.h"
#include "IdStorage.h"
#include "NameStorage.h"

class DataCenter {
public:
    DataCenter();
    ~DataCenter();
    
    Id      create(const Id& id = 0);
    Data*   getData(const Id& id)                   const;
    Id      operator[](const string& id);

    void    save(),
            setData(const Id& id, Data* data),
            match(const Id& src, const Id& des, const Id& med);

    vector<pair<Id, Id>>    getElement(const Id& id)                const;
    vector<Id>              search_name(const string& name)         const;
    vector<pair<Id, Id>>    getSide(const Id& id)                   const;
private:
    IdStorage id_storage;
    NameStorage name_storage;
    DataStorage data_storage;
    unordered_set<size_t> unsaved_id;
};
#endif