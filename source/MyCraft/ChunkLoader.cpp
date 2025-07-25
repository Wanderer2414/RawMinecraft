#include "ChunkLoader.h"
#include "Block.h"
#include "MapCreator.h"
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
    const std::vector<glm::vec4>& ChunkLoader::getChunks() const {
        return __chunkPositions;
    }
    void ChunkLoader::playerAt(const glm::vec3& pos) {
        glm::ivec3 position(floor(pos.x/16)-floor(world_side/2.f), floor(pos.y/16)-floor(world_side/2.f),  floor(pos.z/16) - floor(world_side/2.f));
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
                    __chunkPositions[__chunkIndices[i][j][k]] = glm::vec4(origin*16, 1);
                }
            }
        }
    }
    void ChunkLoader::__movePositiveX() {
        __position.x++;
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                __chunks[__chunkIndices[0][j][k]]->save();
                delete __chunks[__chunkIndices[0][j][k]];
                //Transform chunk indices table
                int tmp = __chunkIndices[0][j][k];
                for (int i = 0; i<world_side-1; i++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i+1][j][k];
                __chunkIndices[world_side-1][j][k] = tmp;
                //Load new chunk
                glm::ivec3  origin =  __position + glm::ivec3(world_side-1, j,k);
                __chunks[tmp] = Chunk::Load(__sourceFolder, origin);
                __chunkPositions[tmp] = glm::vec4(16*origin, 1);
            }
        }
    }
    void ChunkLoader::__moveNegativeX() {
        __position.x--;
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                __chunks[__chunkIndices[world_side-1][j][k]]->save();
                delete __chunks[__chunkIndices[world_side-1][j][k]];
                //Transform chunk indices table
                int tmp = __chunkIndices[world_side-1][j][k];
                for (int i = world_side-1; i>0; i--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i-1][j][k];
                __chunkIndices[0][j][k] = tmp;
                //Load new chunk
                glm::ivec3 origin =  __position + glm::ivec3(0, j,k);
                __chunks[tmp] = Chunk::Load(__sourceFolder, origin);
                __chunkPositions[tmp] = glm::vec4(16*origin, 1);
            }
        }
    }
    void ChunkLoader::__movePositiveY() {
        __position.y++;
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                __chunks[__chunkIndices[i][0][k]]->save();
                delete __chunks[__chunkIndices[i][0][k]];
                //Transform chunk indices table
                int tmp = __chunkIndices[i][0][k];
                for (int j = 0; j<world_side-1; j++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j+1][k];
                __chunkIndices[i][world_side-1][k] = tmp;
                //Load new chunk
                glm::ivec3  origin =  __position + glm::ivec3(i, world_side-1,k);
                __chunks[tmp] = Chunk::Load(__sourceFolder, origin);
                __chunkPositions[tmp] = glm::vec4(16*origin, 1);
            }
        }
    }
    void ChunkLoader::__moveNegativeY() {
        __position.y--;
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[i][world_side-1][k];
                __chunks[index]->save();
                delete __chunks[index];

                //Transform chunk indices table
                for (int j = world_side-1; j>0; j--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j-1][k];
                __chunkIndices[i][0][k] = index;

                //Load new chunk
                glm::ivec3  origin =  __position + glm::ivec3(i, 0,k);
                __chunks[index] = Chunk::Load(__sourceFolder, origin);
                __chunkPositions[index] = glm::vec4(16*origin, 1);
            }
        }
    }
    void ChunkLoader::__movePositiveZ() {
        __position.z++;
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                //Delete outside chunk
                __chunks[__chunkIndices[i][j][0]]->save();
                delete __chunks[__chunkIndices[i][j][0]];
                //Transform chunk indices table
                int tmp = __chunkIndices[i][j][0];
                for (int k = 0; k<world_side-1; k++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j][k+1];
                __chunkIndices[i][j][world_side-1] = tmp;
                //Load new chunk
                glm::ivec3  origin =  __position + glm::ivec3(i, j,world_side-1);
                __chunks[tmp] = Chunk::Load(__sourceFolder, origin);
                __chunkPositions[tmp] = glm::vec4(16*origin, 1);
            }
        }
    }
    void ChunkLoader::__moveNegativeZ() {
        __position.z--;
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                //Delete outside chunk
                __chunks[__chunkIndices[i][j][world_side-1]]->save();
                delete __chunks[__chunkIndices[i][j][world_side-1]];
                //Transform chunk indices table
                int tmp = __chunkIndices[i][j][world_side-1];
                for (int k = world_side-1; k>0; k--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j][k-1];
                __chunkIndices[i][j][0] = tmp;
                //Load new chunk
                glm::ivec3  origin =  __position + glm::ivec3(i, j,0);
                __chunks[tmp] = Chunk::Load(__sourceFolder, origin);
                __chunkPositions[tmp] = glm::vec4(16*origin, 1);
            }
        }
    }

    Chunk& ChunkLoader::operator[](const glm::vec3& pos) {
        int x = floor(pos.x/16-__position.x), y = floor(pos.y/16-__position.y), z= floor(pos.z/16-__position.z);
        return *__chunks[__chunkIndices[x][y][z]];
    }
    const Chunk& ChunkLoader::operator[](const glm::vec3& pos) const {
        int x = floor(pos.x/16 -__position.x), y = floor(pos.y/16 - __position.y), z= floor(pos.z/16-__position.z);
        return *__chunks[__chunkIndices[x][y][z]];
    }
    void ChunkLoader::glDraw() const {
        for (auto& chunk:__chunks) chunk->glDraw();
    }

}