#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H
#include "Data.h"
#include "Id.h"

class DataStorage {
public:
    const unsigned int block_sizes[3] = {64, 128, 256};
    const unsigned int max_file_id = 100;

    DataStorage(const string& source_dir);
    ~DataStorage();

    unsigned char   getType(const Data* data)               const,
                    getType(const size_t& sz)               const;
    size_t          getEmptyBlock(Data* data)               ;
    uint            getFragileBlock(const size_t& index);
    string      getFile(const unsigned int& index)          const;
    Data*       getData(const Id& id)                       const;

    void        load(),
                save();

    void        create(const Id& id),
                set_data(const Id& id, Data* data);

private:
    map<Id, Data*> data;
    string      current_file;
    string      source_dir;
    fstream     index_file, fragile_file, current_data_file;
    map<Id, size_t>                         indexes;
    vector<stack<uint>> fragile;
    vector<uint>                            sizes;
    unordered_set<size_t>                   unsaved_id;

    void        loadIndexes(),
                loadFragile(),
                saveIndexes(),
                saveIndexes(const Id& id, const uint& index),
                saveFragiles();

    void        loadId(const Id& id),
                saveId(const Id& id),
                removeId(const Id& id);

};
#endif