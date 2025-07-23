#ifndef WORLDS_MANAGE_H
#define WORLDS_MANAGE_H
#include "Date.h"
#include "FileTransferElement.h"
#include "Global.h"

namespace MyCraft {
    class MapInfo: public MyBase::FileTransferElement {
    public:
        MapInfo();
        MapInfo(const unsigned char& index, const std::string& name, const MyBase::Date& date);
        MapInfo(const MapInfo&) = delete;
        ~MapInfo();
        MapInfo& operator=(const MapInfo&) const = delete; 
        const std::string& getWorldName() const;
        const MyBase::Date& getCreatedDate() const;
    protected:
    private:
        unsigned char   __index;
        std::string     __name;
        MyBase::Date    __date;
        void input(MyBase::File& file) override;
        void output(MyBase::File& file) const override;
    };
    
    class WorldsManage {
    public:
        WorldsManage();
        ~WorldsManage();
        WorldsManage(const WorldsManage&) = delete;
        WorldsManage& operator=(const WorldsManage&) const = delete; 
        int createWorld(const std::string& name, const MyBase::Date& date);
        const std::vector<MapInfo*>& getWorld() const;

    protected:
    private:
        static constexpr char source[] = "bin/";
        std::vector<MapInfo*>    __mapsInfo;
    };
};
#endif