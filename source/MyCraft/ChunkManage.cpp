#include "ChunkManage.h"
#include "ChunkBase.h"
#include "Chunk.h"
#include "Container3D.h"
#include "DrawingCenter.h"
#include "General.h"
namespace MyCraft {
    ChunkManage::ChunkManage(const std::string& src): __isLoaded(false), __sourceFolder(src), __waterManage(*this) {
        insert(&__waterManage);
        __chunks.resize(world_side*world_side*world_side, 0);
        __chunkPositions.resize(world_side*world_side*world_side);
        for (int i = 0; i<world_side; i++)
            for (int j = 0; j<world_side; j++) 
                for (int k = 0; k<world_side; k++) 
                    __chunkIndices[i][j][k] = i*world_side*world_side + j*world_side + k;
        __texture.load("assets/images/blockCatogary.png", false);
    }
    ChunkManage::~ChunkManage() {
        for (auto& chunk:__chunks) {
            chunk->save();
            delete chunk;
        }
    }
    bool ChunkManage::contains(const glm::ivec3& position) const {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= getPosition()/16;
        return (offset.x >= 0 && offset.x < world_side && offset.y >= 0 && offset.y < world_side && offset.z >= 0 && offset.z < world_side  && __chunks[__chunkIndices[offset.x][offset.y][offset.z]]);
    }
    const std::vector<glm::vec4>& ChunkManage::getChunks() const {
        return __chunkPositions;
    }
    void ChunkManage::playerAt(const glm::ivec3& pos) {
        glm::ivec3 position(floor(pos.x/16.f) - floor(world_side/2.f), floor(pos.y/16.f) - floor(world_side/2.f), floor(pos.z/16.f) - floor(world_side/2.f));
        glm::ivec3 delta = position - __position/16;
        float length = glm::length((glm::vec3)delta);
        if (!__isLoaded || length>2) {
            if (!__isLoaded) __isLoaded = true;
            else {
                for (auto& chunk: __chunks) {
                    chunk->save();
                    delete chunk;
                    chunk = 0;
                }
            }
            __position = position*16;
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
                    glm::ivec3 origin = __position/16 + glm::ivec3(i,j,k);
                    __chunks[__chunkIndices[i][j][k]] = Chunk::Load(this, __sourceFolder, origin);
                    __chunkPositions[__chunkIndices[i][j][k]] = glm::ivec4(origin*16, 16);
                }
            }
        }
    }
    void ChunkManage::__movePositiveX() {
        __position.x+=16;
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[0][j][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int i = 0; i<world_side-1; i++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i+1][j][k];
                __chunkIndices[world_side-1][j][k] = index;
            }
        }
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Load new chunk
                int index = __chunkIndices[world_side-1][j][k];
                glm::ivec3  origin =  __position/16 + glm::ivec3(world_side-1, j,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
    }
    void ChunkManage::__moveNegativeX() {
        __position.x-=16;
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[world_side-1][j][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int i = world_side-1; i>0; i--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i-1][j][k];
                __chunkIndices[0][j][k] = index;
            }
        }
        for (int j = 0; j<world_side; j++) {
            for (int k = 0; k<world_side; k++) {
                int index = __chunkIndices[0][j][k];
                //Load new chunk
                glm::ivec3 origin =  __position/16 + glm::ivec3(0, j,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
    }
    void ChunkManage::__movePositiveY() {
        __position.y+=16;
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[i][0][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int j = 0; j<world_side-1; j++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j+1][k];
                __chunkIndices[i][world_side-1][k] = index;
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                int index = __chunkIndices[i][world_side-1][k];
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, world_side-1,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
    }
    void ChunkManage::__moveNegativeY() {
        __position.y-=16;
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                //Delete outside chunk
                int index = __chunkIndices[i][world_side-1][k];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;

                //Transform chunk indices table
                for (int j = world_side-1; j>0; j--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j-1][k];
                __chunkIndices[i][0][k] = index;
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int k = 0; k<world_side; k++) {
                int index = __chunkIndices[i][0][k];
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, 0,k);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
    }
    void ChunkManage::__movePositiveZ() {
        __position.z+=16;
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                //Delete outside chunk
                int index = __chunkIndices[i][j][0];
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int k = 0; k<world_side-1; k++) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j][k+1];
                __chunkIndices[i][j][world_side-1] = index;
                
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                int index = __chunkIndices[i][j][world_side-1];
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, j,world_side-1);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
    }
    void ChunkManage::__moveNegativeZ() {
        __position.z-=16;
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                int index = __chunkIndices[i][j][world_side-1];
                //Delete outside chunk
                __chunks[index]->save();
                delete __chunks[index];
                __chunks[index] = 0;
                //Transform chunk indices table
                for (int k = world_side-1; k>0; k--) 
                    __chunkIndices[i][j][k] = __chunkIndices[i][j][k-1];
                __chunkIndices[i][j][0] = index;
            }
        }
        for (int i = 0; i<world_side; i++) {
            for (int j = 0; j<world_side; j++) {
                int index = __chunkIndices[i][j][0];;
                //Load new chunk
                glm::ivec3  origin =  __position/16 + glm::ivec3(i, j,0);
                __chunks[index] = Chunk::Load(this, __sourceFolder, origin);;
                __chunkPositions[index] = glm::ivec4(16*origin, 16);
            }
        }
    }
    void ChunkManage::glDraw() const {
        MyBase3D::Container3D::glDraw();
        DrawingCenter::BindCube(__texture);
        glLineWidth(0);
        for (auto& chunk:__chunks) chunk->glDraw();
    }
    void ChunkManage::pushDynamicWater(const glm::ivec3& position) {
        __waterManage.place(position);
    }
    void ChunkManage::glDrawTransparent() const {
        MyBase3D::Container3D::glDrawTransparent();
        DrawingCenter::BindCube(__texture);
        glLineWidth(0);
        for (auto& chunk:__chunks) chunk->glDrawTransparent();
    }
    Chunk& ChunkManage::getChunk(const glm::ivec3& position) {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= __position/16;
        if (offset.x >= world_side || offset.y >= world_side || offset.z >= world_side || !__chunks[__chunkIndices[offset.x][offset.y][offset.z]])
            throw std::runtime_error("Out range of storage!");

        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    const Chunk& ChunkManage::getChunk(const glm::ivec3& position) const {
        glm::ivec3 offset(floor(position.x/16.f), floor(position.y/16.f), floor(position.z/16.f));
        offset -= __position/16;
        if (offset.x >= world_side || offset.y >= world_side || offset.z >= world_side || !__chunks[__chunkIndices[offset.x][offset.y][offset.z]])
            throw std::runtime_error("Out range of storage!");
        return *__chunks[__chunkIndices[offset.x][offset.y][offset.z]];
    }
    const glm::ivec3& ChunkManage::getPosition() const {
        return __position;
    };

}