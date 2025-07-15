#include "Chunk.h"
#include "Block.h"
#include "DrawingCenter.h"
#include "General.h"
namespace MyCraft {
    Chunk::Chunk(): __isChange(false) {}
    Chunk::~Chunk() {}
    Chunk* Chunk::Load(const std::string& src) {
        Chunk* new_chunk = new Chunk();
        std::ifstream file(src, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.read((char*)&new_chunk->__position, sizeof(glm::vec3));
            unsigned int sz;
            file.read((char*)&sz, sizeof(int));
            for (int i = 0; i<sz; i++) {
                BlockCatogary::Catogary type;
                file.read((char*)&type, sizeof(type));
                unsigned int size;
                file.read((char*)&size, sizeof(int));
                auto& vecs = new_chunk->__list[type];
                vecs.resize(size);
                vecs.back().x = 0;
            }
            unsigned int buffer[16][16];
            file.read((char*)&buffer[0], 256*sizeof(int));
            for (int i = 0; i<16; i++)
                for (int j = 0; j<16; j++) {
                    file.read((char*)&new_chunk->__blockTypes[i][j], sizeof(BlockCatogary::Catogary)*16);
                }
            if (new_chunk->__list.size()) {
                for (int i = 0; i<16; i++)
                    for (int j = 0; j<16; j++) {
                        unsigned int& data = buffer[i][j];
                        new_chunk->__bits[i][j] = data;
                        while (data) {
                            int k = std::log2(data);
                            auto& vecs = new_chunk->__list[new_chunk->__blockTypes[i][j][k]];
                            int index = vecs.back().x++;
                            glm::vec4 position(new_chunk->__position, 1);
                            position.x += i; position.y += j; position.z += k;
                            vecs[index] = position;
                            new_chunk->__tableIndexes[i][j][k] = index;
                            data -= 1<<k;
                        }
                    }
            }
        }
        new_chunk->__source = src;
        file.close();
        return new_chunk;
    }
    glm::vec3 Chunk::getPosition() const {
        return __position;
    }
    void Chunk::save() {
        if (!__isChange) return ;
        std::ofstream file(__source, std::ios::binary | std::ios::out);
        file.write((char*)&__position, sizeof(glm::vec3));
        unsigned int sz = __list.size();
        file.write((char*)&sz, sizeof(int));
        for (auto& [type, vecs]:__list) {
            file.write((char*)&type, sizeof(type));
            unsigned int size = vecs.size();
            file.write((char*)&size, sizeof(int));
        }
        unsigned int buffer[256];
        for (int i = 0; i<16; i++) {
            for (int j = 0; j<16; j++) {
                buffer[i*16+j] = __bits[i][j].to_ulong();
            }
        }
        file.write((char*)&buffer[0], sizeof(int)*256);
        for (int i = 0; i<16; i++) {
            for (int j  = 0; j<16; j++) {
                file.write((char*)&__blockTypes[i][j], sizeof(BlockCatogary::Catogary)*16);
            }
        }
        __isChange = false;
        file.close();
    }
    const BlockCatogary::Catogary& Chunk::getType(const glm::vec3& pos) const {
        int x = std::floor(pos.x - __position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);

        return __blockTypes[x][y][z];
    }
    void Chunk::setType(const glm::vec3& pos, const BlockCatogary::Catogary& type) {
        int x = std::floor(pos.x-__position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        if (__blockTypes[x][y][z] == type) return;
        __isChange = true;
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
            __isChange = true;
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
            __isChange = true;
            auto& vecs = __list[__blockTypes[x][y][z]];
            int index = __tableIndexes[x][y][z];
            __tableIndexes[x][y][z] = -1;
            if (index < vecs.size()-1) {
                vecs[index] = vecs.back();
                vecs.pop_back();

                glm::ivec3 origin = glm::vec3(vecs[index]) - __position;
                __tableIndexes[origin.x][origin.y][origin.z] = index;
            }
            else vecs.pop_back();
            if (vecs.empty()) __list.erase(__blockTypes[x][y][z]);

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