#include "Chunk.h"
#include "Block.h"
#include "ChunkLoader.h"
#include "DrawingCenter.h"
#include "General.h"
#include "MapCreator.h"
#include <stdexcept>

namespace MyCraft {
    Chunk::Chunk(): __isChange(false), __numBlock(0), __numBit(0), __enableQueue(true) {}
    Chunk::~Chunk() {}
    Chunk* Chunk::Load(const std::string& src, const glm::ivec3& position) {
        Chunk* new_chunk = new Chunk();
        new_chunk->__source = MapCreator::getFileName(src, position);
        std::ifstream file(new_chunk->__source, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.read((char*)&new_chunk->__position, sizeof(glm::ivec3));
            file.read((char*)&new_chunk->__numBit, sizeof(int));
            new_chunk->__list.resize(new_chunk->__numBit);
            unsigned int buffer[128];
            if (new_chunk->__numBit) {
                file.read((char*)&buffer[0], 128*sizeof(int));
            }
            file.read((char*)&new_chunk->__numBlock, sizeof(int));
            file.read((char*)&new_chunk->__blockTypes[0][0][0], sizeof(BlockCatogary)*4096);
            file.close();
            int index = 0;
            if (new_chunk->__numBit) {
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
        }
        else {
            new_chunk->__position = position*16;
            memset(new_chunk->__blockTypes, 0, sizeof(BlockCatogary)*4096);
        }
        return new_chunk;
    }
    glm::ivec3 Chunk::getPosition() const {
        return __position;
    }

    void Chunk::disableList() {
        __enableQueue = false;
    }
    void Chunk::enableList() {
        __enableQueue = true;
        __list.clear();
        __list.resize(__numBit);
        int index = 0;
        for (int x = 0; x<16; x++) {
            for (int y = 0; y<16; y++) {
                for (int z = 0; z<16; z++) {
                    if (__bits[x][y][z]) {
                        __list[index] = glm::ivec4(x,y,z, __blockTypes[x][y][z]);
                        __tableIndexes[x][y][z] = index;
                        index++;
                    }
                }
            }
        }
    }
    void Chunk::save() {
        if (!__isChange) return ;
        if (__numBlock) {
            std::ofstream file(__source, std::ios::binary | std::ios::out);
            file.write((char*)&__position, sizeof(glm::vec3));
            file.write((char*)&__numBit, sizeof(int));
            if (__numBit) {
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
    const BlockCatogary& Chunk::getType(const glm::ivec3& pos) const {
        glm::ivec3 offset = pos - __position;
        return __blockTypes[offset.x][offset.y][offset.z];
    }

    const BlockCatogary& Chunk::getLocalType(const glm::ivec3& offset) const {
        return __blockTypes[offset.x][offset.y][offset.z];
    }

    void Chunk::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        glm::ivec3 offset = pos - __position;
        setLocalType(offset, type);
    }

    void Chunk::setLocalType(const glm::ivec3& offset, const BlockCatogary& type) {
        if (__blockTypes[offset.x][offset.y][offset.z] == type) return;
        if (type && !__blockTypes[offset.x][offset.y][offset.z]) __numBlock++;
        else if (!type && __blockTypes[offset.x][offset.y][offset.z]) __numBlock--;
        __isChange = true;
        __blockTypes[offset.x][offset.y][offset.z] = type;
        if (__enableQueue && __bits[offset.x][offset.y][offset.z]) {
            __list[__tableIndexes[offset.x][offset.y][offset.z]].w = type;
        }
    }
    std::bitset<16>::reference Chunk::getBit(const glm::ivec3& pos) {
        glm::ivec3 offset = pos - __position;
        return __bits[offset.x][offset.y][offset.z];
    }
    std::bitset<16>::reference Chunk::getLocalBit(const glm::ivec3& offset) {
        return __bits[offset.x][offset.y][offset.z];
    }
    void Chunk::enableBit(const glm::ivec3& pos) {
        glm::ivec3 offset = pos - __position;
        enableLocalBit(offset);
    }
    void Chunk::enableLocalBit(const glm::ivec3& offset) {
        if (!__bits[offset.x][offset.y][offset.z]) {
            __isChange = true;
            __numBit++;
            __bits[offset.x][offset.y][offset.z] = 1;
            if (__enableQueue) {
                __tableIndexes[offset.x][offset.y][offset.z] = __list.size();
                __list.push_back(glm::vec4(offset+__position, __blockTypes[offset.x][offset.y][offset.z]));
            }
        };
    }
    void Chunk::disableBit(const glm::ivec3& pos) {
        glm::ivec3 offset = pos - __position;
        disableLocalBit(offset);
    }
    void Chunk::disableLocalBit(const glm::ivec3& offset) {
        if (__bits[offset.x][offset.y][offset.z]) {
            __isChange = true;
            if (__enableQueue) {
                int index = __tableIndexes[offset.x][offset.y][offset.z];
                __tableIndexes[offset.x][offset.y][offset.z] = -1;
                if (index < __list.size()-1) {
                    __list[index] = __list.back();
                    __list.pop_back();

                    glm::ivec3 origin = glm::ivec3(__list[index]) - __position;
                    __tableIndexes[origin.x][origin.y][origin.z] = index;
                }
                else __list.pop_back();
            }
            __numBit--;
            __bits[offset.x][offset.y][offset.z] = 0;
        }
    }
    void Chunk::glDraw() const {
        DrawingCenter::BindCube();
        DrawingCenter::DrawCubes((void*)__list.data(), __list.size());
    }


    DynamicChunk::DynamicChunk(const std::string& src): __chunk(0), __source(src) {}
    DynamicChunk::~DynamicChunk() {
        if (__chunk) {
            __chunk->save();
            delete __chunk;
            __chunk = 0;
        }
    }
    glm::ivec3 DynamicChunk::getPosition() const {
        if (__chunk) return __chunk->getPosition();
        else throw std::runtime_error("Chunk is nullptr!");
    }
    void DynamicChunk::Load(const glm::ivec3& position) {
        if (__chunk) {
            __chunk->save();
            delete __chunk;
            __chunk = 0;
        }
        glm::ivec3 chunk(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        __chunk = Chunk::Load(__source, chunk);
    }
    void DynamicChunk::disableList() {
        if (__chunk) __chunk->disableList();
        else throw std::runtime_error("Chunk is nullptr!");
    }
    void DynamicChunk::enableList() {
        if (__chunk) __chunk->enableList();
        else throw std::runtime_error("Chunk is nullptr!");
    }
    void DynamicChunk::save() {
        if (__chunk) __chunk->save();
        else throw std::runtime_error("Chunk is nullptr!");
    }
    const BlockCatogary& DynamicChunk::getType(const glm::ivec3& pos) {
        if (__chunk) {
            glm::ivec3 offset = pos - __chunk->getPosition();
            if (offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15) {
                __chunk->save();
                delete __chunk;
                __chunk = 0;
            }
        }
        if (!__chunk) {
            glm::ivec3 chunk(floor(pos.x/16.f), floor(pos.y/16.f), floor(pos.z/16.f));
            __chunk = Chunk::Load(__source, chunk);
        }
        return __chunk->getType(pos);
    }

    const BlockCatogary& DynamicChunk::getLocalType(const glm::ivec3& offset) {
        if (__chunk) {
            glm::ivec3 position = offset + __chunk->getPosition();
            return getType(position);
        }
        else throw std::runtime_error("Chunk is nullptr!");
    }

    void DynamicChunk::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        if (__chunk) {
            glm::ivec3 offset = pos - __chunk->getPosition();
            if (offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15) {
                __chunk->save();
                delete __chunk;
                __chunk = 0;
            }
        }
        if (!__chunk) {
            glm::ivec3 chunk(floor(pos.x/16.f), floor(pos.y/16.f), floor(pos.z/16.f));
            __chunk = Chunk::Load(__source, chunk);
        }
        __chunk->setType(pos, type);
    }

    void DynamicChunk::setLocalType(const glm::ivec3& offset, const BlockCatogary& type) {
        if (__chunk) {
            glm::ivec3 position = offset + __chunk->getPosition();
            setType(position, type);
        }
        else throw std::runtime_error("Chunk is nullptr!");
    }
    std::bitset<16>::reference DynamicChunk::getBit(const glm::ivec3& pos) {
        if (__chunk) {
            glm::ivec3 offset = pos - __chunk->getPosition();
            if (offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15) {
                __chunk->save();
                delete __chunk;
                __chunk = 0;
            }
        }
        if (!__chunk) {
            glm::ivec3 chunk(floor(pos.x/16.f), floor(pos.y/16.f), floor(pos.z/16.f));
            __chunk = Chunk::Load(__source, chunk);
        }
        return __chunk->getBit(pos);
    }
    std::bitset<16>::reference DynamicChunk::getLocalBit(const glm::ivec3& offset) {
        if (__chunk) {
            glm::ivec3 position = offset + __chunk->getPosition();
            return getBit(position);
        }
        else throw std::runtime_error("Chunk is nullptr!");
    }
    void DynamicChunk::place(const glm::ivec3& pos, const BlockCatogary& type) {
        if (!type) {
            if (getType(pos)) {
                disableBit(pos);
                setType(pos, BlockCatogary::Air);
                glm::vec3 position = pos;
                position.x--;
                if (getBit(position) && getType(position)) enableBit(position);
                position.x += 2;
                if (!getBit(position) && getType(position)) enableBit(position);

                position -= glm::vec3(1, 1, 0);
                if (!getBit(position) && getType(position)) enableBit(position);
                position.y += 2;
                if (!getBit(position) && getType(position)) enableBit(position);
                
                position -= glm::vec3(0, 1, 1);
                if (!getBit(position) && getType(position)) enableBit(position);

                position.z += 2;
                if (!getBit(position) && getType(position)) enableBit(position);
            }
        }
        else {
            if (!getType(pos)) {
                setType(pos, type);
                enableBit(pos);
                glm::vec3 position = pos;
                position.x--;
                __disableBit(position);
                position.x += 2;
                __disableBit(position);

                position -= glm::vec3(1, 1, 0);
                __disableBit(position);
                position.y += 2;
                __disableBit(position);
                
                position -= glm::vec3(0, 1, 1);
                __disableBit(position);
                position.z += 2;
                __disableBit(position);
            }
            else {
                disableBit(pos);
                setType(pos, type);
                enableBit(pos);
            }
        }
    }
    void DynamicChunk::enableBit(const glm::ivec3& pos) {
        if (__chunk) {
            glm::ivec3 offset = pos - __chunk->getPosition();
            if (offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15) {
                __chunk->save();
                delete __chunk;
                __chunk = 0;
            }
        }
        if (!__chunk) {
            glm::ivec3 chunk(floor(pos.x/16.f), floor(pos.y/16.f), floor(pos.z/16.f));
            __chunk = Chunk::Load(__source, chunk);
        }
        __chunk->enableBit(pos);
    }
    void DynamicChunk::enableLocalBit(const glm::ivec3& offset) {
        if (__chunk) {
            glm::ivec3 position = offset + __chunk->getPosition();
            enableBit(position);
        }
        else throw std::runtime_error("Chunk is nullptr!");
    }
    void DynamicChunk::disableBit(const glm::ivec3& pos) {
        if (__chunk) {
            glm::ivec3 offset = pos - __chunk->getPosition();
            if (offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15) {
                __chunk->save();
                delete __chunk;
                __chunk = 0;
            }
        }
        if (!__chunk) {
            glm::ivec3 chunk(floor(pos.x/16.f), floor(pos.y/16.f), floor(pos.z/16.f));
            __chunk = Chunk::Load(__source, chunk);
        }
        __chunk->disableBit(pos);
    }
    void DynamicChunk::disableLocalBit(const glm::ivec3& offset) {
        if (__chunk) {
            glm::ivec3 position = offset + __chunk->getPosition();
            disableBit(position);
        }
        else throw std::runtime_error("Chunk is nullptr!");
    }
    void DynamicChunk::glDraw() const {
        if (__chunk) __chunk->glDraw();
        else throw std::runtime_error("Chunk is nullptr!");
    }
    void DynamicChunk::__disableBit(const glm::ivec3& pos) {
        if (!getBit(pos)) return;
        glm::ivec3 position = pos;
        position.x -= 1;
        if (!getType(position)) return;
        
        position.x += 2;
        if (!getType(position)) return ;
        
        position -= glm::vec3(1, 1, 0);
        if (!getType(position)) return ;
        position.y += 2;
        if (!getType(position)) return ;

        position -= glm::vec3(0, 1, 1);
        if (!getType(position)) return ;
        position.z += 2;
        if (!getType(position)) return ;

        disableBit(pos);
    }
}