#include "WorldsManage.h"
#include "File.h"
#include <cstring>

namespace MyCraft {
    MapInfo::MapInfo() {};
    MapInfo::MapInfo(const unsigned char& index, const std::string& name, const MyBase::Date& date):
        __index(index), __name(name), __date(date) {}
    MapInfo::~MapInfo() {}
    const std::string& MapInfo::getWorldName() const {
        return __name;
    }
    const MyBase::Date& MapInfo::getCreatedDate() const {
        return __date;
    }
    void MapInfo::input(MyBase::File& file) {
        file >> __index;
        file >> __name;
        file >> __date;
    };
    void MapInfo::output(MyBase::File& file) const {
        file << __index;
        file << __name;
        file << __date;
    };
    WorldsManage::WorldsManage() {
        std::string src = source; src += "maps.bin";
        MyBase::File file(src);
        if (!file.isNew()) {
            unsigned int size = 0;
            file >> size;
            __mapsInfo.resize(size);
            for (int i = 0; i<size; i++) {
                __mapsInfo[i] = new MapInfo();
                file >> *__mapsInfo[i];
            }
        }
        file.close();
    }
    void WorldsManage::save() {
        std::string src = source; src += "maps.bin";
        MyBase::File file(src);
        unsigned int size = __mapsInfo.size();
        file << size;
        for (int i = 0; i<size; i++) {
            file << *__mapsInfo[i];
        }
        file.close();   
    }
    WorldsManage::~WorldsManage() {
        save();
        for (auto& i:__mapsInfo) delete i;
        __mapsInfo.clear();
    }

    int WorldsManage::createWorld(const std::string& name, const MyBase::Date& date) {
        __mapsInfo.push_back(new MapInfo(__mapsInfo.size(), name, date));
        return __mapsInfo.size()-1;
    }
    const std::vector<MapInfo*>& WorldsManage::getWorld() const {
        return __mapsInfo;
    }
};