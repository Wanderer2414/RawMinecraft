#include "DataCenter.h"
#include "Data.h"
#include "DataStorage.h"
#include "IdStorage.h"

DataCenter::DataCenter(): id_storage("bin"), data_storage("bin") {
    
}
DataCenter::~DataCenter() {

}

vector<Id> DataCenter::search_name(const string& name) const {
    return name_storage.getList(name);
}
Id DataCenter::create(const Id& i) {
    Id id = i;
    if (!id) id = id_storage.create();
    data_storage.create(id);
    unsaved_id.insert(id);
    return id;
}
void DataCenter::save() {
    id_storage.save();
    data_storage.save();
}
void DataCenter::match(const Id& src, const Id& des, const Id& med) {
    id_storage.match(src, des, med);
}
void DataCenter::setData(const Id& id, Data* data) {
    data_storage.set_data(id, data);
    unsaved_id.insert(id);
    if (data->type() == Data::string) {
        name_storage.insert(data->to_string(), id);
    }
}
Id DataCenter::operator[](const string& data) {
    return name_storage[data];
}
Data* DataCenter::getData(const Id& id) const {
    return data_storage.getData(id);
}
vector<pair<Id,Id>> DataCenter::getElement(const Id& id) const{
    return id_storage.getElement(id);
}
vector<pair<Id, Id>> DataCenter::getSide(const Id& id) const {
    return id_storage.getSide(id);
}