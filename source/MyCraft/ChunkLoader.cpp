#include "ChunkLoader.h"
#include "DrawingCenter.h"
#include "General.h"
#include "MapCreator.h"
#include <stdexcept>
namespace MyCraft {

    const BlockCatogary& ChunkObject::getLocalType(const glm::ivec3& offset) const {
        glm::ivec3 position = offset + getPosition();
        return getType(position);
    }
    const BlockCatogary& ChunkObject::getLocalType(const glm::ivec3& offset) {
        glm::ivec3 position = offset + getPosition();
        return getType(position);
    }

    std::bitset<16>::reference ChunkObject::getLocalBit(const glm::ivec3& offset) {
        glm::ivec3 position = offset + getPosition();
        return getBit(position);
    }
    void ChunkObject::enableLocalBit(const glm::ivec3& offset) {
        glm::ivec3 position = offset + getPosition();
        enableBit(position);
    }
    void ChunkObject::disableLocalBit(const glm::ivec3& offset) {
        glm::ivec3 position = offset + getPosition();
        disableBit(position);
    }
    void ChunkObject::setLocalType(const glm::ivec3& offset, const BlockCatogary& type) {
        glm::ivec3 position = offset + getPosition();
        setType(position, type);
    }

    void ChunkLoader::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        if (!type) {
            if (getType(pos)) {
                getChunk(pos).disableBit(pos);
                getChunk(pos).setType(pos, BlockCatogary::Air);
                glm::vec3 position = pos;
                position.x--;
                if (!getBit(position) && getType(position)) enableBit(position);
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
                getChunk(pos).setType(pos, type);
                getChunk(pos).enableBit(pos);
                glm::vec3 position = pos;
                position.x--;
                disableBit(position);
                position.x += 2;
                disableBit(position);

                position -= glm::vec3(1, 1, 0);
                disableBit(position);
                position.y += 2;
                disableBit(position);
                
                position -= glm::vec3(0, 1, 1);
                disableBit(position);
                position.z += 2;
                disableBit(position);
            }
            else {
                getChunk(pos).setType(pos, type);
            }
        }
    }
    void ChunkLoader::enableBit(const glm::ivec3& position) {
        getChunk(position).enableBit(position);
    }
    void ChunkLoader::disableBit(const glm::ivec3& pos) {
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

        getChunk(pos).disableBit(pos);
    }


    const BlockCatogary& ChunkLoader::getType(const glm::ivec3& position) const  {
        return getChunk(position).getType(position);
    }
    const BlockCatogary& ChunkLoader::getType(const glm::ivec3& position) {
        return getChunk(position).getType(position);
    }
    std::bitset<16>::reference ChunkLoader::getBit(const glm::ivec3& position) {
        return getChunk(position).getBit(position);
    }

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
    const glm::ivec3& Chunk::getPosition() const {
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

    const BlockCatogary& Chunk::getType(const glm::ivec3& pos) {
        glm::ivec3 offset = pos - __position;
        return __blockTypes[offset.x][offset.y][offset.z];
    }

    void Chunk::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        glm::ivec3 offset = pos - __position;
        if (offset.x >= 16 || offset.y >= 16 || offset.z >= 16)
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
        if (offset.x >= 16 || offset.y >= 16 || offset.z >= 16) 
            throw std::runtime_error("Out range of chunk!");
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
        if (offset.x >= 16 || offset.y >= 16 || offset.z >= 16) 
            throw std::runtime_error("Out range of chunk!");
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

    ChunkManage::ChunkManage(const std::string& src): __isLoaded(false), __sourceFolder(src) {
        __chunks.resize(world_side*world_side*world_side, 0);
        __chunkPositions.resize(world_side*world_side*world_side);
        for (int i = 0; i<world_side; i++)
            for (int j = 0; j<world_side; j++) 
                for (int k = 0; k<world_side; k++) 
                    __chunkIndices[i][j][k] = i*world_side*world_side + j*world_side + k;
        playerAt({0,0,0});
    }
    ChunkManage::~ChunkManage() {
        for (auto& chunk:__chunks) {
            chunk->save();
            delete chunk;
        }
    }
    const std::vector<glm::ivec4>& ChunkManage::getChunks() const {
        return __chunkPositions;
    }
    void ChunkManage::playerAt(const glm::ivec3& pos) {
        glm::ivec3 position(pos.x/16 - world_side/2, pos.y/16 - world_side/2, pos.z/16 - world_side/2);
        glm::ivec3 delta = position - __position;
        float length = glm::length((glm::vec3)delta);
        if (!__isLoaded || length>2) {
            if (!__isLoaded) __isLoaded = true;
            else {
                for (auto& chunk: __chunks) {
                    chunk->save();
                    delete chunk;
                }
            }
            __position = position;
            __loadDefault();
        }
        else if (length>=1) {
            if (delta.x>0) __movePositiveX();
            else if (delta.x<0) __moveNegativeX();

            if (delta.y>0) __movePositiveY();
            else if (delta.y<0) __moveNegativeY();

            if (delta.z>0) __movePositiveZ();
            else if (delta.z<0) __moveNegativeZ();
        }
    }

    void ChunkManage::__loadDefault() {
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                for (int k = 0; k<world_side; k++) {
                    glm::ivec3 origin = __position + glm::ivec3(i,j,k);
                    __chunks[__chunkIndices[i][j][k]] = Chunk::Load(__sourceFolder, origin);
                    __chunkPositions[__chunkIndices[i][j][k]] = glm::ivec4(origin*16, 16);
                }
            }
        }
    }
    void ChunkManage::__moveSubPositiveX(ChunkManage* chunkLoader, const int& j, const int& k) {
        //Delete outside chunk
        chunkLoader->__chunks[chunkLoader->__chunkIndices[0][j][k]]->save();
        delete chunkLoader->__chunks[chunkLoader->__chunkIndices[0][j][k]];
        //Transform chunk indices table
        int tmp = chunkLoader->__chunkIndices[0][j][k];
        for (int i = 0; i<world_side-1; i++) 
            chunkLoader->__chunkIndices[i][j][k] = chunkLoader->__chunkIndices[i+1][j][k];
        chunkLoader->__chunkIndices[world_side-1][j][k] = tmp;
        //Load new chunk
        glm::ivec3  origin =  chunkLoader->__position + glm::ivec3(world_side-1, j,k);
        chunkLoader->__chunks[tmp] = Chunk::Load(chunkLoader->__sourceFolder, origin);
        chunkLoader->__chunkPositions[tmp] = glm::ivec4(16*origin, 16);
    }
    void ChunkManage::__movePositiveX() {
        __position.x++;
        std::thread* threads[world_side*world_side];
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                threads[j*world_side+k] = new std::thread(__moveSubPositiveX, this, j, k);
            }
        }
        for (int i = 0; i<world_side*world_side; i++) {
            threads[i]->join();
            delete threads[i];
        }
    }
    void ChunkManage::__moveSubNegativeX(ChunkManage* chunkLoader, const int& j, const int& k) {
        //Delete outside chunk
        chunkLoader->__chunks[chunkLoader->__chunkIndices[world_side-1][j][k]]->save();
        delete chunkLoader->__chunks[chunkLoader->__chunkIndices[world_side-1][j][k]];
        //Transform chunk indices table
        int tmp = chunkLoader->__chunkIndices[world_side-1][j][k];
        for (int i = world_side-1; i>0; i--) 
            chunkLoader->__chunkIndices[i][j][k] = chunkLoader->__chunkIndices[i-1][j][k];
        chunkLoader->__chunkIndices[0][j][k] = tmp;
        //Load new chunk
        glm::ivec3 origin =  chunkLoader->__position + glm::ivec3(0, j,k);
        chunkLoader->__chunks[tmp] = Chunk::Load(chunkLoader->__sourceFolder, origin);
        chunkLoader->__chunkPositions[tmp] = glm::ivec4(16*origin, 16);
    }
    void ChunkManage::__moveNegativeX() {
        __position.x--;
        std::thread* threads[world_side*world_side];
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                threads[j*world_side+k] = new std::thread(__moveSubNegativeX, this, j, k);
            }
        }
        for (int i = 0; i<world_side*world_side; i++) {
            threads[i]->join();
            delete threads[i];
        }
    }
    void ChunkManage::__moveSubPositiveY(ChunkManage* chunkLoader, const int& i, const int& k) {
        //Delete outside chunk
        chunkLoader->__chunks[chunkLoader->__chunkIndices[i][0][k]]->save();
        delete chunkLoader->__chunks[chunkLoader->__chunkIndices[i][0][k]];
        //Transform chunk indices table
        int tmp = chunkLoader->__chunkIndices[i][0][k];
        for (int j = 0; j<world_side-1; j++) 
            chunkLoader->__chunkIndices[i][j][k] = chunkLoader->__chunkIndices[i][j+1][k];
        chunkLoader->__chunkIndices[i][world_side-1][k] = tmp;
        //Load new chunk
        glm::ivec3  origin =  chunkLoader->__position + glm::ivec3(i, world_side-1,k);
        chunkLoader->__chunks[tmp] = Chunk::Load(chunkLoader->__sourceFolder, origin);
        chunkLoader->__chunkPositions[tmp] = glm::ivec4(16*origin, 16);
    }
    void ChunkManage::__movePositiveY() {
        __position.y++;
        std::thread* threads[world_side*world_side];
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                threads[i*world_side+k] = new std::thread(__moveSubPositiveY, this, i, k);
            }
        }
        for (int i = 0; i<world_side*world_side; i++) {
            threads[i]->join();
            delete threads[i];
        }
    }
    void ChunkManage::__moveSubNegativeY(ChunkManage* chunkLoader, const int& i, const int& k) {
        //Delete outside chunk
        int index = chunkLoader->__chunkIndices[i][world_side-1][k];
        chunkLoader->__chunks[index]->save();
        delete chunkLoader->__chunks[index];

        //Transform chunk indices table
        for (int j = world_side-1; j>0; j--) 
            chunkLoader->__chunkIndices[i][j][k] = chunkLoader->__chunkIndices[i][j-1][k];
        chunkLoader->__chunkIndices[i][0][k] = index;

        //Load new chunk
        glm::ivec3  origin =  chunkLoader->__position + glm::ivec3(i, 0,k);
        chunkLoader->__chunks[index] = Chunk::Load(chunkLoader->__sourceFolder, origin);
        chunkLoader->__chunkPositions[index] = glm::ivec4(16*origin, 16);
    }
    void ChunkManage::__moveNegativeY() {
        __position.y--;
        std::thread* threads[world_side*world_side];
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                threads[i*world_side+k] = new std::thread(__moveSubNegativeY, this, i, k);
            }
        }
        for (int i = 0; i<world_side*world_side; i++) {
            threads[i]->join();
            delete threads[i];
        }
    }
    void ChunkManage::__moveSubPositiveZ(ChunkManage* chunkLoader, const int& i, const int& j) {
        //Delete outside chunk
        chunkLoader->__chunks[chunkLoader->__chunkIndices[i][j][0]]->save();
        delete chunkLoader->__chunks[chunkLoader->__chunkIndices[i][j][0]];
        //Transform chunk indices table
        int tmp = chunkLoader->__chunkIndices[i][j][0];
        for (int k = 0; k<world_side-1; k++) 
            chunkLoader->__chunkIndices[i][j][k] = chunkLoader->__chunkIndices[i][j][k+1];
        chunkLoader->__chunkIndices[i][j][world_side-1] = tmp;
        //Load new chunk
        glm::ivec3  origin =  chunkLoader->__position + glm::ivec3(i, j,world_side-1);
        chunkLoader->__chunks[tmp] = Chunk::Load(chunkLoader->__sourceFolder, origin);
        chunkLoader->__chunkPositions[tmp] = glm::ivec4(16*origin, 16);
    }
    void ChunkManage::__movePositiveZ() {
        __position.z++;
        std::thread *threads[world_side*world_side];
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                threads[i*world_side+j] = new std::thread(__moveSubPositiveZ, this, i, j);
            }
        }
        for (int i = 0; i<world_side*world_side; i++) {
            threads[i]->join();
            delete threads[i];
        }
    }
    void ChunkManage::__moveSubNegativeZ(ChunkManage* chunkLoader, const int& i, const int& j) {
        //Delete outside chunk
        chunkLoader->__chunks[chunkLoader->__chunkIndices[i][j][world_side-1]]->save();
        delete chunkLoader->__chunks[chunkLoader->__chunkIndices[i][j][world_side-1]];
        //Transform chunk indices table
        int tmp = chunkLoader->__chunkIndices[i][j][world_side-1];
        for (int k = world_side-1; k>0; k--) 
            chunkLoader->__chunkIndices[i][j][k] = chunkLoader->__chunkIndices[i][j][k-1];
        chunkLoader->__chunkIndices[i][j][0] = tmp;
        //Load new chunk
        glm::ivec3  origin =  chunkLoader->__position + glm::ivec3(i, j,0);
        chunkLoader->__chunks[tmp] = Chunk::Load(chunkLoader->__sourceFolder, origin);
        chunkLoader->__chunkPositions[tmp] = glm::ivec4(16*origin, 16);
    }
    void ChunkManage::__moveNegativeZ() {
        __position.z--;
        std::thread* threads[world_side*world_side];
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                threads[i*world_side+j] = new std::thread(__moveSubNegativeZ, this, i, j);
            }
        }
        for (int i = 0; i<world_side*world_side; i++) {
            threads[i]->join();
            delete threads[i];
        }
    }
    void ChunkManage::glDraw() const {
        glLineWidth(0);
        for (auto& chunk:__chunks) chunk->glDraw();
    }
    Chunk& ChunkManage::getChunk(const glm::ivec3& position) {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= getPosition();
        if (offset.x >= world_side || offset.y >= world_side || offset.z >= world_side)
            throw std::runtime_error("Out range of storage!");
        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    const Chunk& ChunkManage::getChunk(const glm::ivec3& position) const {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= getPosition();
        if (offset.x >= world_side || offset.y >= world_side || offset.z >= world_side)
            throw std::runtime_error("Out range of storage!");
        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    const glm::ivec3& ChunkManage::getPosition() const {
        return __position;
    };


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