#include "Biome.h"
#include <stdexcept>
namespace MyCraft {


    Biomes::Biomes(const unsigned int& width, const unsigned int& height): __size(width, height), __position(0, 0) {
        __count[0] = __count[1] = __count[2] = __count[3] = __count[4] = 0;
        __averageHeight[0] = __averageHeight[1] = __averageHeight[2] = __averageHeight[3] = __averageHeight[4] =0;
        __biome = new Biome*[width];
        for (int i = 0; i<width; i++) {
            __biome[i] = new Biome[height];
            memset(__biome[i], 0, height*sizeof(Biome));
        }
    }
    Biomes::Biomes(Biomes&& biome) {
        for (int i = 0; i<5; i++) {
            __averageHeight[i] = biome.__averageHeight[i];
            __count[i] = biome.__count[i];
        }
        __biome = biome.__biome;
        __size = biome.__size;
        __position = biome.__position;
        biome.__biome = 0;
    }
    Biomes::~Biomes() {
        if (__biome) {
            for (int i = 0; i<__size.x; i++) delete[] __biome[i];
            delete[] __biome;
        }
    }
    Biomes::Biomes(const HeightMap& map) {
        __count[0] = __count[1] = __count[2] = __count[3] = __count[4] = 0;
        __averageHeight[0] = __averageHeight[1] = __averageHeight[2] = __averageHeight[3] = __averageHeight[4] = 0;
        __size = {map.getSize().x/16, map.getSize().y/16};
        __biome = new Biome*[__size.x];
        __position = map.getPosition()/16;
        for (int i = 0; i<__size.x; i++) {
            __biome[i] = new Biome[__size.y];
            memset(__biome[i], 0, __size.y*sizeof(Biome));
        }
        for (int x = 0; x<__size.x; x++) {
            for (int y = 0; y<__size.y; y++) {
                float height = 0;
                for (int i = 0; i<16; i++) {
                    for (int j = 0; j<16; j++) {
                        glm::ivec2 position(__position.x*16 + x*16+i, __position.y*16 + y*16+j);
                        height += 1.0f*map[position]/256;
                    }
                }
                if (height>32) __biome[x][y].type = Biome::SuperHigh;
                else if (height>-24)  __biome[x][y].type = Biome::High;
                else if (height>-32) __biome[x][y].type = Biome::Mid;
                else if (height>-56) __biome[x][y].type = Biome::Low;
                else if (height>-64) __biome[x][y].type = Biome::SuperLow;
                else __biome[x][y].type = Biome::Sea;
                if (__biome[x][y].type>=Biome::SuperLow && __biome[x][y].type<=Biome::SuperHigh) {
                    __averageHeight[__biome[x][y].type-1]+=height;
                    __count[__biome[x][y].type-1]++;
                }
            }
        }
    }
    void Biomes::filter(std::vector<glm::ivec2>& list, const Biome::BiomeType& type) {
        std::vector<std::vector<bool>> matrix(__size.x, std::vector<bool>(__size.y, 0));
        for (int i = 0; i<__size.x; i++) {
            for (int j = 0; j<__size.y; j++) {
                if (!matrix[i][j] && __biome[i][j].type == type) {
                    int count = 16 + rand()%20;
                    int mX = i, mY = j, side = 1;
                    std::vector<glm::ivec2> dummy = {__position + glm::ivec2(i,j)};
                    bool noNew = false;
                    while (!noNew && dummy.size()<count) {
                        mX--; mY--; side+=2;
                        noNew = true;
                        for (int x = mX; x<mX+side; x++) {
                            for (int y = mY; y<mY+side; y++) {
                                if (x>=0 && x<__size.x && y>=0 && y<__size.y && !matrix[x][y]) {
                                    dummy.push_back(__position + glm::ivec2(x,y));
                                    matrix[x][y] = true;
                                    noNew = false;
                                }
                            }
                        }
                    }
                    if (dummy.size()>16 && __biome[i][j].type<6) {
                        list = dummy;
                    }
                }
                
            }
        }
    }
    std::string to_string(const Biome::BiomeType& type) {
        switch (type) {
            case Biome::Sea: return "Sea";
            case Biome::Beach: return "Beach";
            case Biome::MixRockyHill: return "RockyHillSide";
            case Biome::RockyHill: return "RockyHill";
            case Biome::Desert: return "Desert";
            case Biome::Oasis: return "Oasis";
            case Biome::MixOasis: return "OasisSide";
            case Biome::Lake: return "Lake";
            case Biome::UnderGround: return "Underground";
            case Biome::Meadow: return "Meadow";
            case Biome::Null: return "Null";
            default: return "None";
        }
    }
    bool Biomes::isValid(const glm::ivec2& position) const {
        glm::ivec2 offset(position - __position);
        if (offset.x < 0 || offset.x>=__size.x || offset.y < 0 || offset.y>=__size.y)
            return false;
        return true;
    }
    int Biomes::getAverageHeight(const Biome::BiomeType& type) const {
        if (type>=Biome::SuperLow && type<=Biome::SuperHigh && __count[type-1]>0)
            return floor(1.0f*__averageHeight[type-1]/__count[type-1]) ;
        return 0;
    }
    const glm::ivec2& Biomes::getPosition() const {
        return __position;
    }
    const glm::ivec2& Biomes::getSize() const {
        return __size;
    }
    Biome& Biomes::operator[](const glm::ivec2& position) {
        glm::ivec2 offset(position - __position);
        if (offset.x < 0 || offset.x>=__size.x || offset.y < 0 || offset.y>=__size.y)
            throw std::runtime_error("Out range of biomes!");
        return __biome[offset.x][offset.y];
    }
    const Biome& Biomes::operator[](const glm::ivec2& position) const {
        glm::ivec2 offset(position - __position);
        if (offset.x < 0 || offset.x>=__size.x || offset.y < 0 || offset.y>=__size.y)
            throw std::runtime_error("Out range of biomes!");
        return __biome[offset.x][offset.y];
    }
    void Biomes::setPosition(const glm::ivec2& position) {
        __position = position;
    }
    
    BiomeManage::BiomeManage(const std::string& src):__isLoad(false), __source(src), __isChanged(false) {}
    BiomeManage::~BiomeManage() {
        __save();
    }
    std::string BiomeManage::getFileName(const std::string& src, const glm::ivec3& position) {
        glm::ivec3 true_pos(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        size_t id = (true_pos.x+100)*40000 + (true_pos.y+100)*200 + (true_pos.z+100);
        return src+std::format("biome{}.bin", id);
    }
    Biome& BiomeManage::get(const glm::ivec3& position) {
        if (!__isLoad) {
            __isLoad = true;
            __load(position);
        }
        glm::ivec3 offset = position - __position;
        if (offset.x < 0 || offset.x>=16 || offset.y < 0 || offset.y>=16 || offset.z < 0 || offset.z>=16) {
            __load(position);
            offset = position - __position;
            offset = {floor(offset.x/16.f), floor(offset.y/16.f), floor(offset.z/16.f)};
        }
        return __store[offset.x][offset.y][offset.z];
    };

    const Biome& BiomeManage::getBiome(const glm::ivec3& position) {
        return get(position);
    };
    
    void BiomeManage::setBiomeType(const glm::ivec3& position, const Biome::BiomeType& type) {
        get(position).type = type;
        __isChanged = true;
    }
    void BiomeManage::setBiomeHeight(const glm::ivec3& position, const int& height) {
        get(position).height = height;
        __isChanged = true;
    }
    void MyCraft::BiomeManage::__load(const glm::ivec3& position) {
        if (__isChanged) __save();
        std::ifstream file(getFileName(__source, position), std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.read((char*)&__position, sizeof(glm::ivec3));
            file.read((char*)&__store[0][0][0], 4096*sizeof(Biome));
            file.close();
        }
        else {
            __position = {floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f)};
            __position *= 16;
            memset(&__store[0][0][0], 0, 4096*sizeof(Biome));
        }
    }
    void BiomeManage::__save() {
        if (__isChanged) {
            std::ofstream file(getFileName(__source, __position), std::ios::out | std::ios::binary);
            file.write((char*)&__position, sizeof(glm::vec3));
            file.write((char*)&__store[0][0][0], 4096*sizeof(Biome));
            file.close();
            __isChanged = false;
        }
    }
}