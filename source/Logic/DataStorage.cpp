#include "DataStorage.h"
#include "Data.h"
#include "Global.h"
#include "file_extended.h"

DataStorage::DataStorage(const string& s): source_dir(s) {
    connect(index_file, source_dir+"/data_index.bin");
    connect(fragile_file, source_dir+"/data_fragile.bin");
    loadIndexes();
    for (auto& i:indexes) cout << data[i.first]->to_string() << endl;
}
DataStorage::~DataStorage() {
    saveIndexes();
    for (auto& [id, data]:data) {
        data->free();
    }
    if (current_file.size()) close(current_data_file, current_file);
    close(index_file, source_dir+"/data_index.bin");
    close(fragile_file, source_dir+"/data_fragile.bin");
}

unsigned char DataStorage::getType(const size_t& index) const {
    return (index/max_file_id)%3;
}
unsigned char DataStorage::getType(const Data* data) const {
    uint remain = UINT_MAX;
    u_char ans = 0;
    for (int i = 0; i<3; i++) {
        uint current_remain = block_sizes[i] - data->size()%block_sizes[i];
        if (current_remain<remain) {
            ans = i;
            remain = current_remain;
        }
    }
    return ans;
}
string DataStorage::getFile(const uint& index) const {
    return source_dir + "/data_" + to_string(index) + ".bin";
}
size_t DataStorage::getEmptyBlock(Data* data) {
    u_char type = getType(data);
    int i = 0;
    for (int i = type; i<fragile.size(); i+=3) {
        if (sizes[i]<max_file_id) {
            size_t ans = i*max_file_id+sizes[i];
            sizes[i]++;
            return ans;
        }
        if (fragile[i].size()) {
            uint ans = fragile[i].top();
            fragile[i].pop();
            return ans + i*max_file_id;
        }
    }
    fragile.push_back(stack<uint>());
    sizes.push_back(1);
    return (sizes.size()-1)*max_file_id;
}
uint DataStorage::getFragileBlock(const size_t& index){
    if (fragile[index].size()) {
        uint ans = fragile[index].top();
        fragile[index].pop();
        return ans;
    }
    else return max(sizes[index]++, uint(101));
}
void DataStorage::create(const Id& id) {
    data[id] = new VertexData(id);
    unsaved_id.insert(id);
}
void DataStorage::set_data(const Id& id, Data* d) {
    data[id]->free();
    data[id] = d;
    unsaved_id.insert(id);
}
Data* DataStorage::getData(const Id& id) const {
    if (data.find(id)!=data.end()) return data.at(id);
    return 0;
}
void DataStorage::save() {
    for (auto& id:unsaved_id) saveId(id);
}
void DataStorage::saveId(const Id& id) {
    size_t index;
    Data* d = data[id];
    if (indexes.find(id) == indexes.end()) {
        index = getEmptyBlock(d);
        indexes[id] = index;
    }
    else {
        index = indexes[id];
        if (getType(d) != getType(index)) {
            removeId(id);
            index = getEmptyBlock(d);
            indexes[id] = index;
        }
    }
    u_char block_size = block_sizes[getType(d)];
    uint file_index = index/max_file_id;
    uint block_index = index%max_file_id;
    string file = getFile(file_index);
    if (current_file != file) {
        if (current_file.size()) close(current_data_file, current_file);
        connect(current_data_file, file);
        current_file = file;     
    }
    current_data_file.seekp(block_index*block_size);
    char *store = new char[block_size];
    fill(store, store+block_size, 0);
    uint offset = 0;
    size_t sz = d->size();

    memcpy(store, (char*)&sz, sizeof(size_t));
    offset += sizeof(size_t);

    char* tmp = new char[sz];
    d->get(tmp);

    while (sz) {
        uint size = min(sz, block_size - offset - sizeof(size_t));
        memcpy(store+offset,tmp, size);
        offset += size;
        sz -= size;
        if (sz) {
            uint address = getFragileBlock(index);
            memcpy(store+offset, (char*)&address, sizeof(int));
            fill(store, store+block_size, 0);
            current_data_file.write(store, block_size);
            current_data_file.seekp(address);
            offset = 0;
        }
    }
    current_data_file.write(store, block_size);
    delete[] tmp;
    delete[] store;

}
void DataStorage::loadId(const Id& id) {
    size_t index = indexes[id];
    uint file_index = index/max_file_id;
    string file = getFile(file_index);
    if (current_file != file) {
        close(current_data_file, current_file);
        connect(current_data_file, file);
        current_file = file;     
    }
    u_char block_size = block_sizes[getType(index)];
    uint block_index = index%max_file_id;
    current_data_file.seekg(block_size*block_index);
    char* store = new char[block_size];
    fill(store, store+block_size, 0);

    current_data_file.read(store, block_size);
    size_t offset = 0;

    size_t sz;
    memcpy((char*)&sz, store, sizeof(size_t));
    offset += sizeof(size_t);

    char* tmp = new char[sz];
    size_t local_offset = 0;

    while (sz) {
        uint size = min(block_size-sizeof(size_t)-offset, sz);
        memcpy(tmp + local_offset, store + offset, size);
        local_offset += size;
        offset += size;
        sz -= size;
        if (sz) {
            size_t addr;
            memcpy((char*)&addr, store+offset, sizeof(size_t));
            current_data_file.seekg(addr);
            current_data_file.read(store, block_size);
            offset = 0;
        }
    }
    set_data(id, Data::fetch(tmp));
    delete[] tmp;
    delete[] store;
}

void DataStorage::removeId(const Id& id) {
    uint index = indexes[id];
    uint file_index = index/max_file_id;
    uint block_index = index%max_file_id;
    u_char block_size = block_sizes[getType(index)];
    string file = getFile(file_index);
    if (file != current_file) {
        close(current_data_file, current_file);
        connect(current_data_file, file);
        current_file = file;
    }
    size_t addr = block_index*block_size;
    do {
        current_data_file.seekg(addr + block_size - sizeof(size_t));
        current_data_file.read((char*)&addr, sizeof(size_t));
        if (addr) fragile[file_index].push(addr/block_size + file_index*max_file_id); 
    }
    while (addr);
    indexes.erase(id);
}
void DataStorage::saveIndexes(const Id& id, const uint& addr) {
    if (indexes.find(id) == indexes.end()) {
        index_file.seekp(indexes.size()*(sizeof(int)+sizeof(Id)) + sizeof(int));
        char store[sizeof(Id) + sizeof(int)];
        memcpy(store, (char*)&id, sizeof(Id));
        memcpy(store+sizeof(int), (char*)&addr, sizeof(int));
        index_file.write(store, sizeof(Id)+sizeof(int));
    }
}
void DataStorage::saveIndexes() {
    index_file.seekp(0);

    u_char block_size = block_sizes[0];
    char store[block_sizes[0]];
    fill(store, store+block_size, 0);
    uint offset = 0;

    mempcpy(store+offset, HASHCODE, sizeof(int));
    offset+=sizeof(int);

    uint sz = indexes.size();
    memcpy(store + offset, (char*)&sz, sizeof(int));
    offset += sizeof(int);

    for (auto& i:indexes) {
        if (offset+sizeof(Id) + sizeof(int) >= block_size) {
            index_file.write(store, offset);
            memcpy(store, store+offset, block_size-offset);
            offset = block_size-offset;
            fill(store+offset, store+block_size, 0);
        }
        memcpy(store+offset, (char*)&i.first, sizeof(Id));
        offset += sizeof(Id);

        memcpy(store+offset, (char*)&i.second, sizeof(uint));
        offset += sizeof(uint);
    }
    index_file.write(store, offset);
}
void DataStorage::loadIndexes() {
    index_file.seekg(0, ios::end);
    size_t file_size = index_file.tellg();
    index_file.seekg(0, ios::beg);

    char store[block_sizes[0]];
    uint offset = 0;

    u_char block_size = block_sizes[0];
    
    index_file.read(store, min((size_t)block_size, file_size));
    char code[5] = {0};
    memcpy(code, store, sizeof(int));
    offset += sizeof(int);
    if (strcmp(code, HASHCODE)) return;

    uint n = 0;
    memcpy((char*)&n, store+offset, sizeof(int));
    offset += sizeof(int);
    sizes.clear();
    for (int i = 0; i<n; i++) {
        if (offset + sizeof(Id) + sizeof(int) > block_size) {
            memcpy(store, store+offset, block_size-offset);
            offset = block_size-offset;
            index_file.read(store+offset, block_size-offset);
            offset = 0;
        }
        Id id;
        memcpy((char*)&id, store+offset, sizeof(Id));
        offset += sizeof(Id);
        uint addr;
        memcpy((char*)&addr, store+offset, sizeof(int));
        offset += sizeof(int);
        indexes[id] = addr;
        uint file_index = addr/block_sizes[getType(addr)];
        if (file_index>=sizes.size()) sizes.resize(file_index+1, 0);
        sizes[file_index]++;
        create(id);
        if (getType(addr) == 0) loadId(id);
    }
}
void DataStorage::saveFragiles() {
    uint block_size = block_sizes[0];
    char store[block_sizes[0]];
    uint offset = sizeof(int);
    fragile_file.seekp(block_size);
    uint size = 0;
    for (uint i = 0; i<fragile.size(); i++) {
        if (fragile[i].size()) {
            char local_store[block_sizes[0]];
            uint local_offset = 0;
            fill(local_store, local_store+block_size, 0);
            offset = 0;
            while (fragile[i].size()) {
                memcpy(local_store+local_offset, (char*)&fragile[i].top(), sizeof(int));
                local_offset += sizeof(int);
                fragile[i].pop();
            }
            size++;
            fragile_file.write(local_store, block_size);
            memcpy(store + offset, (char*)&i, sizeof(int));
            offset += sizeof(int);
            uint addr = size*block_size;
            memcpy(store + offset, (char*)&addr, sizeof(int));
            offset += sizeof(int);
        }
    }
    memcpy(store, (char*)&size, sizeof(int));
    fragile_file.seekp(0);
    fragile_file.write(store, block_size);
}