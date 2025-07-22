#include "MapManager.h"
namespace MyCraft {
    MapManager::MapManager() {}
    MapManager::~MapManager() {}

    MapManager::MapInfo::MapInfo() {}
    MapManager::MapInfo::~MapInfo() {}

    void MapManager::MapInfo::input(MyBase::File& file) {
        file >> __name;
        file >> __createDate;
    }
    void MapManager::MapInfo::output(MyBase::File& file) const {
        file << __name;
        file << __createDate;
    }


    void MapManager::input(MyBase::File& file) {
        unsigned int size;
        file >> size;
        __maps.resize(size);
        for (int i = 0; i<size; i++) file >> __maps[i];
    }
    void MapManager::output(MyBase::File& file) const {
        unsigned int size = __maps.size();
        file << size;
        for (int i = 0; i<size; i++) file << __maps[i];
    }
}