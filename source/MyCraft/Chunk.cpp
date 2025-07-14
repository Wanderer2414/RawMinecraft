#include "Chunk.h"
#include "Block.h"
#include "DrawingCenter.h"

namespace MyCraft {
    Chunk::Chunk() {
        __blockTypes = new BlockCatogary::Catogary**[16];
        __bits = new std::bitset<16>*();
        __tableIndexes.resize(16);
        for (int i = 0; i<16; i++) {
            __tableIndexes[i].resize(16);
            __blockTypes[i] = new BlockCatogary::Catogary*[16];
            __bits[i] = new std::bitset<16>();
            for (int j = 0; j<16; j++) {
                __tableIndexes[i][j].resize(16, -1);
                __blockTypes[i][j] = new BlockCatogary::Catogary[16];
                std::memset(__blockTypes[i][j], 0, sizeof(BlockCatogary::Catogary)*16);
            }
        }
    }
    Chunk::~Chunk() {
        for (int i = 0; i<16; i++) {
            for (int j = 0; j<16; j++)
                delete[] __blockTypes[i][j];
            delete[] __bits[i];
            delete[] __blockTypes[i];
        }
        delete[] __bits;
        delete[] __blockTypes;
    }
    Chunk* Chunk::Load(const std::string& src) {
        Chunk& new_chunk = *new Chunk();
        std::ifstream file(src, std::ios::in | std::ios::binary);
        file.read((char*)&new_chunk.__position, sizeof(glm::vec3));
        unsigned int sz;
        file.read((char*)&sz, sizeof(int));
        for (int i = 0; i<sz; i++) {
            BlockCatogary::Catogary type;
            file.read((char*)&type, sizeof(type));
            unsigned int size;
            file.read((char*)&size, sizeof(int));
            auto& vecs = new_chunk.__list[type];
            vecs.resize(size);
            file.read((char*)vecs.data(), sizeof(glm::vec4)*size);
            for (int i = 0; i<size; i++) {
                int x = vecs[i].x, y = vecs[i].y, z = vecs[i].z;
                new_chunk.__bits[x][y][z] = 1;
                new_chunk.__tableIndexes[x][y][z] = i;
            }
        }
        for (int i = 0; i<16; i++)
            for (int j = 0; j<16; j++) 
                file.read((char*)&new_chunk.__blockTypes[i][j], sizeof(BlockCatogary::Catogary)*16);
        return &new_chunk;
    }
    void Chunk::save(const std::string& src) {
        std::ofstream file(src, std::ios::binary | std::ios::out);
        file.write((char*)&__position, sizeof(glm::vec3));
        unsigned int sz = __list.size();
        file.write((char*)&sz, sizeof(int));
        for (auto& [type, vecs]:__list) {
            file.write((char*)&type, sizeof(type));
            unsigned int size = vecs.size();
            file.write((char*)&size, sizeof(int));
            file.write((char*)vecs.data(), vecs.size()*sizeof(glm::vec4));
        }
        for (int i = 0; i<16; i++) {
            for (int j  = 0; j<16; j++) {
                file.write((char*)&__blockTypes[i][j], sizeof(BlockCatogary::Catogary)*16);
            }
        }

    }
    BlockCatogary::Catogary& Chunk::getType(const glm::vec3& pos) const {
        int x = std::floor(pos.x-__position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        return __blockTypes[x][y][z];
    }
    void Chunk::setType(const glm::vec3& pos, const BlockCatogary::Catogary& type) {
        int x = std::floor(pos.x-__position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        if (__blockTypes[x][y][z]) {
            auto& vecs = __list[__blockTypes[x][y][z]];
            int index = __tableIndexes[x][y][z];
            vecs[index] = vecs.back();
            vecs.pop_back();
            auto& new_vecs = __list[type];
            __tableIndexes[x][y][z] = new_vecs.size();
            new_vecs.push_back(glm::vec4(pos,1));
        }
        __blockTypes[x][y][z] = type;
    }
    std::bitset<16>::reference Chunk::getBit(const glm::vec3& pos) {
        int x = std::floor(pos.x-__position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        return __bits[x][y][z];
    }
    void Chunk::enableBit(const glm::vec3& pos) {
        int x = std::floor(pos.x-__position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        if (!__bits[x][y][z]) {
            auto& vecs = __list[__blockTypes[x][y][z]];
            __tableIndexes[x][y][z] = vecs.size();
            vecs.push_back(glm::vec4(pos, 1));
            __bits[x][y][z] = 1;
        };

    }
    void Chunk::disableBit(const glm::vec3& pos) {
        int x = std::floor(pos.x-__position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        if (__bits[x][y][z]) {
            auto& vecs = __list[__blockTypes[x][y][z]];
            int index = __tableIndexes[x][y][z];
            vecs[index] = vecs.back();
            vecs.pop_back();
            __bits[x][y][z] = 0;
        }
    }
    void Chunk::glDraw() const {
        DrawingCenter::getInstance().BindCube();
        for (const auto& item: __list) {
            DrawingCenter::getInstance().DrawCubes(item.first, (void*)item.second.data(), item.second.size());
        }
    }
}