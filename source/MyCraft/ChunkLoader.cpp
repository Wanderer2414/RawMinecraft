#include "ChunkLoader.h"
namespace MyCraft {
    ChunkLoader::ChunkLoader(const std::string& src): __isLoaded(false), __sourceFolder(src) {
        __chunks.resize(world_side*world_side*world_side, 0);
        __chunkPositions.resize(world_side*world_side*world_side);
        for (int i = 0; i<world_side; i++)
            for (int j = 0; j<world_side; j++) 
                for (int k = 0; k<world_side; k++) 
                    __chunkIndices[i][j][k] = i*world_side*world_side + j*world_side + k;
        playerAt({0,0,0});
    }
    ChunkLoader::~ChunkLoader() {
        for (auto& chunk:__chunks) {
            chunk->save();
            delete chunk;
        }
    }
    const std::vector<glm::ivec4>& ChunkLoader::getChunks() const {
        return __chunkPositions;
    }
    void ChunkLoader::playerAt(const glm::ivec3& pos) {
        glm::ivec3 position(__chunkPosition(pos) - glm::ivec3(world_side/2));
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

    void ChunkLoader::__loadDefault() {
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
    void ChunkLoader::__moveSubPositiveX(ChunkLoader* chunkLoader, const int& j, const int& k) {
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
    void ChunkLoader::__movePositiveX() {
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
    void ChunkLoader::__moveSubNegativeX(ChunkLoader* chunkLoader, const int& j, const int& k) {
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
    void ChunkLoader::__moveNegativeX() {
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
    void ChunkLoader::__moveSubPositiveY(ChunkLoader* chunkLoader, const int& i, const int& k) {
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
    void ChunkLoader::__movePositiveY() {
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
    void ChunkLoader::__moveSubNegativeY(ChunkLoader* chunkLoader, const int& i, const int& k) {
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
    void ChunkLoader::__moveNegativeY() {
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
    void ChunkLoader::__moveSubPositiveZ(ChunkLoader* chunkLoader, const int& i, const int& j) {
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
    void ChunkLoader::__movePositiveZ() {
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
    void ChunkLoader::__moveSubNegativeZ(ChunkLoader* chunkLoader, const int& i, const int& j) {
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
    void ChunkLoader::__moveNegativeZ() {
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
    glm::ivec3 ChunkLoader::__chunkPosition(const glm::ivec3& block) const {
        return {floor(block.x/16.f), floor(block.y/16.f), floor(block.z/16.f)};
    }
    Chunk& ChunkLoader::operator[](const glm::ivec3& pos) {
        glm::ivec3 offset = __chunkPosition(pos) - __position;
        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    const Chunk& ChunkLoader::operator[](const glm::ivec3& pos) const {
        glm::ivec3 offset = __chunkPosition(pos) - __position;
        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    void ChunkLoader::glDraw() const {
        glLineWidth(0);
        for (auto& chunk:__chunks) chunk->glDraw();
    }

}