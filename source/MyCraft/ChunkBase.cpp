#include "ChunkBase.h"
#include "Block.h"
#include "Chunk.h"
#include "ChunkManage.h"
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
    void ChunkLoader::setType(const glm::ivec3& pos, const BlockCatogary& type) {
        if (!type) {
            if (getType(pos)) {
                BlockCatogary currentType = getType(pos);
                getChunk(pos).disableBit(pos);
                getChunk(pos).setType(pos, BlockCatogary::Air);
                if (!isTransparent(currentType)) {
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
                if (isLightSource(currentType)) setLight(pos, 0);
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
                if (isLightSource(type)) {
                    setLight(pos, MyCraft::getLightIndensity(type));
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
        if (getChunk(position).getType(position)==Air || getChunk(position).getType(position)==Water) {
            glm::vec4 heights(height);
            heights[0] = std::max(heights[0], getWaterHeight(position-glm::ivec3(1,0,0)));
            heights[0] = std::max(heights[0], getWaterHeight(position-glm::ivec3(0,1,0)));
            heights[0] = std::max(heights[0], getWaterHeight(position-glm::ivec3(1,1,0)));
            heights[0] = std::max(heights[0], getWaterHeight(position));

            heights[1] = std::max(heights[1], getWaterHeight(position+glm::ivec3(1,0,0)));
            heights[1] = std::max(heights[1], getWaterHeight(position-glm::ivec3(0,1,0)));
            heights[1] = std::max(heights[1], getWaterHeight(position+glm::ivec3(1,-1,0)));
            heights[1] = std::max(heights[1], getWaterHeight(position));

            heights[2] = std::max(heights[2], getWaterHeight(position+glm::ivec3(1,0,0)));
            heights[2] = std::max(heights[2], getWaterHeight(position+glm::ivec3(0,1,0)));
            heights[2] = std::max(heights[2], getWaterHeight(position+glm::ivec3(1,1,0)));
            heights[2] = std::max(heights[2], getWaterHeight(position));

            heights[3] = std::max(heights[3], getWaterHeight(position-glm::ivec3(1,0,0)));
            heights[3] = std::max(heights[3], getWaterHeight(position+glm::ivec3(0,1,0)));
            heights[3] = std::max(heights[3], getWaterHeight(position+glm::ivec3(-1,1,0)));
            heights[3] = std::max(heights[3], getWaterHeight(position));

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
    void ChunkLoader::pushDynamicWater(const glm::ivec3& position) {}
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