#include "ChunkBase.h"
#include "Block.h"
#include "Chunk.h"
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
                if (!isTransparent(getType(pos))) {
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
        }
        else {
            if (!getType(pos)) {
                getChunk(pos).setType(pos, type);
                getChunk(pos).enableBit(pos);
                if (!isTransparent(getType(pos))) {
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
            }
            else if (isTransparent(type) == isTransparent(getType(pos))) 
                getChunk(pos).setType(pos, type);
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

}