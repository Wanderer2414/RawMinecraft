#include "Chunk.h"
#include "Block.h"
#include "DrawingCenter.h"
#include "General.h"
#include "MapCreator.h"

namespace MyCraft {

    Chunk::Chunk(): __isChange(false), __numBlock(0), __numBit(0), __enableQueue(true), __container(0) {
        memset(__waterHeight, 0, 4096);
    }
    Chunk::~Chunk() {
        for (int code: __lightSource) {
            glm::vec3 position;
            unsigned char I = code%256; code /= 256;
            position.z = code%16 + __position.z; code /= 16;
            position.y = code%16 + __position.y; code /= 16;
            position.x = code + __position.x;
            __container->removeLight(position);
        }
        save();
    }
    Chunk* Chunk::Load(ChunkLoader* loader, const std::string& src, const glm::ivec3& position) {
        Chunk* new_chunk = new Chunk();
        new_chunk->__container = loader;
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
            
            unsigned int numState = 0;
            file.read((char*)&numState, sizeof(int));
            for (int i = 0; i<numState; i++) {
                unsigned int index;
                file.read((char*)&index, sizeof(int));
                glm::mat4 state;
                file.read((char*)&state, sizeof(glm::mat4));
                new_chunk->__specialState[index] = state;
            }
            unsigned int size = 0;
            file.read((char*)&size, sizeof(int));
            int *lightsrc = new int[size];
            file.read((char*)lightsrc, sizeof(int)*size);
            for (int i = 0; i<size; i++) {
                int code = lightsrc[i];
                glm::vec3 position;
                new_chunk->__lightSource.insert(code);
                unsigned char I = code%256; code /= 256;
                position.z = code%16 + new_chunk->__position.z; code /= 16;
                position.y = code%16 + new_chunk->__position.y; code /= 16;
                position.x = code + new_chunk->__position.x;
                loader->setLight(position, I);
            }
            delete[] lightsrc;
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
            new_chunk->__container = loader;
            memset(new_chunk->__blockTypes, Air, sizeof(BlockCatogary)*4096);
            if (src == "bin/test/" && position.z < 0) {
                new_chunk->__numBlock = 4096;
                memset(new_chunk->__blockTypes, Grass, 4096);
                if (position.z == -1) {
                    for (glm::ivec3 pos(0,0,15); pos.x<16; pos.x++) {
                        for (pos.y=0;pos.y<16; pos.y++) new_chunk->enableLocalBit(pos);
                    }
                }
            }
        }
        return new_chunk;
    }
    float Chunk::getWaterHeight(const glm::ivec3& position) const {
        glm::ivec3 offset = position-__position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk");
        if (__waterHeight[offset.x][offset.y][offset.z])
            return (__waterHeight[offset.x][offset.y][offset.z]-1)/10.f;
        return 0;
    }
    const glm::ivec3& Chunk::getPosition() const {
        return __position;
    }

    void Chunk::disableList() {
        __enableQueue = false;
    }
    void Chunk::enableList() {
        __enableQueue = true;
        __normal.clear();
        __transparent.clear();
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
    void Chunk::setLight(const glm::ivec3& position, const float& indensity) {
        glm::ivec3 pos = position - __position;
        int index = __tableIndexes[pos.x][pos.y][pos.z];
        if (__bits[pos.x][pos.y][pos.z]) {
            if (isTransparent(__blockTypes[pos.x][pos.y][pos.z]))
                __transparent.setLight(index, indensity*5/255.f);
            else __normal.setLight(index, indensity*5/255.f);
        }
        else if (__waterHeight[pos.x][pos.y][pos.z]) {
            __water.setLight(__tableIndexes[pos.x][pos.y][pos.z], indensity*5/255.f);
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
            unsigned int size = __specialState.size();
            file.write((char*)&size, sizeof(int));
            for (auto& element: __specialState) {
                file.write((char*)&element.first, sizeof(int));
                file.write((char*)&element.second, sizeof(glm::mat4));
            }
            size = __lightSource.size();
            file.write((char*)&size, sizeof(int));
            for (auto& i:__lightSource) file.write((char*)&i, sizeof(int));
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
    glm::mat4 Chunk::getState(const glm::ivec3& position) const {
        glm::ivec3 offset = position-__position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk");
        if (!__bits[offset.x][offset.y][offset.z]) return glm::mat4(1);
        int index = __tableIndexes[offset.x][offset.y][offset.z];
        BlockCatogary type = __blockTypes[offset.x][offset.y][offset.z];
        if (isTransparent(type)) return __transparent.getState(index);
        else return __normal.getState(index);
    }
    void Chunk::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        glm::ivec3 offset = pos - __position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk");

        if (__blockTypes[offset.x][offset.y][offset.z] == type) return;

        if (isLightSource(type)) {
            __lightSource.insert(offset.x*65536+offset.y*4096+offset.z*256 + getLightIndensity(type));
        }
        else if (!type && isLightSource(__blockTypes[offset.x][offset.y][offset.z])) {
            __lightSource.erase(offset.x*65536+offset.y*4096+offset.z*256 + getLightIndensity(__blockTypes[offset.x][offset.y][offset.z]));
        }

        if (type && !__blockTypes[offset.x][offset.y][offset.z]) __numBlock++;
        else if (!type && __blockTypes[offset.x][offset.y][offset.z]) __numBlock--;
        __isChange = true;
        __blockTypes[offset.x][offset.y][offset.z] = type;
        if (__enableQueue && __bits[offset.x][offset.y][offset.z]) {
            __normal.setType(__tableIndexes[offset.x][offset.y][offset.z], type);
        }
    }
    void Chunk::flowWater() {
        __water.increase();
    }
    void Chunk::setWater(const glm::ivec3& position, const glm::vec4& height) {
        glm::ivec3 offset = position - __position;
        if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
            throw std::runtime_error("Out range of chunk");
        float h = 10.f*std::min(height.x, std::min(height.y, std::min(height.z, height.w)))+1;
        if (glm::length(height) && __waterHeight[offset.x][offset.y][offset.z]<h) {
            if (!__waterHeight[offset.x][offset.y][offset.z]) {
                __tableIndexes[offset.x][offset.y][offset.z] = __water.size();
                __water.push(position, height);
            }
            __waterHeight[offset.x][offset.y][offset.z] = h;
            __water.setLight(__tableIndexes[offset.x][offset.y][offset.z] ,__container->getLightIndensity(position)*5.f/255);
        }
    }
    void Chunk::setState(const glm::ivec3& pos, const glm::mat4& state) {
        if (state != glm::mat4(1)) {
            glm::ivec3 offset = pos - __position;
            if (offset.x < 0 || offset.x >= 16 || offset.y < 0 ||  offset.y >= 16 || offset.z < 0 ||  offset.z >= 16) 
                throw std::runtime_error("Out range of chunk");
            BlockCatogary type = __blockTypes[offset.x][offset.y][offset.z];
            glm::mat4 cState(1);
            if (isSpecial(type)) cState = getSpecialState(type);
            cState = state*cState;
            cState[3] += glm::vec4(pos,0);

            __specialState[offset.x*256 + offset.y*16 + offset.z] = state;

            if (__enableQueue && __bits[offset.x][offset.y][offset.z]) {
                if (isTransparent(type)) __transparent.setState(__tableIndexes[offset.x][offset.y][offset.z], cState);
                else __normal.setState(__tableIndexes[offset.x][offset.y][offset.z], cState);
            }
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
            glm::mat4 state(1);
            if (isSpecial(type)) state = getSpecialState(type);
            if (__specialState.find(offset.x*256+offset.y*16+offset.z) != __specialState.end())
                state = __specialState[offset.x*256+offset.y*16+offset.z]*state;
            state[3] += glm::vec4(position, 0);
            if (isTransparent(type)) {
                __tableIndexes[offset.x][offset.y][offset.z] = __transparent.size();
                __transparent.push(position, state, glm::vec4(0,getTransparentConst(type),0,type));
            }
            else {
                __tableIndexes[offset.x][offset.y][offset.z] = __normal.size();
                __normal.push(position, state, glm::vec4(0,0,0,type));
            }
            if (__container) {
                setLight(position, __container->getLightIndensity(position));
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
        if (__specialState.find(offset.x*256+offset.y*16 + offset.z) != __specialState.end())
            __specialState.erase(offset.x*256+offset.y*16 + offset.z);
        if (isTransparent(type)) {
            __transparent.remove(index);
            if (index != __transparent.size()) {
                glm::ivec3 origin = glm::ivec3(__transparent.getPosition(index)) - __position;
                __tableIndexes[origin.x][origin.y][origin.z] = index;
            }
        }
        else {
            __normal.remove(index);
            if (index != __normal.size()) {
                glm::ivec3 origin = glm::ivec3(__normal.getPosition(index)) - __position;
                __tableIndexes[origin.x][origin.y][origin.z] = index;
            }
        }

    }
    void Chunk::glDraw() const {
        DrawingCenter::DrawCubes(__normal);
    }
    void Chunk::glDrawTransparent() const {
        DrawingCenter::DrawCubes(__transparent);
    }
    void Chunk::drawWater() const {
        DrawingCenter::DrawWater(__water);
    }


    DynamicChunk::DynamicChunk(const std::string& src): __chunk(0), __source(src) {}
    DynamicChunk::~DynamicChunk() {
        if (__chunk) {
            __chunk->save();
            delete __chunk;
            __chunk = 0;
        }
    }
    bool DynamicChunk::contains(const glm::ivec3& position) const {
        glm::ivec3 offset = position - __chunk->getPosition();
        return !(offset.x<0 || offset.x>15 || offset.y<0 || offset.y>15 || offset.z < 0 || offset.z > 15);
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
            __chunk = Chunk::Load(this,__source, chunk);
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