#include "ChunkBase.h"
#include "Block.h"
#include "Chunk.h"
#include "ChunkManage.h"
#include "glm/geometric.hpp"
namespace MyCraft {

    ChunkObject::~ChunkObject() {}
    
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

    void ChunkObject::setLocalLight(const glm::ivec3& offset, const float& indensity) {
        glm::ivec3 position = offset + getPosition();
        setLight(position, indensity);
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

    void ChunkObject::setLocalState(const glm::ivec3& offset, const glm::mat4& state) {
        glm::ivec3 position = offset + getPosition();
        setState(position, state);
    }
    bool ChunkLoader::isInWater(const glm::ivec3& position) const {
        return getChunk(position).isInWater(position);
    }
    void ChunkLoader::setState(const glm::ivec3& pos, const glm::mat4& state) {
        getChunk(pos).setState(pos, state);
    }
    void ChunkLoader::placeDynamicWater(const glm::ivec4& position) {}
    void ChunkLoader::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        if (!type) {
            if (!isPlaceable(getType(pos))) {
                BlockCatogary currentType = getType(pos);
                getChunk(pos).disableBit(pos);
                getChunk(pos).setType(pos, BlockCatogary::Air);
                if (!isTransparent(currentType)) {
                    glm::vec3 position = pos;
                    position.x--;
                    if (!getBit(position) && !isPlaceable(getType(position))) enableBit(position);
                    position.x += 2;
                    if (!getBit(position) && !isPlaceable(getType(position))) enableBit(position);

                    position -= glm::vec3(1, 1, 0);
                    if (!getBit(position) && !isPlaceable(getType(position))) enableBit(position);
                    position.y += 2;
                    if (!getBit(position) && !isPlaceable(getType(position))) enableBit(position);
                    
                    position -= glm::vec3(0, 1, 1);
                    if (!getBit(position) && !isPlaceable(getType(position))) enableBit(position);

                    position.z += 2;
                    if (!getBit(position) && !isPlaceable(getType(position))) enableBit(position);
                }
                if (isLightSource(currentType)) setLight(pos, 0);
                placeDynamicWater(glm::vec4(pos, 0));
            }
        }
        else {
            if (isPlaceable(getType(pos))) {
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
                if (isLightSource(type)) {
                    setLight(pos, MyCraft::getLightIndensity(type));
                }
                if (type == Water) placeDynamicWater(glm::vec4(pos, 0));
                else {
                    placeDynamicWater(glm::vec4(pos+glm::ivec3(1,0,0), 10));
                    placeDynamicWater(glm::vec4(pos+glm::ivec3(-1,0,0), 10));
                    placeDynamicWater(glm::vec4(pos+glm::ivec3(0,1,0), 10));
                    placeDynamicWater(glm::vec4(pos+glm::ivec3(0,-1,0), 10));
                    placeDynamicWater(glm::vec4(pos+glm::ivec3(0,0,1), 10));
                    placeDynamicWater(glm::vec4(pos+glm::ivec3(0,0,-1), 10));
                }
            }
            else {
                setType(pos, Air);
                setType(pos, type);
            }
        }
    }
    bool ChunkLoader::pourWater(const glm::ivec3& position, const glm::vec4& height) {
        bool is_changed = false;
        glm::vec4 heights(0);
        if (getType(position)==Air || getType(position)==Water) {
            if (isInWater(position+glm::ivec3(0,0,1)) || getType(position)==Water) 
                heights = glm::ivec4(1);
            else {
                {
                    float s1 = getWaterHeight(position-glm::ivec3(1,0,0)), s2 = getWaterHeight(position-glm::ivec3(0,1,0));
                    heights[0] = std::max(heights[0], std::max(s1, s2));
                    if (s1 || s2) heights[0] = std::max(heights[0], getWaterHeight(position-glm::ivec3(1,1,0)));
                }
                {
                    float s1 =  getWaterHeight(position+glm::ivec3(1,0,0)), s2 = getWaterHeight(position-glm::ivec3(0,1,0));
                    heights[1] = std::max(heights[1], std::max(s1, s2));
                    if (s1 || s2) heights[1] = std::max(heights[1], getWaterHeight(position+glm::ivec3(1,-1,0)));
                }
                {
                    float s1 = getWaterHeight(position+glm::ivec3(1,0,0)), s2 = getWaterHeight(position+glm::ivec3(0,1,0));
                    heights[2] = std::max(heights[2], std::max(s1, s2));
                    if (s1 || s2) heights[2] = std::max(heights[2], getWaterHeight(position+glm::ivec3(1,1,0)));
                }
                {
                    float s1 = getWaterHeight(position-glm::ivec3(1,0,0)), s2 = getWaterHeight(position+glm::ivec3(0,1,0));
                    heights[3] = std::max(heights[3], std::max(s1, s2));
                    if (s1 || s2) heights[3] = std::max(heights[3], getWaterHeight(position+glm::ivec3(-1,1,0)));
                }

                if (heights == glm::vec4(1)) 
                    getChunk(position).setType(position, Water);
            }
        }
        
        if (glm::length(heights) || isInWater(position)) {
            float M = std::max(heights.x, std::max(heights.y, std::max(heights.z, heights.w)));
            if (heights[0]<M-0.2) heights[0]=M-0.2;
            if (heights[1]<M-0.2) heights[1]=M-0.2;
            if (heights[2]<M-0.2) heights[2]=M-0.2;
            if (heights[3]<M-0.2) heights[3]=M-0.2;

            is_changed = getChunk(position).pourWater(position, heights);

            glm::ivec3 cur = position;
            cur.x++;
            if (isInWater(cur)) {
                getChunk(cur).disableWaterPlane(cur, 2);
            }
            else if (isTransparent(getType(cur)))  
                getChunk(position).enableWaterPlane(position, 3);

            cur.x-=2;
            if (isInWater(cur)) {
                getChunk(cur).disableWaterPlane(cur, 3);
            }
            else if (isTransparent(getType(cur))) 
                getChunk(position).enableWaterPlane(position, 2);

            cur += glm::vec3(1, 1, 0);
            if (isInWater(cur)) {
                getChunk(cur).disableWaterPlane(cur, 1);
            }
            else if (isTransparent(getType(cur))) 
                getChunk(position).enableWaterPlane(position, 4);

            cur.y -= 2;
            if (isInWater(cur)) {
                getChunk(cur).disableWaterPlane(cur, 4);
            }
            else if (isTransparent(getType(cur))) 
                getChunk(position).enableWaterPlane(position, 1);

            cur += glm::vec3(0, 1, 1);
            if (isInWater(cur)) {
                getChunk(cur).disableWaterPlane(cur, 5);
            }
            else if (isTransparent(getType(cur))) 
                getChunk(position).enableWaterPlane(position, 0);

            cur.z -= 2;
            if (isInWater(cur)) {
                getChunk(cur).disableWaterPlane(cur, 0);
            }
            else if (isTransparent(getType(cur)))
                getChunk(position).enableWaterPlane(position, 5);
        }
        
        return is_changed;
    }
    bool ChunkLoader::takeWater(const glm::ivec3& position) {
        if (isInWater(position)) {
            float M = 0;
            M = std::max(M, getWaterHeight(position+glm::ivec3(1,0,0)));
            M = std::max(M, getWaterHeight(position+glm::ivec3(-1,0,0)));
            M = std::max(M, getWaterHeight(position+glm::ivec3(0,1,0)));
            M = std::max(M, getWaterHeight(position+glm::ivec3(0,-1,0)));
            if (M<=getWaterHeight(position)) 
                return getChunk(position).takeWater(position);
        }
        return false;
    }
    float ChunkLoader::getWaterHeight(const glm::ivec3& position) const {
        return getChunk(position).getWaterHeight(position);
    }
    float ChunkLoader::getLightIndensity(const glm::ivec3& position) const {
        return tree.getLightIndensity(position);
    }
    
    void ChunkLoader::removeLight(const glm::ivec3& position) {
        tree.remove(position);
    }
    void ChunkLoader::setLight(const glm::ivec3& position, const float& indensity) {
        glm::ivec3 offset = position - getPosition();
        if (offset.x < 0 || offset.x >= 16*world_side || offset.y < 0 ||  offset.y >= 16*world_side || offset.z < 0 ||  offset.z >= 16*world_side) 
            throw std::runtime_error("Out range of chunk");
        glm::ivec3 cPosition = position;
        if (indensity>0) {
            unsigned char side = 1;
            int I = indensity;
            tree.insert(position, indensity);
            if (contains(position)) getChunk(position).setLight(position, I);
            while (I>0) {
                I--;
                side += 2;
                cPosition -= glm::ivec3(1);
                glm::ivec3 position = cPosition; 
                for (position.y = cPosition.y;position.y < cPosition.y + side; position.y++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position)) 
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }
                position.x = cPosition.x + side - 1;
                for (position.y = cPosition.y;position.y < cPosition.y + side; position.y++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position)) 
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }

                position.y = cPosition.y;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position)) 
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }

                position.y = cPosition.y + side - 1;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position)) 
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }

                position.z = cPosition.z;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.y = cPosition.y; position.y < cPosition.y + side; position.y++) {
                        if (contains(position)) 
                            getChunk(position).setLight(position, tree.getLightIndensity(position));

                    }
                }

                position.z = cPosition.z + side - 1;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.y = cPosition.y; position.y < cPosition.y + side; position.y++) {
                        if (contains(position)) 
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }
            }
        }
        else {
            unsigned char side = 1;
            float I = tree.remove(position);
            while (I>0) {
                I--;
                side += 2;
                cPosition -= glm::ivec3(1);
                glm::ivec3 position = cPosition; 
                for (position.y = cPosition.y;position.y < cPosition.y + side; position.y++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position))
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }
                position.x = cPosition.x + side - 1;
                for (position.y = cPosition.y;position.y < cPosition.y + side; position.y++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position))
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }

                position.y = cPosition.y;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position))
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }

                position.y = cPosition.y + side - 1;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.z = cPosition.z; position.z < cPosition.z + side; position.z++) {
                        if (contains(position))
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }

                position.z = cPosition.z;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.y = cPosition.y; position.y < cPosition.y + side; position.y++) {
                        if (contains(position))
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }

                position.z = cPosition.z + side - 1;
                for (position.x = cPosition.x;position.x < cPosition.x + side; position.x++) {
                    for (position.y = cPosition.y; position.y < cPosition.y + side; position.y++) {
                        if (contains(position))
                            getChunk(position).setLight(position, tree.getLightIndensity(position));
                    }
                }
            }

        }
    }
    void ChunkLoader::pushDynamicWater(const glm::ivec4& position) {
        getChunk(position).pushDynamicWater(position);
    }
    void ChunkLoader::popDynamicWater(const glm::ivec4& position) {
        getChunk(position).popDynamicWater(position);
    }
    void ChunkLoader::enableBit(const glm::ivec3& position) {
        getChunk(position).enableBit(position);
    }
    void ChunkLoader::disableBit(const glm::ivec3& pos) {
        if (!getBit(pos)) return;
        glm::ivec3 position = pos;
        position.x -= 1;
        if (isTransparent(getType(position))) return;
        
        position.x += 2;
        if (isTransparent(getType(position))) return ;
        
        position -= glm::vec3(1, 1, 0);
        if (isTransparent(getType(position))) return ;
        position.y += 2;
        if (isTransparent(getType(position))) return ;

        position -= glm::vec3(0, 1, 1);
        if (isTransparent(getType(position))) return ;
        position.z += 2;
        if (isTransparent(getType(position))) return ;

        getChunk(pos).disableBit(pos);
    }

    glm::mat4 ChunkLoader::getState(const glm::ivec3& position) const {
        return getChunk(position).getState(position);
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