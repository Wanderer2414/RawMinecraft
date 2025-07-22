#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H
#include "Date.h"
#include "File.h"
namespace MyCraft {
    class MapManager: public MyBase::FileTransferElement {
    public:
        MapManager();
        MapManager(const MapManager&) = delete;
        ~MapManager();
        MapManager& operator=(const MapManager&) const = delete; 

        class MapInfo: public MyBase::FileTransferElement {
        public:
            MapInfo();
            ~MapInfo();
            MapInfo& operator=(const MapInfo&) const = delete; 

        protected:
        private:
            std::string     __name;
            MyBase::Date    __createDate;
            void input(MyBase::File& file) override;
            void output(MyBase::File& file) const override;
        };
    protected:
    private:
        std::vector<MapInfo> __maps;
        void input(MyBase::File& file) override;
        void output(MyBase::File& file) const override;
    };
}
#endif