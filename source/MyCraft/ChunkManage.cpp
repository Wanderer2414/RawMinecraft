#include "ChunkManage.h"
#include "ChunkBase.h"
#include "Chunk.h"
namespace MyCraft {
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
    void ChunkManage::glDrawTransparent() const {
        glLineWidth(0);
        for (auto& chunk:__chunks) chunk->glDrawTransparent();
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

}