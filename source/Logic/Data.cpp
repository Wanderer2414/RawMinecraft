#include "Data.h"
#include <cstring>

Data* Data::fetch(char* data) {
    int type;
    memcpy((char*)&type, data, sizeof(int));
    if (type == Type::string) return StringData::fetch(data);
    else if (type == Type::vertex) return VertexData::fetch(data);
    return 0;
}

StringData::StringData(const std::string& value): data(value) {

}
StringData::~StringData() {

}

string StringData::to_string() const {
    return data;
}

Data::Type StringData::type() const {
    return Type::string;
}
size_t StringData::size() const {
    return data.size()+sizeof(int)+1;
}
void StringData::free() {
    delete this;
}

void StringData::get(char* des) const {
    int type = Type::string;
    memcpy(des, (char*)&type, sizeof(int));
    memcpy(des+sizeof(int), data.c_str(), data.size());
    des[sizeof(int)+data.size()] = 0;
}
StringData* StringData::fetch(char* d) {
    return new StringData(d+sizeof(int));
}
VertexData::VertexData(const Id& i): id(i) {

}
VertexData::~VertexData() {

}



size_t VertexData::size() const {
    return sizeof(Id)+sizeof(int);
}

VertexData* VertexData::fetch(char* data) {
    size_t id;
    memcpy((char*)&id, data+sizeof(int), sizeof(size_t));
    return new VertexData(id);
}
void VertexData::get(char* des) const {
    int type = Type::vertex;
    memcpy(des, (char*)&type, sizeof(int));
    memcpy(des + sizeof(int), (char*)&id, sizeof(Id));
}

string VertexData::to_string() const {
    return id.to_string();
}

Data::Type VertexData::type() const {
    return Type::vertex;
}
void VertexData::free() {
    delete this;
}