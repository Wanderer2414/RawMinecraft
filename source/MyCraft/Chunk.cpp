#include "Chunk.h"
#include "DrawingCenter.h"
#include "General.h"
#include "MapCreator.h"

namespace MyCraft {

    Chunk::Chunk(): __isChange(false), __numBlock(0), __numBit(0), __enableQueue(true) {}
    Chunk::~Chunk() {
        save();
    }
    Chunk* Chunk::Load(const std::string& src, const glm::ivec3& position) {
        Chunk* new_chunk = new Chunk();
        new_chunk->__source = MapCreator::getFileName(src, position);
        std::ifstream file(new_chunk->__source, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            file.read((char*)&new_chunk->__position, sizeof(glm::ivec3));
            file.read((char*)&new_chunk->__numBit, sizeof(int));
            unsigned int buffer[128];
            if (new_chunk->__numBit) {
                file.read((char*)&buffer[0], 128*sizeof(int));
            }
            file.read((char*)&new_chunk->__numBlock, sizeof(int));
            file.read((char*)&new_chunk->__blockTypes[0][0][0], sizeof(BlockCatogary)*4096);
            file.close();
            if (new_chunk->__numBit) {
                for (int i = 0; i<16; i++)
                    for (int j = 0; j<16; j++) {
                        unsigned int data = buffer[i*8+j/2];
                        if (j%2) data >>= 16;
                        else data = data & 0xFFFF;
                        new_chunk->__bits[i][j] = data;
                        while (data) {
                            int k = std::log2(data);
                            glm::ivec3 position(new_chunk->__position);
                            position.x += i; position.y += j; position.z += k;
                            new_chunk->__add(position);
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
    const glm::ivec3& Chunk::getPosition() const {
        return __position;
    }

    void Chunk::disableList() {
        __enableQueue = false;
    }
    void Chunk::enableList() {
        __enableQueue = true;
        __list.clear();
        __transparentList.clear();
        for (int x = 0; x<16; x++) {
            for (int y = 0; y<16; y++) {
                for (int z = 0; z<16; z++) {
                    if (__bits[x][y][z]) {
                        __add(glm::ivec3(x,y,z) + __position);
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

    const BlockCatogary& Chunk::getType(const glm::ivec3& pos) {
        glm::ivec3 offset = pos - __position;
        return __blockTypes[offset.x][offset.y][offset.z];
    }

    void Chunk::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        glm::ivec3 offset = pos - __position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk");
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

    void Chunk::enableBit(const glm::ivec3& pos) {
        glm::ivec3 offset = pos - __position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk!");
        if (!__bits[offset.x][offset.y][offset.z]) {
            __isChange = true;
            __numBit++;
            __bits[offset.x][offset.y][offset.z] = 1;
            if (__enableQueue) __add(pos);
        };
    }
    void Chunk::disableBit(const glm::ivec3& pos) {
        glm::ivec3 offset = pos - __position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk!");
        if (__bits[offset.x][offset.y][offset.z]) {
            __isChange = true;
            if (__enableQueue) __remove(pos);
            __numBit--;
            __bits[offset.x][offset.y][offset.z] = 0;
        }
    }

    void Chunk::__add(const glm::ivec3& position) {
        glm::ivec3 offset = position - __position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk!");
        if (__blockTypes[offset.x][offset.y][offset.z] && __bits[offset.x][offset.y][offset.z]) {
            BlockCatogary type = __blockTypes[offset.x][offset.y][offset.z];
            if (isTransparent(type)) {
                __tableIndexes[offset.x][offset.y][offset.z] = __transparentList.size();
                __transparentList.push_back(glm::ivec4(position, type));
            }
            else {
                __tableIndexes[offset.x][offset.y][offset.z] = __list.size();
                __list.push_back(glm::ivec4(position, type));
            }
        }
    }
    void Chunk::__remove(const glm::ivec3& position) {
        glm::ivec3 offset = position - __position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk!");
        BlockCatogary type = __blockTypes[offset.x][offset.y][offset.z];
        int index = __tableIndexes[offset.x][offset.y][offset.z];
        __tableIndexes[offset.x][offset.y][offset.z] = -1;
        if (isTransparent(type)) {
            if (index < __transparentList.size()-1) {
                __transparentList[index] = __transparentList.back();
                __transparentList.pop_back();

                glm::ivec3 origin = glm::ivec3(__transparentList[index]) - __position;
                __tableIndexes[origin.x][origin.y][origin.z] = index;
            }
            else __transparentList.pop_back();
        }
        else {
            if (index < __list.size()-1) {
                __list[index] = __list.back();
                __list.pop_back();

                glm::ivec3 origin = glm::ivec3(__list[index]) - __position;
                __tableIndexes[origin.x][origin.y][origin.z] = index;
            }
            else __list.pop_back();
        }

    }
    void Chunk::glDraw() const {
        DrawingCenter::BindCube();
        DrawingCenter::DrawCubes((void*)__list.data(), __list.size());
    }
    void Chunk::glDrawTransparent() const {
        DrawingCenter::BindCube();
        DrawingCenter::DrawCubes((void*)__transparentList.data(), __transparentList.size());
    }


    DynamicChunk::DynamicChunk(const std::string& src): __chunk(0), __source(src) {}
    DynamicChunk::~DynamicChunk() {
        if (__chunk) {
            __chunk->save();
            delete __chunk;
            __chunk = 0;
        }
    }
    const glm::ivec3& DynamicChunk::getPosition() const {
        if (__chunk) return __chunk->getPosition();
        else throw std::runtime_error("Chunk is nullptr!");
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

    Chunk& DynamicChunk::getChunk(const glm::ivec3& position) {
        if (__chunk) {
            glm::ivec3 offset = position - __chunk->getPosition();
            if (offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15) {
                __chunk->save();
                delete __chunk;
                __chunk = 0;
            }
        }
        if (!__chunk) {
            glm::ivec3 chunk(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
            __chunk = Chunk::Load(__source, chunk);
        }
        return *__chunk;
    }
    const Chunk& DynamicChunk::getChunk(const glm::ivec3& position) const {
        if (__chunk) {
            glm::ivec3 offset = position - __chunk->getPosition();
            if (offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15) 
                throw std::runtime_error("Out of range!");
        }
        if (!__chunk) 
            throw std::runtime_error("Out of range!");
        return *__chunk;
    }
    void DynamicChunk::glDraw() const {
        if (__chunk) __chunk->glDraw();
        else throw std::runtime_error("Chunk is nullptr!");
    }
}