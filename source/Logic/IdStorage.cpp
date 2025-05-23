#include "IdStorage.h"
#include "Id.h"
#include <cstring>
#include <number_extended.h>
#include <file_extended.h>

IdStorage::IdStorage(const string& s): source_dir(s) {
    root = new Node();
    connect(index_file, source_dir+"/id_index.bin");
    connect(fragile_file, source_dir+"/id_fragile.bin");
    loadIndexes();
    loadFragile();
}
IdStorage::~IdStorage() {
    free(root);
    for (auto& [id, data]:vertices) {
        delete data;
    }
    if (current_file.size()) close(current_data_file, current_file);
    close(index_file, source_dir+"/id_index.bin");
    close(fragile_file, source_dir+"/id_fragile.bin");
}

Id IdStorage::create() {
    srand(clock());
    Id id = Id(rand()%9000000000 + 1000000000);
    while (vertices.find(id) != vertices.end()) id+=1;
    vertices[id] = new vertex(id);
    insert(id);
    unsaved_id.insert(id);
    return id;
}
bool IdStorage::contains(const Id& id) const {
    size_t num = id.m_id;
    reverse(num);
    Node* tmp = root;
    for (int i = 0; i<Id::size; i++) {
        if (!tmp->next[num%10]) return false;
        tmp = tmp->next[num%10];
        num /= 10;
    }
    return true;
}
string IdStorage::getFile(const size_t& index) const {
    return source_dir + "/id_link_"+to_string(index) + ".bin";
}
void IdStorage::match(const Id& sid, const Id& did, const Id& mid) {
    vertex* src = 0, *des = 0, *med = 0;
    src = vertices[sid];
    des = vertices[did];
    med = vertices[mid];
    med->element[sid] = des;
    src->next.push_back(med);
    des->next.push_back(med);
    unsaved_id.insert(mid);
}
void IdStorage::insert(Id& id) {
    size_t num = id;
    reverse(num);
    Node* tmp = root;
    for (int i = 0; i<Id::size; i++) {
        if (!tmp->next[num%10]) tmp->next[num%10] = new Node();
        tmp = tmp->next[num%10];
        num /= 10;
    }
}
void IdStorage::save() {
    for (auto& i:unsaved_id) save(i);
    unsaved_id.clear();
}
void IdStorage::save(const Id& id) {
    cout << "Save id " << id.to_string() << " 's links" << endl;
    //Save index
    uint offset = 0;
    if (indexes.find(id)==indexes.end()) {
        index_file.seekp(0);
        unsigned int sz = indexes.size() + 1;
        index_file.write((char*)&sz, sizeof(int));
        offset += sizeof(size_t);
        sz -= 1;
        index_file.seekp(sz*(sizeof(Id) + sizeof(int)) + sizeof(int));
        index_file.write((char*)&id, sizeof(Id));
        index_file.write((char*)&sz, sizeof(int));
        indexes[id] = sz;
    }
    //Save data
    int index = indexes[id];
    uint file_index = index/file_max_id;
    uint block_index = index%file_max_id;
    string file = getFile(file_index);
    //Connect file
    if (file!=current_file) {
        if (current_file.size()) close(current_data_file, current_file);
        connect(current_data_file, file);
        current_file = file;
    }
    //Set push position
    current_data_file.seekp(block_size*index);
    //Prepare data
    vector<pair<Id, Id>> elements;
    for (auto& [src, des]:vertices[id]->element) elements.push_back({src, des->id}); 
    //Initial store block
    char store[block_size] = {0};
    offset = 0;
    unsigned int count = elements.size();
    memcpy(store+offset, (char*)&count, sizeof(int));
    offset+=sizeof(int);
    cout << elements.size() << endl;
    for (auto& [src, des]:elements) {
        if (offset + sizeof(size_t) + sizeof(Id)*2>block_size) {
            uint block = 0;
            if (fragile.find(index) != fragile.end() && fragile[index].size()) {
                auto& tmp = fragile[index];
                block = tmp.top();
                tmp.pop();
                if (tmp.empty()) tmp.push(block+1);
            }
            else {
                fragile[index].push(file_max_id+2);
                block = file_max_id+1;
            }
            mempcpy(store+block_size - sizeof(size_t), (char*)&block, sizeof(int));
            current_data_file.write(store, block_size);
            offset = 0;
        }
        memcpy(store+offset, (char*)&src, sizeof(Id));
        offset += sizeof(Id);
        memcpy(store+offset, (char*)&des, sizeof(Id));
        offset += sizeof(Id);
    }
    current_data_file.write(store, block_size);
}
void IdStorage::loadIndexes() {
    //Load indexes
    unsigned int n = 0;
    unsigned int offset = 0;
    char store[block_size] = {0};
    index_file.seekg(0);
    index_file.read(store, block_size);
    memcpy((char*)&n, store+offset, sizeof(int));
    offset += sizeof(int);
    while (n) {
        while (n && block_size - offset > sizeof(Id) + sizeof(int)) {
            Id id;
            memcpy((char*)&id, store + offset, sizeof(Id));
            offset += sizeof(Id);
            unsigned int addr;
            memcpy((char*)&addr, store + offset, sizeof(int));
            offset += sizeof(int);
            indexes[id] = addr;
            n--;
        }
        memcpy(store, store+offset, block_size-offset);
        offset = block_size-offset;
        index_file.read(store+offset, block_size-offset);
        offset = 0;
    }
}
void IdStorage::loadFragile() {
    //Load fragile
    unsigned int n = 0;
    unsigned int offset = 0;
    char store[block_size];
    fragile_file.seekg(0);
    fragile_file.read((char*)&n, sizeof(int));
    for (int i = 0; i<n; i++) {
        fragile_file.read(store, block_size);
        offset = 0;
        unsigned int index;
        memcpy((char*)&index, store+offset, sizeof(int));
        offset += sizeof(int);
        unsigned int count;
        memcpy((char*)&count, store+offset, sizeof(int));
        offset += sizeof(int);
        for (int i = 0; i<count; i++) {
            unsigned int addr;
            memcpy((char*)&addr, store+offset, sizeof(int));
            offset += sizeof(int);
            fragile[index].push(addr);
        }
    }
}
void IdStorage::load() {
    loadIndexes();
    loadFragile();
}
vector<Id> IdStorage::operator[](const Id& id) {
    size_t num = id.m_id;
    reverse(num);
    Node* tmp = root;
    while (num) {
        if (!tmp->next[num%10]) break;
        tmp = tmp->next[num%10];
        num /= 10;
    }
    vector<Id> ans;
    size_t i = id;
    search(tmp, ans, i);
    return ans;
}
void IdStorage::search(Node* root, vector<Id>& ans) {
    size_t id = 0;
    search(root, ans, id);
}
void IdStorage::search(Node* root, vector<Id>& ans, size_t& id) {
    bool end = true;
    for (int i = 0; i<10; i++) {
        if (root->next[i]) {
            end = false;
            id = id*10 + i;
            search(root->next[i], ans, id);
            id = id/10;
        }
    }
    if (end && id) ans.push_back(id);
}
void IdStorage::free(Node*& root) {
    if (!root) return ;
    for (int i = 0; i<10; i++) 
        if (root->next[i]) free(root->next[i]);
    delete root;
    root = 0;
}

vector<pair<Id,Id>> IdStorage::getSide(const Id& id) const {
    vector<pair<Id,Id>> ans;
    for (auto& i:vertices.at(id)->next) {
        ans.push_back({i->id, vertices.at(i->id)->element[id]->id});
    }
    return ans;
}
vector<pair<Id,Id>> IdStorage::getElement(const Id& id) const {
    vector<pair<Id,Id>> ans;
    for (auto& i:vertices.at(id)->element) {
        ans.push_back({i.first, i.second->id});
    }
    return ans;
}