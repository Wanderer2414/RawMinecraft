#include "Chunk.h"
#include "Block.h"
#include "DrawingCenter.h"
#include "General.h"
#include "MapCreator.h"

namespace MyCraft {
    Chunk::Chunk(): __isChange(false), __numBlock(0) {}
    Chunk::~Chunk() {}
    Chunk* Chunk::Load(const std::string& src, const glm::ivec3& position) {
        Chunk* new_chunk = new Chunk();
        new_chunk->__source = MapCreator::getFileName(src, position);
        std::ifstream file(new_chunk->__source, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.read((char*)&new_chunk->__position, sizeof(glm::vec3));
            unsigned int sz;
            file.read((char*)&sz, sizeof(int));
            new_chunk->__list.resize(sz);
            unsigned int buffer[128];
            if (sz) {
                file.read((char*)&buffer[0], 128*sizeof(int));
            }
            file.read((char*)&new_chunk->__numBlock, sizeof(int));
            file.read((char*)&new_chunk->__blockTypes[0][0][0], sizeof(BlockCatogary)*4096);
            int index = 0;
            if (sz) {
                for (int i = 0; i<16; i++)
                    for (int j = 0; j<16; j++) {
                        unsigned int data = buffer[i*8+j/2];
                        if (j%2) data >>= 16;
                        else data = data & 0xFFFF;
                        new_chunk->__bits[i][j] = data;
                        while (data) {
                            int k = std::log2(data);
                            glm::ivec4 position(new_chunk->__position, new_chunk->__blockTypes[i][j][k]);
                            position.x += i; position.y += j; position.z += k;
                            new_chunk->__list[index] = position;
                            new_chunk->__tableIndexes[i][j][k] = index;
                            index++;
                            data -= 1<<k;
                        }
                    }
            }
            file.close();
        }
        else {
            new_chunk->__position = position*16;
            memset(new_chunk->__blockTypes, 0, sizeof(BlockCatogary)*4096);
        }
        return new_chunk;
    }
    glm::vec3 Chunk::getPosition() const {
        return __position;
    }
    void Chunk::save() {
        if (!__isChange) return ;
        if (__numBlock) {
            std::ofstream file(__source, std::ios::binary | std::ios::out);
            file.write((char*)&__position, sizeof(glm::vec3));
            unsigned int sz = __list.size();
            file.write((char*)&sz, sizeof(int));
            if (sz) {
                unsigned int buffer[128];
                for (int i = 0; i<16; i++) {
                    for (int j = 0; j<8; j++) {
                        buffer[i*8+j] = __bits[i][j*2].to_ulong() | __bits[i][j*2+1].to_ulong() << 16;
                    }
                }
                file.write((char*)&buffer[0], sizeof(int)*128);
            }
            file.write((char*)&__numBlock, sizeof(int));
            file.write((char*)&__blockTypes[0][0][0], sizeof(BlockCatogary)*4096);
            file.close();
        }
        else MyBase::DeleteFile(__source);
        __isChange = false;
    }
    const BlockCatogary& Chunk::getType(const glm::vec3& pos) const {
        int x = std::floor(pos.x - __position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        return __blockTypes[x][y][z];
    }
    void Chunk::setType(const glm::vec3& pos, const BlockCatogary& type) {
        int x = (int)std::floor(pos.x-__position.x);
        int y = (int)std::floor(pos.y - __position.y);
        int z = (int)std::floor(pos.z - __position.z);
        if (__blockTypes[x][y][z] == type) return;
        if (type && !__blockTypes[x][y][z]) __numBlock++;
        else __numBlock--;
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
            __tableIndexes[x][y][z] = __list.size();
            __list.push_back(glm::vec4(pos, __blockTypes[x][y][z]));
            __bits[x][y][z] = 1;
        };

    }
    void Chunk::disableBit(const glm::vec3& pos) {
        int x = std::floor(pos.x-__position.x);
        int y = std::floor(pos.y - __position.y);
        int z = std::floor(pos.z - __position.z);
        if (__bits[x][y][z]) {
            __isChange = true;
            int index = __tableIndexes[x][y][z];
            __tableIndexes[x][y][z] = -1;
            if (index < __list.size()-1) {
                __list[index] = __list.back();
                __list.pop_back();

                glm::ivec3 origin = glm::ivec3(__list[index]) - __position;
                __tableIndexes[origin.x][origin.y][origin.z] = index;
            }
            else __list.pop_back();

            __bits[x][y][z] = 0;
        }
    }
    void Chunk::glDraw() const {
        DrawingCenter::getInstance().BindCube();
        DrawingCenter::getInstance().DrawCubes((void*)__list.data(), __list.size());
    }
}